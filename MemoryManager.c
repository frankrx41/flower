#include "CoreMini.h"

#include "Engine.h"

#include "MemoryManager.h"

#include "Queue.h"
#include "String.h"
#include "tData.h"
#include "Stat.h"


#define MEMORY_STAT_MAX     ( 50 )
static StatDataCount stat_alloc_size[MEMORY_STAT_MAX];
static StatDataCount stat_alloc_count;

static StatDataCount* Memory_FindStatData(const strcrc* local_name)
{
    for(int i=0; i<MEMORY_STAT_MAX; i++)
    {
        if( StrCrc_IsSame(&stat_alloc_size[i].m_local_name, local_name) )
        {
            return &stat_alloc_size[i];
        }
        
        if( StrCrc_IsEmpty(&stat_alloc_size[i].m_local_name) )
        {
            StrCrc_Copy(local_name, &stat_alloc_size[i].m_local_name);
            return &stat_alloc_size[i];
        }
    }

    Assert(false, "stat_alloc_size is full, please inc MEMORY_STAT_MAX");
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
#undef Engine_Profile_Memory

void Engine_Profile_Memory()
{
    tsize   total_alloc_size = 0;
    uint32  total_alloc_count = 0;

    Log(4, "Memory Profile:\n");
    Log(4, "=====================================================\n");

    for(int i=0; i<MEMORY_STAT_MAX; i++)
    {
        StatDataCount* stat_data_count = &stat_alloc_size[i];
        tsize size = stat_data_count->m_data_value.m_int32;
        if( StrCrc_IsEmpty(&stat_data_count->m_local_name) )
        {
            break;
        }

        if( size == 0 )
        {
            continue;
        }

        Log(4, "%-30s: %-8d / %-8d \n", stat_data_count->m_local_name.m_str, size, stat_data_count->m_data_max.m_int32);
        total_alloc_size    += size;
    }

    Log(4, "%-30s: %d\n", "Total", total_alloc_size);
    Log(4, "%-30s: %d\n", "Alloc Count", stat_alloc_count.m_data_value);
    Log(4, "=====================================================\n");
}

void Engine_Debug_Memory_Check_Leak()
{
    for(int i=0; i<MEMORY_STAT_MAX; i++)
    {
        StatDataCount* stat_data_count = &stat_alloc_size[i];
        Assert(stat_data_count->m_data_value.m_int32 == 0, "");
    }
}


////////////////////////////////////////////////////////////////////////////////
//  +----+---------+-------------------
//  |tptr|size| crc|  Memory Data ...
//  +----+----+----+-------------------
//  |block         |ptr
typedef struct MemoryBlock MemoryBlock;
struct MemoryBlock
{
    strcrc  m_local_name;
    tptr    m_pointer;
    tsize   m_alloc_size;
    byte    m_byte[1];
};

static MemoryBlock* CastToMemoryBlock(tptr ptr)
{
    MemoryBlock* memory_block = (MemoryBlock*)((tchar*)ptr - OFFSET_OF(MemoryBlock, m_byte));
    Assert(memory_block->m_pointer == ptr, "");
    return memory_block;
};

////////////////////////////////////////////////////////////////////////////////
// static bool CallBack_Memory_Profile_FindData(MemoryProfileData* memory_profile_data, crc32 crc)
// {
//     return memory_profile_data->m_crc == crc;
// }

tptr    Memory_Alloc_Plat   (tsize size);
void    Memory_Free_Plat    (void* ptr);
tptr    Memory_Copy_Plat    (tptr dst_ptr, const tptr src_ptr, tsize size);
tptr    Memory_Set_Plat     (tptr address, int32 val, tsize size);


tptr Memory_Alloc(const strcrc* local_name, tsize size)
{
    MemoryBlock * memory_block  = Memory_Alloc_Plat(sizeof(MemoryBlock) + size);
    Assert(memory_block != NULL, "");
    memory_block->m_pointer     = memory_block->m_byte;
    StrCrc_Copy(local_name, &memory_block->m_local_name);
    memory_block->m_alloc_size  = size;

    StatDataCount_Int32_Add(Memory_FindStatData(local_name), memory_block->m_alloc_size);
    StatDataCount_Int32_Add(&stat_alloc_count, 1);

    return memory_block->m_byte;
}

void Memory_Free(tptr ptr)
{
    Assert(ptr != NULL, "");

    MemoryBlock * memory_block = CastToMemoryBlock(ptr);

    StatDataCount_Int32_Add(Memory_FindStatData(&memory_block->m_local_name), -1*memory_block->m_alloc_size);

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
