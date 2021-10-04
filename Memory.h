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

tptr    Engine_Memory_Alloc         (const tchar* local_namespace, tsize size);
error   Engine_Memory_Free          (tptr ptr);
tptr    Engine_Memory_AllocPtrSize  (const tchar* local_namespace, const tptr ptr);
tptr    Engine_Memory_Clone         (const tchar* local_namespace, const tptr ptr);
tptr    Engine_Memory_SafeClone     (const tchar* local_namespace, const tptr ptr);

tptr    Engine_Memory_Copy          (tptr dst_ptr, const tptr src_ptr, tsize size);
tptr    Engine_Memory_Set           (tptr ptr, int32 val, tsize size);
tptr    Engine_Memory_FullFill      (tptr ptr, int32 val);
tptr    Engine_Memory_Zero          (tptr ptr);

tsize   Engine_Memory_GetSize       (const tptr ptr);

Memory* Engine_Memory_GetInstance   ();

#define MemNew(local_namespace, type)       (type*)Engine_Memory_Alloc(local_namespace, sizeof(type))
#define MemNewSize(local_namespace, size)   Engine_Memory_Alloc(local_namespace, size)
#define MemClone(local_namespace, ptr)      Engine_Memory_Clone(local_namespace, (tptr)ptr)
#define MemDel(ptr)                         do{Engine_Memory_Free(ptr); ptr = NULL;}while(0)
#define MemSafeDel(ptr)                     do{if(ptr){Engine_Memory_Free(ptr); ptr = NULL;}}while(0)