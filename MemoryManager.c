#include "CoreMini.h"

#include "Engine.h"

#include "MemoryManager.h"

#include "Queue.h"
#include "String.h"

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


ptr32    Memory_Alloc_Plat   (tsize size);
void    Memory_Free_Plat    (ptr32 ptr);
ptr32    Memory_Copy_Plat    (ptr32 dst_ptr, const ptr32 src_ptr, tsize size);
ptr32    Memory_Set_Plat     (ptr32 address, int32 val, tsize size);

static tsize static_alloc_memory_size = 0;

#define MAGIC_NUMBER        'GPYM'
#define STATIC_LOCAL_NAME   "GPYM"



//  +----+--------------+-------------------------
//  |flag| id |crc |size|  Memory Data ...
//  +----+----+----+----+-------------------------
//  |MYPG|              |
//  +----+--------------+-------------------------
//  |block              |ptr

struct MemoryBlock
{
    uint32  m_flag;
    uint32  m_id;
    crc32   m_crc;
    tsize   m_alloc_size;
    byte    m_byte[1];
};

static uint32 MemoryManager_IncreaseAllocID(MemoryManager* memory_manager)
{
    return ++memory_manager->m_alloc_id;
}

static MemoryBlock* CastToMemoryBlock(ptr32 ptr)
{
    MemoryBlock* memory_block;
    memory_block = (MemoryBlock*)((tchar*)ptr - offsetof(MemoryBlock, m_byte));
    Assert(memory_block->m_flag == MAGIC_NUMBER, "");
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
    memory_manager->m_local_name = String_New(local_name, local_name);

    memory_manager->m_memory_profile_data_queue = Queue_Create(local_name, MemoryProfileData*);
    return memory_manager;
}

void MemoryManager_Destroy(MemoryManager* memory_manager)
{
    Queue_Destroy(memory_manager->m_memory_profile_data_queue, MemoryProfileData_Destroy);
    String_Del(memory_manager->m_local_name);

    MemDel(memory_manager);
}

static bool CallBack_Memory_Profile_FindData(MemoryProfileData* memory_profile_data, crc32 crc)
{
    return memory_profile_data->m_crc == crc;
}

ptr32 MemoryManager_Alloc(MemoryManager* memory_manager,const tchar* local_name, tsize size)
{
    bool is_dynamic = !Str_IsSame(local_name, STATIC_LOCAL_NAME);
    MemoryBlock * memory_block  = Memory_Alloc_Plat(sizeof(MemoryBlock) + size);
    Assert(memory_block != NULL, "");
    memory_block->m_flag        = MAGIC_NUMBER;
    memory_block->m_id          = is_dynamic ? MemoryManager_IncreaseAllocID(memory_manager) : 0;
    memory_block->m_crc         = Str_CalcCrc(local_name, 0);
    memory_block->m_alloc_size  = size;

    if( is_dynamic )
    {
        Queue(MemoryProfileData*)* memory_profile_data_queue = memory_manager->m_memory_profile_data_queue;
        MemoryProfileData* memory_profile_data = Queue_Find(MemoryProfileData*)( memory_profile_data_queue, (CB_FindData)CallBack_Memory_Profile_FindData, (ptr32)memory_block->m_crc );
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
            memory_profile_data->m_local_name = String_New(memory_manager_local_name, local_name);
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

void MemoryManager_Free(MemoryManager* memory_manager, ptr32 ptr)
{
    Assert(ptr != NULL, "");


    MemoryBlock * memory_block;
    memory_block = CastToMemoryBlock(ptr);

    if( !Engine_IsExit() )
    {
        Assert( memory_block->m_id != 0, "You try to free a static memory!");
    }
    bool is_dynamic = memory_block->m_id != 0 ? true : false;
    
    if( is_dynamic )
    {
        Queue(MemoryProfileData*)* memory_profile_data_queue = memory_manager->m_memory_profile_data_queue;
        MemoryProfileData* memory_profile_data = Queue_Find(MemoryProfileData*)( memory_profile_data_queue, (CB_FindData)CallBack_Memory_Profile_FindData, (ptr32)memory_block->m_crc );
        if( memory_profile_data )
        {
            memory_profile_data->m_alloc_size -= memory_block->m_alloc_size;
        }
    }
    else
    {
        static_alloc_memory_size -= memory_block->m_alloc_size;
    }

    Memory_Free_Plat(memory_block);
}

ptr32 MemoryManager_AllocPtrSize(MemoryManager* memory_manager, const tchar* local_name, const ptr32 ptr)
{
    Assert(ptr != NULL, "");

    MemoryBlock * src_block;
    src_block = CastToMemoryBlock(ptr);

    ptr32 new_ptr;
    new_ptr = MemoryManager_Alloc(memory_manager, local_name, src_block->m_alloc_size);
    return new_ptr;
}

ptr32 MemoryManager_Clone(MemoryManager* memory_manager, const tchar* local_name, const ptr32 ptr)
{
    Assert(ptr != NULL, "");
    MemoryBlock * src_block;
    src_block = CastToMemoryBlock(ptr);

    ptr32 new_ptr;
    new_ptr = MemoryManager_AllocPtrSize(memory_manager, local_name, ptr);
    Memory_Copy(new_ptr, ptr, src_block->m_alloc_size);

    return new_ptr;
}

ptr32 MemoryManager_SafeClone(MemoryManager* memory_manager, const tchar* local_name, const ptr32 ptr)
{
    if( !ptr )
    {
        return NULL;
    }

    return
    MemoryManager_Clone(memory_manager, local_name, ptr);
}

ptr32 Memory_Copy(ptr32 dst_ptr, const ptr32 src_ptr, tsize size)
{
    Assert(dst_ptr != NULL, "");
    Assert(src_ptr != NULL, "");

    MemoryBlock* dst_block;
    MemoryBlock* src_block;
    dst_block = CastToMemoryBlock(dst_ptr);
    src_block = CastToMemoryBlock(src_ptr);

    Assert( dst_block->m_alloc_size >= size, "");
    Assert( src_block->m_alloc_size >= size, "");

    return
    Memory_Copy_Plat(dst_ptr, src_ptr, size);
}

ptr32 Memory_Set(ptr32 ptr, int32 val, tsize size)
{
    Assert(ptr != NULL, "");

    MemoryBlock * dst_block;
    dst_block = CastToMemoryBlock(ptr);
    Assert( dst_block->m_alloc_size >= size, "");

    return
    Memory_Set_Plat(ptr, val, size);
}

ptr32 Memory_FullFill(ptr32 ptr, int32 val)
{
    Assert(ptr != NULL, "");

    MemoryBlock * dst_block;
    dst_block = CastToMemoryBlock(ptr);

    return
    Memory_Set_Plat(ptr, val, dst_block->m_alloc_size);
}

ptr32 Memory_Zero(ptr32 ptr)
{
    Assert(ptr != NULL, "");

    MemoryBlock * dst_block;
    dst_block = CastToMemoryBlock(ptr);

    return
    Memory_Set_Plat(ptr, 0, dst_block->m_alloc_size);
}

tsize Memory_GetSize(const ptr32 ptr)
{
    Assert(ptr != NULL, "");

    MemoryBlock * memory_block;
    memory_block = CastToMemoryBlock(ptr);
    return memory_block->m_alloc_size;
}

////////////////////////////////////////////////////////////////////////////////

static void CallBack_Memory_ProfileLog(MemoryProfileData* memory_profile_data, ptr32 ptr)
{
    Log(0, "%-20s: %d / %d \n", String_CStr(memory_profile_data->m_local_name), memory_profile_data->m_alloc_size, memory_profile_data->m_alloc_size_max);
}

#undef Engine_Profile_Memory
void Engine_Profile_Memory()
{
    MemoryManager* memory_manager = MemoryManager_GetInstance();
    Queue(MemoryProfileData*)* memory_profile_data_queue = memory_manager->m_memory_profile_data_queue;
    Log(0, "Memory Profile\n");
    Log(0, "=====================================================\n");
    Queue_ForEach( memory_profile_data_queue, CallBack_Memory_ProfileLog, NULL );
    Log(0, "%-20s: %d\n", "Static", static_alloc_memory_size);
    Log(0, "=====================================================\n");
}

static void CallBack_Memory_Check_Memory_Leak(MemoryProfileData* memory_profile_data, ptr32 ptr)
{
    Assert(memory_profile_data->m_alloc_size == 0, "There has a memory leak!");
}

void Engine_Debug_Memory_Check_Leak()
{
    MemoryManager* memory_manager = MemoryManager_GetInstance();
    Queue(MemoryProfileData*)* memory_profile_data_queue = memory_manager->m_memory_profile_data_queue;
    Queue_ForEach( memory_profile_data_queue, CallBack_Memory_Check_Memory_Leak, NULL );
    // TODO: Fix memory leak
    // Assert(Engine_Debug_Memory_AllocSize_Get() == 0, "Memory leak!");
}
