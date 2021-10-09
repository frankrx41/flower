#pragma once

#define Queue struct Queue

typedef bool (*CB_FindData_Bool_tPtr_tPtr)      (tptr data, const tptr ptr);
typedef void (*CB_ProcessData_Void_tPtr_tPtr)   (tptr data, const tptr ptr);
typedef bool (*CB_CompareData_Bool_tPtr_tPtr)   (const tptr dat1, const tptr dat2);
typedef void (*CB_DestroyData_Void_tPtr)        (tptr data);

Queue*  Queue_Create        (const tchar* local_name, const tchar* type_str);
void    Queue_Destroy       (Queue* queue, CB_DestroyData_Void_tPtr cb_destroy_data);

bool    Queue_IsEmpty       (const Queue* queue);
tptr    Queue_Find          (const Queue* queue, CB_FindData_Bool_tPtr_tPtr cb_find_data, tptr ptr);
uint32  Queue_GetLength     (const Queue* queue);
void    Queue_ForEach       (const Queue* queue, CB_ProcessData_Void_tPtr_tPtr cb_process_data, tptr ptr);

void    Queue_Push          (const tchar* local_name, Queue* queue, tptr reference_data, const tchar* type_str);

tptr    Queue_Pop           (Queue* queue);
tptr    Queue_Dequeue       (Queue* queue);
tptr    Queue_PeekFirst     (const Queue* queue);
tptr    Queue_PeekTail      (const Queue* queue);

tptr    Queue_RemoveFindFirst   (Queue* queue, CB_FindData_Bool_tPtr_tPtr cb_find_data, tptr ptr);     // If cb_find_data is NULL, it will find the ptr
int32   Queue_RemoveFindAll     (Queue* queue, CB_FindData_Bool_tPtr_tPtr cb_find_data, tptr ptr);

void    Queue_Clear         (Queue* queue, CB_DestroyData_Void_tPtr cb_destroy_data);
void    Queue_Sort          (Queue* queue, CB_CompareData_Bool_tPtr_tPtr cb_compare_data);

#undef Queue
#define Queue(type)                             struct Queue
#define Queue_Create(local_name, type)          Queue_Create(local_name, #type)
#define Queue_Push(type, local_name, queue, reference_data) Queue_Push(local_name, queue, (tptr)reference_data, MACRO_TOSTR(type))
#define Queue_Find(type)                        (type)Queue_Find
#define Queue_Pop(type)                         (type)Queue_Pop
#define Queue_Dequeue(type)                     (type)Queue_Dequeue
#define Queue_PeekHead(type)                    (type)Queue_PeekFirst
#define Queue_PeekTail(type)                    (type)Queue_PeekTail
#define Queue_RemoveFindFirst(type)             (type)Queue_RemoveFindFirst

