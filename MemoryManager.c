#include "CoreMini.h"

#include "Engine.h"

#include "MemoryManager.h"

#include "Queue.h"
#include "String.h"
#include "tData.h"

typedef struct MemoryBlock MemoryBlock;
typedef struct MemoryProfileData MemoryProfileData;

struct MemoryProfileData
{
    String* m_local_name;
    crc32   m_crc;
    tsize   m_alloc_size;
    tsize   m_alloc_size_max;
};

static void MemoryProfileData_Destroy(MemoryProfileData* memory_profile_data)
{
    String_Del(memory_profile_data->m_local_name);
    MemDel(memory_profile_data);
}

struct MemoryManager
{
    uint32                      m_alloc_id;
    String*                     m_local_name;
    MemoryProfileData           m_static_memory;
    Queue(MemoryProfileData*)*  m_memory_profile_data_queue;
};


tptr   Memory_Alloc_Plat   (tsize size);
void    Memory_Free_Plat    (void* ptr);
tptr   Memory_Copy_Plat    (tptr dst_ptr, const tptr src_ptr, tsize size);
tptr   Memory_Set_Plat     (tptr address, int32 val, tsize size);

static tsize static_alloc_memory_size = 0;


//  +----+---------------+-------------------
//  |tptr|size| id | crc |  Memory Data ...
//  +----+----+----+-----+-------------------
//  |block               |ptr

struct MemoryBlock
{
    tptr    m_pointer;
    tsize   m_alloc_size;
    uint32  m_id;
    crc32   m_crc;
    byte    m_byte[1];
};

static uint32 MemoryManager_IncreaseAllocID(MemoryManager* memory_manager)
{
    Assert(memory_manager != NULL, "You need to create MemoryManager first!");
    return ++memory_manager->m_alloc_id;
}

static MemoryBlock* CastToMemoryBlock(tptr ptr)
{
    MemoryBlock* memory_block = (MemoryBlock*)((tchar*)ptr - OFFSET_OF(MemoryBlock, m_byte));
    Assert(memory_block->m_pointer == ptr, "");
    return memory_block;
};

////////////////////////////////////////////////////////////////////////////////
MemoryManager* MemoryManager_Create(const tchar* local_name)
{
    MemoryManager* memory_manager = MemNew(local_name, MemoryManager);
    memory_manager->m_alloc_id = 0;
    memory_manager->m_static_memory.m_crc = 0;
    memory_manager->m_static_memory.m_alloc_size = 0;
    memory_manager->m_static_memory.m_alloc_size_max = 0;
    memory_manager->m_static_memory.m_local_name = NULL;
    memory_manager->m_local_name = String_New(local_name, local_name, true);

    memory_manager->m_memory_profile_data_queue = Queue_Create(local_name, MemoryProfileData*);
    return memory_manager;
}

void MemoryManager_Destroy(MemoryManager* memory_manager)
{
    Queue_Destroy(memory_manager->m_memory_profile_data_queue, MemoryProfileData_Destroy);
    String_Del(memory_manager->m_local_name);

    Log(4, "%-30s: %d\n", "memory alloc count", memory_manager->m_alloc_id);

    MemDel(memory_manager);
}

static bool CallBack_Memory_Profile_FindData(MemoryProfileData* memory_profile_data, crc32 crc)
{
    return memory_profile_data->m_crc == crc;
}

tptr MemoryManager_Alloc(MemoryManager* memory_manager,const tchar* local_name, tsize size)
{
    const bool is_dynamic = !Str_IsSame(local_name, "MemoryManager");
    MemoryBlock * memory_block  = Memory_Alloc_Plat(sizeof(MemoryBlock) + size);
    Assert(memory_block != NULL, "");
    memory_block->m_pointer     = memory_block->m_byte;
    memory_block->m_id          = is_dynamic ? MemoryManager_IncreaseAllocID(memory_manager) : 0;
    memory_block->m_crc         = Str_CalcCrc(local_name, 0);
    memory_block->m_alloc_size  = size;

    if( is_dynamic )
    {
        Queue(MemoryProfileData*)* memory_profile_data_queue = memory_manager->m_memory_profile_data_queue;
        MemoryProfileData* memory_profile_data = Queue_Find(MemoryProfileData*)( memory_profile_data_queue, (CB_FindData_Bool_tPtr_tPtr)CallBack_Memory_Profile_FindData, (tptr)memory_block->m_crc );
        if( memory_profile_data )
        {
            memory_profile_data->m_alloc_size += size;
            if( memory_profile_data->m_alloc_size > memory_profile_data->m_alloc_size_max )
            {
                memory_profile_data->m_alloc_size_max = memory_profile_data->m_alloc_size;
            }
        }
        else
        {
            const tchar* memory_manager_local_name = String_CStr(memory_manager->m_local_name);
            memory_profile_data = MemNew(memory_manager_local_name, MemoryProfileData);
            memory_profile_data->m_crc = memory_block->m_crc;
            memory_profile_data->m_local_name = String_New(memory_manager_local_name, local_name, true);
            memory_profile_data->m_alloc_size = size;
            memory_profile_data->m_alloc_size_max = size;
            Queue_Push(MemoryProfileData*, NULL, memory_profile_data_queue, memory_profile_data);
        }
    }
    else
    {
        static_alloc_memory_size += size;
    }

    return memory_block->m_byte;
}

void MemoryManager_Free(MemoryManager* memory_manager, tptr ptr)
{
    Assert(ptr != NULL, "");

    MemoryBlock * memory_block = CastToMemoryBlock(ptr);

    if( !Engine_IsExit() )
    {
        Assert( memory_block->m_id != 0, "You try to free a static memory!");
    }
    const bool is_dynamic = memory_block->m_id != 0 ? true : false;
    
    if( is_dynamic )
    {
        Queue(MemoryProfileData*)* memory_profile_data_queue = memory_manager->m_memory_profile_data_queue;
        MemoryProfileData* memory_profile_data = Queue_Find(MemoryProfileData*)( memory_profile_data_queue, (CB_FindData_Bool_tPtr_tPtr)CallBack_Memory_Profile_FindData, (tptr)memory_block->m_crc );
        if( memory_profile_data )
        {
            memory_profile_data->m_alloc_size -= memory_block->m_alloc_size;
            Assert(memory_profile_data->m_alloc_size >= 0, "");
        }
    }
    else
    {
        static_alloc_memory_size -= memory_block->m_alloc_size;
        Assert(static_alloc_memory_size >= 0, "");
    }

    Memory_Free_Plat(memory_block);
}

tptr MemoryManager_AllocPtrSize(MemoryManager* memory_manager, const tchar* local_name, const tptr ptr)
{
    Assert(ptr != NULL, "");

    MemoryBlock* src_block = CastToMemoryBlock(ptr);

    const tptr new_ptr = MemoryManager_Alloc(memory_manager, local_name, src_block->m_alloc_size);
    return new_ptr;
}

tptr MemoryManager_Clone(MemoryManager* memory_manager, const tchar* local_name, const tptr ptr)
{
    Assert(ptr != NULL, "");
    MemoryBlock* src_block = CastToMemoryBlock(ptr);

    const tptr new_ptr = MemoryManager_AllocPtrSize(memory_manager, local_name, ptr);
    Memory_Copy(new_ptr, ptr, src_block->m_alloc_size);

    return new_ptr;
}

tptr MemoryManager_SafeClone(MemoryManager* memory_manager, const tchar* local_name, const tptr ptr)
{
    if( !ptr )
    {
        return NULL;
    }

    return
    MemoryManager_Clone(memory_manager, local_name, ptr);
}

tptr Memory_Copy(tptr dst_ptr, const tptr src_ptr, tsize size)
{
    Assert(dst_ptr != NULL, "");
    Assert(src_ptr != NULL, "");

    MemoryBlock* dst_block = CastToMemoryBlock(dst_ptr);
    MemoryBlock* src_block = CastToMemoryBlock(src_ptr);

    Assert( dst_block->m_alloc_size >= size, "");
    Assert( src_block->m_alloc_size >= size, "");

    return
    Memory_Copy_Plat(dst_ptr, src_ptr, size);
}

tptr Memory_Set(tptr ptr, int32 val, tsize size)
{
    Assert(ptr != NULL, "");

    MemoryBlock* memory_block = CastToMemoryBlock(ptr);
    Assert( memory_block->m_alloc_size >= size, "");

    return
    Memory_Set_Plat(ptr, val, size);
}

tptr Memory_FullFill(tptr ptr, int32 val)
{
    Assert(ptr != NULL, "");

    MemoryBlock* memory_block = CastToMemoryBlock(ptr);

    return
    Memory_Set_Plat(ptr, val, memory_block->m_alloc_size);
}

tptr Memory_Zero(tptr ptr)
{
    Assert(ptr != NULL, "");

    MemoryBlock* memory_block = CastToMemoryBlock(ptr);

    return
    Memory_Set_Plat(ptr, 0, memory_block->m_alloc_size);
}

tsize Memory_GetSize(const tptr ptr)
{
    Assert(ptr != NULL, "");

    MemoryBlock* memory_block = CastToMemoryBlock(ptr);
    return memory_block->m_alloc_size;
}

bool Memory_IsInBounds(tptr head_ptr, tptr check_ptr)
{
    MemoryBlock* memory_block = CastToMemoryBlock(head_ptr);
    const tsize size_offset = tSize(check_ptr) - tSize(head_ptr);
    return size_offset < memory_block->m_alloc_size;
}

////////////////////////////////////////////////////////////////////////////////
#undef Engine_Profile_Memory

static void CallBack_Memory_ProfileLog(MemoryProfileData* memory_profile_data, uint32* total_alloc_size)
{
    if( memory_profile_data->m_alloc_size != 0 )
    {
        Log(4, "%-30s: %-8d / %-8d \n", String_CStr(memory_profile_data->m_local_name), memory_profile_data->m_alloc_size, memory_profile_data->m_alloc_size_max);
        *total_alloc_size += memory_profile_data->m_alloc_size;
    }
}

void Engine_Profile_Memory()
{
    uint32 total_alloc_size = 0;
    MemoryManager* memory_manager = MemoryManager_GetInstance();
    Queue(MemoryProfileData*)* memory_profile_data_queue = memory_manager->m_memory_profile_data_queue;
    Log(4, "Memory Profile\n");
    Log(4, "=====================================================\n");
    Queue_ForEach( memory_profile_data_queue, CallBack_Memory_ProfileLog, &total_alloc_size );
    Log(4, "%-30s: %d\n", "Static", static_alloc_memory_size);
    total_alloc_size += static_alloc_memory_size;
    Log(4, "%-30s: %d\n", "Total", total_alloc_size);
    Log(4, "%-30s: %d\n", "memory alloc count", memory_manager->m_alloc_id);
    Log(4, "=====================================================\n");
}

static void CallBack_Memory_Check_Memory_Leak(MemoryProfileData* memory_profile_data, tptr ptr)
{
    Assert(memory_profile_data->m_alloc_size == 0, "There has a memory leak!");
}

void Engine_Debug_Memory_Check_Leak()
{
    MemoryManager* memory_manager = MemoryManager_GetInstance();
    Assert(memory_manager != NULL, "You had free MemoryManager? If you want to check static memory, use Engine_Debug_Memory_Static_Check_Leak.");
    Queue(MemoryProfileData*)* memory_profile_data_queue = memory_manager->m_memory_profile_data_queue;
    Queue_ForEach( memory_profile_data_queue, CallBack_Memory_Check_Memory_Leak, NULL );
}

void Engine_Debug_Memory_Static_Check_Leak()
{
    Assert( static_alloc_memory_size == 0, "Memory leak!");
}
