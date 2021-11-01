#pragma once

typedef struct strcrc strcrc;

extern void     Memory_Stat         ();

extern void*    Memory_Alloc        (const strcrc* local_name, tsize size);
extern void     Memory_Free         (void* ptr);
extern void*    Memory_AllocPtrSize (const strcrc* local_name, const void* ptr);
extern void*    Memory_Clone        (const strcrc* local_name, const void* ptr);
extern void*    Memory_SafeClone    (const strcrc* local_name, const void* ptr);

extern void*    Memory_Copy         (void* out_ptr, const void* src_ptr, tsize size);
extern void*    Memory_Set          (void* ptr, int32 val, tsize size);
extern void*    Memory_FullFill     (void* ptr, int32 val);
extern void*    Memory_Zero         (void* ptr);
extern void*    Memory_ZeroSize     (void* ptr, tsize size);
extern tsize    Memory_GetSize      (const void* ptr);

extern bool     Memory_IsInBounds   (const void* head_ptr, const void* check_ptr);


#define MemNew(local_name, type)            (type*)Memory_Alloc(local_name, sizeof(type))
#define MemNewSize(local_name, size)        Memory_Alloc(local_name, size)
#define MemClone(local_name, ptr)           Memory_Clone(local_name, (tptr)ptr)
#define MemSafeClone(local_name, ptr)       Memory_SafeClone(local_name, (tptr)ptr)
#define MemZero(ptr)                        Memory_Zero(ptr)
#define MemZeroSize(ptr, size)              Memory_ZeroSize(ptr, size)
#define MemDel(ptr)                         do{Memory_Free(ptr); ptr = NULL;}while(0)
#define MemSafeDel(ptr)                     do{if(ptr){Memory_Free(ptr); ptr = NULL;}}while(0)
