#pragma once

#define Queue struct Queue

typedef bool (*FindDataFunc)(tptr data, tptr ptr);
typedef void (*ProcessDataFunc)(tptr data, tptr ptr);
typedef bool (*CompareDataFunc)(tptr a, tptr b);

Queue*  Queue_Create        (const tchar* local_name, const tchar* type_str);
void    Queue_Destroy       (Queue* queue);

bool    Queue_IsEmpty       (const Queue* queue);
bool    Queue_IsExist       (const Queue* queue, FindDataFunc find_data_func, tptr ptr);
uint32  Queue_GetLength     (const Queue* queue);
void    Queue_ForEach       (const Queue* queue, ProcessDataFunc process_data_func, tptr ptr);

void    Queue_Push          (Queue* queue, tptr reference_data, const tchar* type_str);

tptr    Queue_Pop           (Queue* queue);
tptr    Queue_Dequeue       (Queue* queue);
tptr    Queue_PeekFirst     (const Queue* queue);
tptr    Queue_PeekTail      (const Queue* queue);

tptr    Queue_RemoveFrist   (Queue* queue, FindDataFunc find_data_func, tptr ptr);
int32   Queue_RemoveAll     (Queue* queue, FindDataFunc find_data_func, tptr ptr);

void    Queue_Clear         (Queue* queue);
void    Queue_Sort          (Queue* queue, CompareDataFunc compare_data_func);

#undef Queue
#define Queue(type)                         struct Queue
#define Queue_Create(local_name, type)      Queue_Create(local_name, #type)
#define Queue_Push(type, queue, reference_data) Queue_Push(queue, (tptr)reference_data, MACRO_TOSTR(type))
#define Queue_Pop(type)                     (type)Queue_Pop
#define Queue_Dequeue(type)                 (type)Queue_Dequeue
#define Queue_PeekHead(type)                (type)Queue_PeekFirst
#define Queue_PeekTail(type)                (type)Queue_PeekTail
#define Queue_RemoveFrist(type)             (type)Queue_RemoveFrist

