#pragma once

typedef struct MemoryManager MemoryManager;


tptr    MemoryManager_Alloc         (MemoryManager* memory_manager, const tchar* local_name, tsize size);
void    MemoryManager_Free          (MemoryManager* memory_manager, tptr ptr);
tptr    MemoryManager_AllocPtrSize  (MemoryManager* memory_manager, const tchar* local_name, const tptr ptr);
tptr    MemoryManager_Clone         (MemoryManager* memory_manager, const tchar* local_name, const tptr ptr);
tptr    MemoryManager_SafeClone     (MemoryManager* memory_manager, const tchar* local_name, const tptr ptr);

tptr    Memory_Copy         (tptr dst_ptr, const tptr src_ptr, tsize size);
tptr    Memory_Set          (tptr ptr, int32 val, tsize size);
tptr    Memory_FullFill     (tptr ptr, int32 val);
tptr    Memory_Zero         (tptr ptr);
tsize   Memory_GetSize      (const tptr ptr);


MemoryManager* MemoryManager_GetInstance();


#define MemNew(local_name, type)            (type*)MemoryManager_Alloc(MemoryManager_GetInstance(), local_name, sizeof(type))
#define MemNewSize(local_name, size)        MemoryManager_Alloc(MemoryManager_GetInstance(), local_name, size)
#define MemClone(local_name, ptr)           MemoryManager_Clone(MemoryManager_GetInstance(), local_name, (tptr)ptr)
#define MemSafeClone(local_name, ptr)       MemoryManager_SafeClone(MemoryManager_GetInstance(), local_name, (tptr)ptr)
#define MemDel(ptr)                         do{MemoryManager_Free(MemoryManager_GetInstance(), ptr); ptr = NULL;}while(0)
#define MemSafeDel(ptr)                     do{if(ptr){MemoryManager_Free(MemoryManager_GetInstance(), ptr); ptr = NULL;}}while(0)