#pragma once

typedef struct strcrc strcrc;


tptr    Memory_Alloc        (const strcrc* local_name, tsize size);
void    Memory_Free         (tptr ptr);
tptr    Memory_AllocPtrSize (const strcrc* local_name, const tptr ptr);
tptr    Memory_Clone        (const strcrc* local_name, const tptr ptr);
tptr    Memory_SafeClone    (const strcrc* local_name, const tptr ptr);

tptr    Memory_Copy         (tptr dst_ptr, const tptr src_ptr, tsize size);
tptr    Memory_Set          (tptr ptr, int32 val, tsize size);
tptr    Memory_FullFill     (tptr ptr, int32 val);
tptr    Memory_Zero         (tptr ptr);
tsize   Memory_GetSize      (const tptr ptr);

bool    Memory_IsInBounds   (tptr head_ptr, tptr check_ptr);


#define MemNew(local_name, type)            (type*)Memory_Alloc(local_name, sizeof(type))
#define MemNewSize(local_name, size)        Memory_Alloc(local_name, size)
#define MemClone(local_name, ptr)           Memory_Clone(local_name, (tptr)ptr)
#define MemSafeClone(local_name, ptr)       Memory_SafeClone(local_name, (tptr)ptr)
#define MemZero(ptr)                        Memory_Zero(ptr)
#define MemDel(ptr)                         do{Memory_Free(ptr); ptr = NULL;}while(0)
#define MemSafeDel(ptr)                     do{if(ptr){Memory_Free(ptr); ptr = NULL;}}while(0)
