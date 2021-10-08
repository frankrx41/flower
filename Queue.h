#pragma once

#define Queue struct Queue

typedef bool (*CB_FindData)     (ptr32 data, ptr32 ptr);
typedef void (*CB_ProcessData)  (ptr32 data, ptr32 ptr);
typedef bool (*CB_CompareData)  (ptr32 dat1, ptr32 dat2);
typedef void (*CB_DestroyData)  (ptr32 data);

Queue*  Queue_Create        (const tchar* local_name, const tchar* type_str);
void    Queue_Destroy       (Queue* queue, CB_DestroyData cb_destroy_data);

bool    Queue_IsEmpty       (const Queue* queue);
ptr32    Queue_Find          (const Queue* queue, CB_FindData cb_find_data, ptr32 ptr);
uint32  Queue_GetLength     (const Queue* queue);
void    Queue_ForEach       (const Queue* queue, CB_ProcessData cb_process_data, ptr32 ptr);

void    Queue_Push          (const tchar* local_name, Queue* queue, ptr32 reference_data, const tchar* type_str);

ptr32    Queue_Pop           (Queue* queue);
ptr32    Queue_Dequeue       (Queue* queue);
ptr32    Queue_PeekFirst     (const Queue* queue);
ptr32    Queue_PeekTail      (const Queue* queue);

ptr32    Queue_RemoveFindFirst   (Queue* queue, CB_FindData cb_find_data, ptr32 ptr);     // If cb_find_data is NULL, it will find the ptr
int32   Queue_RemoveFindAll     (Queue* queue, CB_FindData cb_find_data, ptr32 ptr);

void    Queue_Clear         (Queue* queue, CB_DestroyData cb_destroy_data);
void    Queue_Sort          (Queue* queue, CB_CompareData cb_compare_data);

#undef Queue
#define Queue(type)                             struct Queue
#define Queue_Create(local_name, type)          Queue_Create(local_name, #type)
#define Queue_Push(type, local_name, queue, reference_data) Queue_Push(local_name, queue, (ptr32)reference_data, MACRO_TOSTR(type))
#define Queue_Find(type)                        (type)Queue_Find
#define Queue_Pop(type)                         (type)Queue_Pop
#define Queue_Dequeue(type)                     (type)Queue_Dequeue
#define Queue_PeekHead(type)                    (type)Queue_PeekFirst
#define Queue_PeekTail(type)                    (type)Queue_PeekTail
#define Queue_RemoveFindFirst(type)             (type)Queue_RemoveFindFirst

