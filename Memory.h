#pragma once

typedef struct Memory Memory;

#if PUBLIC_MEMORY
struct Memory
{
    uint32  m_alloc_id;
};
#endif

// Memory
error   Engine_Memory_Initialize    ();

tptr    Engine_Memory_Alloc         (const tchar* debug_name, tsize size);
error   Engine_Memory_Free          (const tchar* debug_name, tptr ptr);
tptr    Engine_Memory_CloneInfo     (const tchar* debug_name, const tptr ptr);
tptr    Engine_Memory_Clone         (const tchar* debug_name, const tptr ptr);
tptr    Engine_Memory_SafeClone     (const tchar* debug_name, const tptr ptr);

tptr    Engine_Memory_Copy          (tptr dst_ptr, const tptr src_ptr, tsize size);
tptr    Engine_Memory_Set           (tptr ptr, int32 val, tsize size);
tptr    Engine_Memory_FullFill      (tptr ptr, int32 val);
tptr    Engine_Memory_Zero          (tptr ptr);

tsize   Engine_Memory_GetSize       (const tptr ptr);

Memory* Engine_Memory_GetInstance   ();

#define MemNew(debug_name, type)    (type*)Engine_Memory_Alloc(debug_name, sizeof(type))
#define MemNewSize(debug_name, size) Engine_Memory_Alloc(debug_name, size)
#define MemClone(debug_name, ptr)   Engine_Memory_Clone(debug_name, (tptr)ptr)
#define MemDel(debug_name, ptr)     do{Engine_Memory_Free(debug_name, ptr); ptr = NULL;}while(0)