#include "CoreMini.h"

#include "MemoryManager.h"

#include "Queue.h"
#include "String.h"


typedef struct Queue Queue;
typedef struct Node Node;

struct Node
{
    Node*   m_node_prev;
    Node*   m_node_next;
    tptr    m_reference_data;
};

struct Queue
{
    String* m_local_name;
    crc32   m_type_crc32;
    uint32  m_length;
    Node*   m_head;     // head node reference data pointer to queue
};

#undef Queue
#undef Queue_Create
#undef Queue_Find
#undef Queue_Push
#undef Queue_Pop
#undef Queue_Dequeue
#undef Queue_PeekHead
#undef Queue_PeekTail
#undef Queue_RemoveFindFirst


static bool Queue_IsHead(const Queue* queue, Node* node)
{
    return node->m_reference_data == queue;
}

Queue*  Queue_Create(const tchar* local_name, const tchar* type_str)
{
    Queue* queue    = MemNew(local_name, Queue);
    queue->m_type_crc32  = Str_CalcCrc(type_str, 0);
    queue->m_length                 = 0;
    queue->m_head                   = MemNew(local_name, Node);
    queue->m_head->m_node_next      = queue->m_head;
    queue->m_head->m_node_prev      = queue->m_head;
    queue->m_head->m_reference_data = queue;
    queue->m_local_name             = String_New(local_name, local_name, true);
    return queue;
}

void Queue_Destroy(Queue* queue, CB_DestroyData_Void_tPtr cb_destroy_data_void_tptr)
{
    Queue_Clear(queue, cb_destroy_data_void_tptr);
    String_Del(queue->m_local_name);
    MemDel(queue->m_head);
    MemDel(queue);
}

bool Queue_IsEmpty(const Queue* queue)
{
    return Queue_GetLength(queue) == 0;
}

void Queue_ForEach(const Queue* queue, CB_ProcessData_Void_tPtr_tPtr cb_process_data_void_tptr_tptr, tptr ptr)
{
    Node* node = queue->m_head->m_node_next;
    for(; !Queue_IsHead(queue, node); node = node->m_node_next)
    {
        cb_process_data_void_tptr_tptr(node->m_reference_data, ptr);
    }
}

tptr Queue_Find(const Queue* queue, CB_FindData_Bool_tPtr_tPtr cb_find_data_bool_tptr_tptr, tptr ptr)
{
    Assert(queue != NULL, "");
    Node* node = queue->m_head->m_node_next;
    for(; !Queue_IsHead(queue, node); node = node->m_node_next)
    {
        if( cb_find_data_bool_tptr_tptr(node->m_reference_data, ptr) )
        {
            return node->m_reference_data;
        }
    }
    return NULL;
}
uint32 Queue_GetLength(const Queue* queue)
{
    return queue->m_length;
}

void Queue_Push(const tchar* local_name, Queue* queue, tptr reference_data, const tchar* type_str)
{
    Assert(Str_CalcCrc(type_str, 0) == queue->m_type_crc32, "Push type is different from create type, do you forget the star(*) ?");

    if( local_name == NULL )
    {
        local_name = String_CStr(queue->m_local_name);
    }
    Node* new_node = MemNew(local_name, Node);
    new_node->m_reference_data = reference_data;

    Node* last_node = queue->m_head->m_node_prev;
    Node* head_node = last_node->m_node_next;

    Assert(Queue_IsHead(queue, head_node), "");
    new_node->m_node_next   = last_node->m_node_next;
    new_node->m_node_prev   = last_node;
    last_node->m_node_next  = new_node;
    head_node->m_node_prev  = new_node;

    queue->m_length++;
}

tptr Queue_Pop(Queue* queue)
{
    Assert(!Queue_IsEmpty(queue), "");
    if( Queue_IsEmpty(queue) )
    {
        return NULL;
    }

    Node* last_node = queue->m_head->m_node_prev;
    tptr reference_data = last_node->m_reference_data;

    Node* last_node_prev = last_node->m_node_prev;
    Node* last_node_next = last_node->m_node_next;
    last_node->m_node_prev->m_node_next = last_node_prev;
    last_node->m_node_next->m_node_prev = last_node_next;
    
    queue->m_length--;
    return reference_data;
}

tptr Queue_Dequeue(Queue* queue)
{
    Assert(!Queue_IsEmpty(queue), "");
    if( Queue_IsEmpty(queue) )
    {
        return NULL;
    }

    Node* first_node = queue->m_head->m_node_next;
    tptr reference_data = first_node->m_reference_data;

    Node* first_node_prev = first_node->m_node_prev;
    Node* first_node_next = first_node->m_node_next;
    first_node->m_node_prev->m_node_next = first_node_prev;
    first_node->m_node_next->m_node_prev = first_node_next;

    queue->m_length--;
    return reference_data;
}

tptr Queue_PeekFirst(const Queue* queue)
{
    Assert(!Queue_IsEmpty(queue), "");
    if( Queue_IsEmpty(queue) )
    {
        return NULL;
    }

    return queue->m_head->m_node_next->m_reference_data;
}

tptr Queue_PeekTail(const Queue* queue)
{
    Assert(!Queue_IsEmpty(queue), "");
    if( Queue_IsEmpty(queue) )
    {
        return NULL;
    }

    return queue->m_head->m_node_prev->m_reference_data;
}

static tptr Queue_RemoveNode(Queue* queue, Node* node)
{
    Assert(!Queue_IsHead(queue, node), "");

    Node* next_node = node->m_node_next;
    Node* prev_node = node->m_node_prev;

    next_node->m_node_prev = prev_node;
    prev_node->m_node_next = next_node;

    tptr reference_data = node->m_reference_data;
    MemDel(node);

    queue->m_length --;

    return reference_data;
}

static tptr Queue_RemoveByPointer(struct Queue* queue, tptr ptr)
{
    Assert(ptr != NULL, "");

    Node* node = queue->m_head->m_node_next;
    for(; !Queue_IsHead(queue, node); node = node->m_node_next)
    {
        if( node->m_reference_data == ptr)
        {
            return
            Queue_RemoveNode(queue, node);
        }
    }
    return NULL;
}

tptr Queue_RemoveFindFirst(Queue* queue, CB_FindData_Bool_tPtr_tPtr cb_find_data_bool_tptr_tptr, tptr ptr)
{
    if( cb_find_data_bool_tptr_tptr == NULL )
    {
        return
        Queue_RemoveByPointer(queue, ptr);
    }
    else
    {
        Node* node = queue->m_head->m_node_next;
        for(; !Queue_IsHead(queue, node); node = node->m_node_next)
        {
            if( cb_find_data_bool_tptr_tptr(node->m_reference_data, ptr) )
            {
                return
                Queue_RemoveNode(queue, node);
            }
        }
    }
    return NULL;
}

int32 Queue_RemoveFindAll(Queue* queue, CB_FindData_Bool_tPtr_tPtr cb_find_data_bool_tptr_tptr, tptr ptr)
{
    Assert(cb_find_data_bool_tptr_tptr != NULL, "");
    int32 remove_cnt = 0;
    Node* node = queue->m_head->m_node_next;
    Node* node_next;
    for(; !Queue_IsHead(queue, node); node = node_next)
    {
        node_next = node->m_node_next;
        if( cb_find_data_bool_tptr_tptr(node->m_reference_data, ptr) )
        {
            Queue_RemoveNode(queue, node);
            remove_cnt++;
        }
    }
    return remove_cnt;
}


void Queue_Clear(Queue* queue, CB_DestroyData_Void_tPtr cb_destroy_data_void_tptr)
{
    Node* node = queue->m_head->m_node_next;
    Node* node_next;
    for(; !Queue_IsHead(queue, node); node = node_next)
    {
        node_next = node->m_node_next;
        if( cb_destroy_data_void_tptr )
        {
            cb_destroy_data_void_tptr(node->m_reference_data);
        }
        Queue_RemoveNode(queue, node);
    }
    Assert(Queue_GetLength(queue) == 0, "");
}

void Queue_Sort(Queue* queue, CB_CompareData_Bool_tPtr_tPtr cb_compare_data_bool_tptr_tptr)
{
    // TODO:
    Assert(false, "");
}

static const tchar* Queue_GetLocalName(struct Queue* queue)
{
    return String_CStr(queue->m_local_name);
}
