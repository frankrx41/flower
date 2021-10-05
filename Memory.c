#include "CoreMini.h"

#define PUBLIC_MEMORY 1
#include "Memory.h"
#include "String.h"

tptr    Engine_Memory_Alloc_Plat    (tsize size);
void    Engine_Memory_Free_Plat     (tptr ptr);
tptr    Engine_Memory_Copy_Plat     (tptr dst_ptr, const tptr src_ptr, tsize size);
tptr    Engine_Memory_Set_Plat      (tptr address, int32 val, tsize size);

typedef struct MemoryBlock MemoryBlock;

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

static uint32 Memory_IncreaseAllocID()
{
    return Engine_Memory_GetInstance()->m_alloc_id++;
}

static MemoryBlock* CastToMemoryBlock(tptr ptr)
{
    MemoryBlock* memory_block;
    memory_block = (MemoryBlock*)((tchar*)ptr - offsetof(MemoryBlock, m_byte));
    Assert(memory_block->m_flag == 'GPYM', "");
    return memory_block;
};

////////////////////////////////////////////////////////////////////////////////
void Engine_Memory_Initialize()
{
    Memory* memory = Engine_Memory_GetInstance();
    memory->m_alloc_id = 0;
}

tptr Engine_Memory_Alloc(const tchar* local_name, tsize size)
{
    MemoryBlock * memory_block  = Engine_Memory_Alloc_Plat(sizeof(MemoryBlock) + size);
    Assert(memory_block != NULL, "");
    memory_block->m_flag        = 'GPYM';
    memory_block->m_id          = Memory_IncreaseAllocID();
    memory_block->m_crc         = Str_CalcCrc(local_name, 0);
    memory_block->m_alloc_size  = size;

    return memory_block->m_byte;
}

void Engine_Memory_Free(tptr ptr)
{
    Assert(ptr != NULL, "");

    MemoryBlock * dst_block;
    dst_block = CastToMemoryBlock(ptr);

    Engine_Memory_Free_Plat(dst_block);
}

tptr Engine_Memory_AllocPtrSize(const tchar* local_name, const tptr ptr)
{
    Assert(ptr != NULL, "");

    MemoryBlock * src_block;
    src_block = CastToMemoryBlock(ptr);

    tptr new_ptr;
    new_ptr = Engine_Memory_Alloc(local_name, src_block->m_alloc_size);
    return new_ptr;
}

tptr Engine_Memory_Clone(const tchar* local_name, const tptr ptr)
{
    Assert(ptr != NULL, "");
    MemoryBlock * src_block;
    src_block = CastToMemoryBlock(ptr);

    tptr new_ptr;
    new_ptr = Engine_Memory_AllocPtrSize(local_name, ptr);
    Engine_Memory_Copy(new_ptr, ptr, src_block->m_alloc_size);

    return new_ptr;
}

tptr Engine_Memory_SafeClone(const tchar* local_name, const tptr ptr)
{
    if( !ptr )
    {
        return NULL;
    }

    return
    Engine_Memory_Clone(local_name, ptr);
}

tptr Engine_Memory_Copy(tptr dst_ptr, const tptr src_ptr, tsize size)
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
    Engine_Memory_Copy_Plat(dst_ptr, src_ptr, size);
}

tptr Engine_Memory_Set(tptr ptr, int32 val, tsize size)
{
    Assert(ptr != NULL, "");

    MemoryBlock * dst_block;
    dst_block = CastToMemoryBlock(ptr);
    Assert( dst_block->m_alloc_size >= size, "");

    return
    Engine_Memory_Set_Plat(ptr, val, size);
}

tptr Engine_Memory_FullFill(tptr ptr, int32 val)
{
    Assert(ptr != NULL, "");

    MemoryBlock * dst_block;
    dst_block = CastToMemoryBlock(ptr);

    return
    Engine_Memory_Set_Plat(ptr, val, dst_block->m_alloc_size);
}

tptr Engine_Memory_Zero(tptr ptr)
{
    Assert(ptr != NULL, "");

    MemoryBlock * dst_block;
    dst_block = CastToMemoryBlock(ptr);

    return
    Engine_Memory_Set_Plat(ptr, 0, dst_block->m_alloc_size);
}

tsize Engine_Memory_GetSize(const tptr ptr)
{
    Assert(ptr != NULL, "");

    MemoryBlock * memory_block;
    memory_block = CastToMemoryBlock(ptr);
    return memory_block->m_alloc_size;
}

////////////////////////////////////////////////////////////////////////////////
void Engine_Profile_Memory()
{
}
