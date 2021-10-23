#pragma once

#define Queue struct Queue
typedef struct strcrc strcrc;

typedef bool (*CB_FindData_Bool_tPtr_tPtr)      (const void* data, const void* ptr);
typedef void (*CB_ProcessData_Void_tPtr_tPtr)   (void* data, const void* ptr);
typedef bool (*CB_CompareData_Bool_tPtr_tPtr)   (const void* dat1, const void* dat2);
typedef void (*CB_DestroyData_Void_tPtr)        (void* data);

Queue*  Queue_Create        (const strcrc* local_name, const tchar* type_str);
void    Queue_Destroy       (Queue* queue, CB_DestroyData_Void_tPtr cb_destroy_data_void_tptr);

bool    Queue_IsEmpty       (const Queue* queue);
void*   Queue_Find          (const Queue* queue, CB_FindData_Bool_tPtr_tPtr cb_find_data_bool_tptr_tptr, const void* ptr);
uint32  Queue_GetLength     (const Queue* queue);
void    Queue_ForEach       (const Queue* queue, CB_ProcessData_Void_tPtr_tPtr cb_process_data_void_tptr_tptr, const void* ptr);

void    Queue_Push          (const strcrc* local_name, Queue* queue, void* reference_data, const tchar* type_str);

void*   Queue_Pop           (Queue* queue);
void*   Queue_Dequeue       (Queue* queue);
void*   Queue_PeekFirst     (const Queue* queue);
void*   Queue_PeekTail      (const Queue* queue);

void*   Queue_RemoveFindFirst   (Queue* queue, CB_FindData_Bool_tPtr_tPtr cb_find_data_bool_tptr_tptr, const void* ptr, CB_DestroyData_Void_tPtr cb_destroy_data_void_tptr);     // If cb_find_data_bool_tptr_tptr is NULL, it will find the ptr
int32   Queue_RemoveFindAll     (Queue* queue, CB_FindData_Bool_tPtr_tPtr cb_find_data_bool_tptr_tptr, const void* ptr, CB_DestroyData_Void_tPtr cb_destroy_data_void_tptr);

void    Queue_Clear         (Queue* queue, CB_DestroyData_Void_tPtr cb_destroy_data_void_tptr);
void    Queue_Sort          (Queue* queue, CB_CompareData_Bool_tPtr_tPtr cb_compare_data_bool_tptr_tptr);

#undef Queue
#define Queue(type)                             struct Queue
#define Queue_Create(local_name, type)          (((type*)0), Queue_Create(local_name, #type))
#define Queue_Push(type, local_name, queue, reference_data) (((type*)0), Queue_Push(local_name, queue, (void*)reference_data, MACRO_TO_STR(type)))
#define Queue_Find(type)                        (type)Queue_Find
#define Queue_Pop(type)                         (type)Queue_Pop
#define Queue_Dequeue(type)                     (type)Queue_Dequeue
#define Queue_PeekHead(type)                    (type)Queue_PeekFirst
#define Queue_PeekTail(type)                    (type)Queue_PeekTail
#define Queue_RemoveFindFirst(type)             (type)Queue_RemoveFindFirst

