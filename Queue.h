#pragma once

#define Queue struct Queue

typedef bool (*FindDataFunc)(tptr data);
typedef void (*ProcessDataFunc)(tptr data);
typedef bool (*CompareDataFunc)(tptr a, tptr b);

Queue*  Queue_Create        (const tchar* local_namespace, const tchar* type_str);
void    Queue_Destroy       (Queue* queue);

bool    Queue_IsEmpty       (const Queue* queue);
bool    Queue_IsExist       (const Queue* queue, FindDataFunc find_data_func);
uint32  Queue_GetLength     (const Queue* queue);
void    Queue_ForEach       (const Queue* queue, ProcessDataFunc process_data_func);

void    Queue_Push          (const tchar* local_namespace, Queue* queue, tptr reference_data, const tchar* type_str);

tptr    Queue_Pop           (Queue* queue);
tptr    Queue_Dequeue       (Queue* queue);
tptr    Queue_PeekFirst     (const Queue* queue);
tptr    Queue_PeekTail      (const Queue* queue);

tptr    Queue_RemoveFrist   (Queue* queue, FindDataFunc find_data_func);
int32   Queue_RemoveAll     (Queue* queue, FindDataFunc find_data_func);

void    Queue_Clear         (Queue* queue);
void    Queue_Sort          (Queue* queue, CompareDataFunc compare_data_func);

#undef Queue
#define Queue(type)                         struct Queue
#define Queue_Create(local_namespace, type) Queue_Create(local_namespace, #type)
#define _Queue_Push(a,b,c,d)                Queue_Push(a, b, c, MACRO_TOSTR(d))
#define Queue_Push(type, local_namespace, queue, reference_data)           _Queue_Push(local_namespace, queue, reference_data, MACRO_REMOVE_BRACKETS(type))
#define Queue_Pop(type)                     (type)Queue_Pop
#define Queue_Dequeue(type)                 (type)Queue_Dequeue
#define Queue_PeekHead(type)                (type)Queue_PeekFirst
#define Queue_PeekTail(type)                (type)Queue_PeekTail
#define Queue_RemoveFrist(type)             (type)Queue_RemoveFrist

