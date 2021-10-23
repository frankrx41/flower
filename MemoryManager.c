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
    strcrc  m_local_name;
    crc32   m_crc;
    tsize   m_alloc_size;
    tsize   m_alloc_size_max;
};

static void MemoryProfileData_Destroy(MemoryProfileData* memory_profile_data)
{
    MemDel(memory_profile_data);
}

tptr    Memory_Alloc_Plat   (tsize size);
void    Memory_Free_Plat    (void* ptr);
tptr    Memory_Copy_Plat    (tptr dst_ptr, const tptr src_ptr, tsize size);
tptr    Memory_Set_Plat     (tptr address, int32 val, tsize size);

static tsize static_alloc_memory_size = 0;


//  +----+---------+-------------------
//  |tptr|size| crc|  Memory Data ...
//  +----+----+----+-------------------
//  |block         |ptr

struct MemoryBlock
{
    tptr    m_pointer;
    tsize   m_alloc_size;
    crc32   m_crc;
    byte    m_byte[1];
};

static MemoryBlock* CastToMemoryBlock(tptr ptr)
{
    MemoryBlock* memory_block = (MemoryBlock*)((tchar*)ptr - OFFSET_OF(MemoryBlock, m_byte));
    Assert(memory_block->m_pointer == ptr, "");
    return memory_block;
};

////////////////////////////////////////////////////////////////////////////////
static bool CallBack_Memory_Profile_FindData(MemoryProfileData* memory_profile_data, crc32 crc)
{
    return memory_profile_data->m_crc == crc;
}

tptr Memory_Alloc(const strcrc* local_name, tsize size)
{
    MemoryBlock * memory_block  = Memory_Alloc_Plat(sizeof(MemoryBlock) + size);
    Assert(memory_block != NULL, "");
    memory_block->m_pointer     = memory_block->m_byte;
    memory_block->m_crc         = local_name->m_crc32;
    memory_block->m_alloc_size  = size;

    return memory_block->m_byte;
}

void Memory_Free(tptr ptr)
{
    Assert(ptr != NULL, "");

    MemoryBlock * memory_block = CastToMemoryBlock(ptr);

    Memory_Free_Plat(memory_block);
}

tptr Memory_AllocPtrSize(const strcrc* local_name, const tptr ptr)
{
    Assert(ptr != NULL, "");

    MemoryBlock* src_block = CastToMemoryBlock(ptr);

    const tptr new_ptr = Memory_Alloc(local_name, src_block->m_alloc_size);
    return new_ptr;
}

tptr Memory_Clone(const strcrc* local_name, const tptr ptr)
{
    Assert(ptr != NULL, "");
    MemoryBlock* src_block = CastToMemoryBlock(ptr);

    const tptr new_ptr = Memory_AllocPtrSize(local_name, ptr);
    Memory_Copy(new_ptr, ptr, src_block->m_alloc_size);

    return new_ptr;
}

tptr Memory_SafeClone(const strcrc* local_name, const tptr ptr)
{
    if( !ptr )
    {
        return NULL;
    }

    return
    Memory_Clone(local_name, ptr);
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
        Log(4, "%-30s: %-8d / %-8d \n", memory_profile_data->m_local_name.m_str, memory_profile_data->m_alloc_size, memory_profile_data->m_alloc_size_max);
        *total_alloc_size += memory_profile_data->m_alloc_size;
    }
}

void Engine_Profile_Memory()
{
    // uint32 total_alloc_size = 0;
    // MemoryManager* memory_manager = MemoryManager_GetInstance();
    // Queue(MemoryProfileData*)* memory_profile_data_queue = memory_manager->m_memory_profile_data_queue;
    // Log(4, "Memory Profile\n");
    // Log(4, "=====================================================\n");
    // Queue_ForEach( memory_profile_data_queue, CallBack_Memory_ProfileLog, &total_alloc_size );
    // Log(4, "%-30s: %d\n", "Static", static_alloc_memory_size);
    // total_alloc_size += static_alloc_memory_size;
    // Log(4, "%-30s: %d\n", "Total", total_alloc_size);
    // Log(4, "%-30s: %d\n", "memory alloc count", memory_manager->m_alloc_id);
    // Log(4, "=====================================================\n");
}

static void CallBack_Memory_Check_Memory_Leak(MemoryProfileData* memory_profile_data, tptr ptr)
{
    Assert(memory_profile_data->m_alloc_size == 0, "There has a memory leak!");
}

void Engine_Debug_Memory_Check_Leak()
{
    // MemoryManager* memory_manager = MemoryManager_GetInstance();
    // Assert(memory_manager != NULL, "You had free MemoryManager? If you want to check static memory, use Engine_Debug_Memory_Static_Check_Leak.");
    // Queue(MemoryProfileData*)* memory_profile_data_queue = memory_manager->m_memory_profile_data_queue;
    // Queue_ForEach( memory_profile_data_queue, CallBack_Memory_Check_Memory_Leak, NULL );
}

void Engine_Debug_Memory_Static_Check_Leak()
{
    Assert( static_alloc_memory_size == 0, "Memory leak!");
}
