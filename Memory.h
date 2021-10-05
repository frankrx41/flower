#pragma once

typedef struct Memory Memory;

#if PUBLIC_MEMORY
struct Memory
{
    uint32  m_alloc_id;
};
#endif

// Memory
void    Engine_Memory_Initialize    ();

tptr    Engine_Memory_Alloc         (const tchar* local_name, tsize size);
void    Engine_Memory_Free          (tptr ptr);
tptr    Engine_Memory_AllocPtrSize  (const tchar* local_name, const tptr ptr);
tptr    Engine_Memory_Clone         (const tchar* local_name, const tptr ptr);
tptr    Engine_Memory_SafeClone     (const tchar* local_name, const tptr ptr);

tptr    Engine_Memory_Copy          (tptr dst_ptr, const tptr src_ptr, tsize size);
tptr    Engine_Memory_Set           (tptr ptr, int32 val, tsize size);
tptr    Engine_Memory_FullFill      (tptr ptr, int32 val);
tptr    Engine_Memory_Zero          (tptr ptr);

tsize   Engine_Memory_GetSize       (const tptr ptr);

Memory* Engine_Memory_GetInstance   ();

void    Engine_Profile_Memory       ();


#define MemNew(local_name, type)            (type*)Engine_Memory_Alloc(local_name, sizeof(type))
#define MemNewSize(local_name, size)        Engine_Memory_Alloc(local_name, size)
#define MemClone(local_name, ptr)           Engine_Memory_Clone(local_name, (tptr)ptr)
#define MemSafeClone(local_name, ptr)       Engine_Memory_SafeClone(local_name, (tptr)ptr)
#define MemDel(ptr)                         do{Engine_Memory_Free(ptr); ptr = NULL;}while(0)
#define MemSafeDel(ptr)                     do{if(ptr){Engine_Memory_Free(ptr); ptr = NULL;}}while(0)