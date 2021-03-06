#include "CoreMini.h"

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

struct MemoryManager
{
    uint32                      m_alloc_id;
    MemoryProfileData           m_static_memory;
    Queue(MemoryProfileData*)*  m_local_name_queue;
};


tptr    Memory_Alloc_Plat   (tsize size);
void    Memory_Free_Plat    (tptr ptr);
tptr    Memory_Copy_Plat    (tptr dst_ptr, const tptr src_ptr, tsize size);
tptr    Memory_Set_Plat     (tptr address, int32 val, tsize size);



#define MAGIC_NUMBER_CHAR   'GPYM'
#define MAGIC_NUMBER_STR    "GPYM"
#define LOCAL_NAME          "GPYM"


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

static MemoryBlock* CastToMemoryBlock(tptr ptr)
{
    MemoryBlock* memory_block;
    memory_block = (MemoryBlock*)((tchar*)ptr - offsetof(MemoryBlock, m_byte));
    Assert(memory_block->m_flag == MAGIC_NUMBER_CHAR, "");
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

    memory_manager->m_local_name_queue = Queue_Create(LOCAL_NAME, MemoryProfileData*);
    return memory_manager;
}

static bool CallBack_Memory_Profile_FindData(MemoryProfileData* memory_profile_data, crc32 crc)
{
    return memory_profile_data->m_crc == crc;
}

tptr MemoryManager_Alloc(MemoryManager* memory_manager,const tchar* local_name, tsize size)
{
    bool is_profile = !Str_IsSame(local_name, MAGIC_NUMBER_STR);
    MemoryBlock * memory_block  = Memory_Alloc_Plat(sizeof(MemoryBlock) + size);
    Assert(memory_block != NULL, "");
    memory_block->m_flag        = MAGIC_NUMBER_CHAR;
    memory_block->m_id          = is_profile ? MemoryManager_IncreaseAllocID(memory_manager) : 0;
    memory_block->m_crc         = Str_CalcCrc(local_name, 0);
    memory_block->m_alloc_size  = size;

    if( is_profile )
    {
        Queue(MemoryProfileData*)* memory_profile_data_queue = memory_manager->m_local_name_queue;
        MemoryProfileData* memory_profile_data = Queue_Find(MemoryProfileData*)( memory_profile_data_queue, (CB_FindData)CallBack_Memory_Profile_FindData, (tptr)memory_block->m_crc );
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
            memory_profile_data = MemNew(LOCAL_NAME, MemoryProfileData);
            memory_profile_data->m_crc = memory_block->m_crc;
            memory_profile_data->m_local_name = String_New(LOCAL_NAME, local_name);
            memory_profile_data->m_alloc_size = size;
            memory_profile_data->m_alloc_size_max = size;
            Queue_Push(MemoryProfileData*, NULL, memory_profile_data_queue, memory_profile_data);
        }
    }
    else
    {
        // MemoryProfileData* memory_profile_data = &memory_manager->m_static_memory;
        // memory_profile_data->m_alloc_size += size;
        // if( memory_profile_data->m_alloc_size > memory_profile_data->m_alloc_size_max )
        // {
        //     memory_profile_data->m_alloc_size_max = memory_profile_data->m_alloc_size;
        // }
    }

    return memory_block->m_byte;
}

void MemoryManager_Free(MemoryManager* memory_manager, tptr ptr)
{
    Assert(ptr != NULL, "");


    MemoryBlock * memory_block;
    memory_block = CastToMemoryBlock(ptr);

    Assert(memory_block->m_id != 0, "You try to free a static memory!");
    bool is_profile = memory_block->m_id != 0 ? true : false;
    
    if( is_profile )
    {
        Queue(MemoryProfileData*)* memory_profile_data_queue = memory_manager->m_local_name_queue;
        MemoryProfileData* memory_profile_data = Queue_Find(MemoryProfileData*)( memory_profile_data_queue, (CB_FindData)CallBack_Memory_Profile_FindData, (tptr)memory_block->m_crc );
        if( memory_profile_data )
        {
            memory_profile_data->m_alloc_size -= memory_block->m_alloc_size;
        }
    }

    Memory_Free_Plat(memory_block);
}

tptr MemoryManager_AllocPtrSize(MemoryManager* memory_manager, const tchar* local_name, const tptr ptr)
{
    Assert(ptr != NULL, "");

    MemoryBlock * src_block;
    src_block = CastToMemoryBlock(ptr);

    tptr new_ptr;
    new_ptr = MemoryManager_Alloc(memory_manager, local_name, src_block->m_alloc_size);
    return new_ptr;
}

tptr MemoryManager_Clone(MemoryManager* memory_manager, const tchar* local_name, const tptr ptr)
{
    Assert(ptr != NULL, "");
    MemoryBlock * src_block;
    src_block = CastToMemoryBlock(ptr);

    tptr new_ptr;
    new_ptr = MemoryManager_AllocPtrSize(memory_manager, local_name, ptr);
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

    MemoryBlock* dst_block;
    MemoryBlock* src_block;
    dst_block = CastToMemoryBlock(dst_ptr);
    src_block = CastToMemoryBlock(src_ptr);

    Assert( dst_block->m_alloc_size >= size, "");
    Assert( src_block->m_alloc_size >= size, "");

    return
    Memory_Copy_Plat(dst_ptr, src_ptr, size);
}

tptr Memory_Set(tptr ptr, int32 val, tsize size)
{
    Assert(ptr != NULL, "");

    MemoryBlock * dst_block;
    dst_block = CastToMemoryBlock(ptr);
    Assert( dst_block->m_alloc_size >= size, "");

    return
    Memory_Set_Plat(ptr, val, size);
}

tptr Memory_FullFill(tptr ptr, int32 val)
{
    Assert(ptr != NULL, "");

    MemoryBlock * dst_block;
    dst_block = CastToMemoryBlock(ptr);

    return
    Memory_Set_Plat(ptr, val, dst_block->m_alloc_size);
}

tptr Memory_Zero(tptr ptr)
{
    Assert(ptr != NULL, "");

    MemoryBlock * dst_block;
    dst_block = CastToMemoryBlock(ptr);

    return
    Memory_Set_Plat(ptr, 0, dst_block->m_alloc_size);
}

tsize Memory_GetSize(const tptr ptr)
{
    Assert(ptr != NULL, "");

    MemoryBlock * memory_block;
    memory_block = CastToMemoryBlock(ptr);
    return memory_block->m_alloc_size;
}

////////////////////////////////////////////////////////////////////////////////

static void CallBack_Memory_ProfileLog(MemoryProfileData* memory_profile_data, tptr ptr)
{
    Log(0, "%-20s: %d / %d \n", String_CStr(memory_profile_data->m_local_name), memory_profile_data->m_alloc_size, memory_profile_data->m_alloc_size_max);
}

#undef Engine_Profile_Memory
void Engine_Profile_Memory()
{
    MemoryManager* memory_manager = MemoryManager_GetInstance();
    Queue(MemoryProfileData*)* memory_profile_data_queue = memory_manager->m_local_name_queue;
    Log(0, "Memory Profile\n");
    Log(0, "=====================================================\n");
    Queue_ForEach( memory_profile_data_queue, CallBack_Memory_ProfileLog, NULL );
    Log(0, "=====================================================\n");
}

static void CallBack_Memory_Check_Memory_Leak(MemoryProfileData* memory_profile_data, tptr ptr)
{
    Assert(memory_profile_data->m_alloc_size == 0, "There has a memory leak!");
}

void Engine_Debug_Memory_Check_Leak()
{
    MemoryManager* memory_manager = MemoryManager_GetInstance();
    Queue(MemoryProfileData*)* memory_profile_data_queue = memory_manager->m_local_name_queue;
    Queue_ForEach( memory_profile_data_queue, CallBack_Memory_Check_Memory_Leak, NULL );
}
