#pragma once

typedef struct MemoryManager MemoryManager;


ptr32    MemoryManager_Alloc         (MemoryManager* memory_manager, const tchar* local_name, tsize size);
void    MemoryManager_Free          (MemoryManager* memory_manager, ptr32 ptr);
ptr32    MemoryManager_AllocPtrSize  (MemoryManager* memory_manager, const tchar* local_name, const ptr32 ptr);
ptr32    MemoryManager_Clone         (MemoryManager* memory_manager, const tchar* local_name, const ptr32 ptr);
ptr32    MemoryManager_SafeClone     (MemoryManager* memory_manager, const tchar* local_name, const ptr32 ptr);

ptr32    Memory_Copy         (ptr32 dst_ptr, const ptr32 src_ptr, tsize size);
ptr32    Memory_Set          (ptr32 ptr, int32 val, tsize size);
ptr32    Memory_FullFill     (ptr32 ptr, int32 val);
ptr32    Memory_Zero         (ptr32 ptr);
tsize   Memory_GetSize      (const ptr32 ptr);


MemoryManager* MemoryManager_GetInstance();


#define MemNew(local_name, type)            (type*)MemoryManager_Alloc(MemoryManager_GetInstance(), local_name, sizeof(type))
#define MemNewSize(local_name, size)        MemoryManager_Alloc(MemoryManager_GetInstance(), local_name, size)
#define MemClone(local_name, ptr)           MemoryManager_Clone(MemoryManager_GetInstance(), local_name, (ptr32)ptr)
#define MemSafeClone(local_name, ptr)       MemoryManager_SafeClone(MemoryManager_GetInstance(), local_name, (ptr32)ptr)
#define MemZero(ptr)                        Memory_Zero(ptr)
#define MemDel(ptr)                         do{MemoryManager_Free(MemoryManager_GetInstance(), ptr); ptr = NULL;}while(0)
#define MemSafeDel(ptr)                     do{if(ptr){MemoryManager_Free(MemoryManager_GetInstance(), ptr); ptr = NULL;}}while(0)
