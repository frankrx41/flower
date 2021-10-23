#include "CoreMini.h"

#include "MemoryManager.h"

#include "String.h"
#include "tData.h"
#include "Stat.h"


#define MEMORY_STAT_MAX     ( 50 )
static statcount stat_alloc_size[MEMORY_STAT_MAX];
static statcount stat_alloc_count;

static statcount* Memory_FindStatData(const strcrc* local_name)
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
void Memory_Stat()
{
    tsize   total_alloc_size = 0;
    uint32  total_alloc_count = 0;

    Log(4, "Memory Profile:\n");
    Log(4, "=====================================================\n");

    for(int i=0; i<MEMORY_STAT_MAX; i++)
    {
        statcount* stat_data_count = &stat_alloc_size[i];
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

void Memory_Check_Leak()
{
    for(int i=0; i<MEMORY_STAT_MAX; i++)
    {
        statcount* stat_data_count = &stat_alloc_size[i];
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
    void*   m_pointer;
    tsize   m_alloc_size;
    byte    m_byte[1];
};

static MemoryBlock* CastToMemoryBlock(const void* ptr)
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

void*   Memory_Alloc_Platform   (tsize size);
void    Memory_Free_Platform    (void* ptr);
void*   Memory_Copy_Platform    (void* dst_ptr, const void* src_ptr, tsize size);
void*   Memory_Set_Platform     (void* address, int32 val, tsize size);


void* Memory_Alloc(const strcrc* local_name, tsize size)
{
    MemoryBlock * memory_block  = Memory_Alloc_Platform(sizeof(MemoryBlock) + size);
    Assert(memory_block != NULL, "");
    memory_block->m_pointer     = memory_block->m_byte;
    if( local_name != NULL )
    {
        StrCrc_Copy(local_name, &memory_block->m_local_name);
    }
    else
    {
        memory_block->m_local_name = StrCrc("Null", 0);
    }
    memory_block->m_alloc_size  = size;

    StatCount_Int32_Inc(Memory_FindStatData(&memory_block->m_local_name), memory_block->m_alloc_size);
    StatCount_Int32_Inc(&stat_alloc_count, 1);

    return memory_block->m_byte;
}

void Memory_Free(void* ptr)
{
    Assert(ptr != NULL, "");

    MemoryBlock * memory_block = CastToMemoryBlock(ptr);

    StatCount_Int32_Inc(Memory_FindStatData(&memory_block->m_local_name), -1*memory_block->m_alloc_size);

    Memory_Free_Platform(memory_block);
}

void* Memory_AllocPtrSize(const strcrc* local_name, const void* ptr)
{
    Assert(ptr != NULL, "");

    MemoryBlock* src_block = CastToMemoryBlock(ptr);

    void* new_ptr = Memory_Alloc(local_name, src_block->m_alloc_size);
    return new_ptr;
}

void* Memory_Clone(const strcrc* local_name, const void* ptr)
{
    Assert(ptr != NULL, "");
    MemoryBlock* src_block = CastToMemoryBlock(ptr);

    void* new_ptr = Memory_AllocPtrSize(local_name, ptr);
    Memory_Copy(new_ptr, ptr, src_block->m_alloc_size);

    return new_ptr;
}

void* Memory_SafeClone(const strcrc* local_name, const void* ptr)
{
    if( !ptr )
    {
        return NULL;
    }

    return
    Memory_Clone(local_name, ptr);
}

void* Memory_Copy(void* out_ptr, const void* src_ptr, tsize size)
{
    Assert(out_ptr != NULL, "");
    Assert(src_ptr != NULL, "");

    MemoryBlock* dst_block = CastToMemoryBlock(out_ptr);
    MemoryBlock* src_block = CastToMemoryBlock(src_ptr);

    Assert( dst_block->m_alloc_size >= size, "");
    Assert( src_block->m_alloc_size >= size, "");

    return
    Memory_Copy_Platform(out_ptr, src_ptr, size);
}

void* Memory_Set(void* ptr, int32 val, tsize size)
{
    Assert(ptr != NULL, "");

    MemoryBlock* memory_block = CastToMemoryBlock(ptr);
    Assert( memory_block->m_alloc_size >= size, "");

    return
    Memory_Set_Platform(ptr, val, size);
}

void* Memory_FullFill(void* ptr, int32 val)
{
    Assert(ptr != NULL, "");

    MemoryBlock* memory_block = CastToMemoryBlock(ptr);

    return
    Memory_Set_Platform(ptr, val, memory_block->m_alloc_size);
}

void* Memory_Zero(void* ptr)
{
    Assert(ptr != NULL, "");

    MemoryBlock* memory_block = CastToMemoryBlock(ptr);

    return
    Memory_Set_Platform(ptr, 0, memory_block->m_alloc_size);
}

void* Memory_ZeroSize(void* ptr, tsize size)
{
    MemoryBlock* memory_block = CastToMemoryBlock(ptr);
    if( memory_block )
    {
        Assert(size < memory_block->m_alloc_size, "");
    }

    return
    Memory_Set_Platform(ptr, 0, size);
}

tsize Memory_GetSize(const void* ptr)
{
    Assert(ptr != NULL, "");

    MemoryBlock* memory_block = CastToMemoryBlock(ptr);
    return memory_block->m_alloc_size;
}

bool Memory_IsInBounds(void* head_ptr, void* check_ptr)
{
    MemoryBlock* memory_block = CastToMemoryBlock(head_ptr);
    const tsize size_offset = tSize(check_ptr) - tSize(head_ptr);
    return size_offset < memory_block->m_alloc_size;
}
