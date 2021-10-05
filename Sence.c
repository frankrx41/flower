#include "CoreMini.h"

#include "MemoryManager.h"
#include "Queue.h"
#include "String.h"
#include "Sence.h"
#include "Actor.h"


struct Sence
{
    uint32          m_alloc_actor_id;
    String*         m_local_name;
    Queue(Actor*)*  m_actor_queue;
};

Sence* Sence_Create(const tchar* local_name)
{
    Sence* sence = MemNew(local_name, Sence);
    sence->m_actor_queue    = Queue_Create(local_name, Actor*);
    sence->m_alloc_actor_id = 0;
    sence->m_local_name     = String_New(local_name, local_name);
    return sence;
}

void Sence_Destroy(Sence* sence)
{
    Queue_Destroy(sence->m_actor_queue);
    String_Del(sence->m_local_name);
    MemDel(sence);
}

Actor* Sence_Actor_Create(const tchar* local_name, Sence* sence)
{
    Actor* actor = Actor_Create(local_name, sence, sence->m_alloc_actor_id++);
    Queue_Push(Actor*, sence->m_actor_queue, actor);
    return actor;
}

void Sence_Actor_Destroy(Sence* sence, FindDataFunc find_actor_func, tptr ptr)
{
    Actor* actor = Queue_RemoveFrist(Actor*)(sence->m_actor_queue, find_actor_func, ptr);
    Actor_Destroy(actor);
}

static void Sence_Actor_ProcessEachActorEvent(Actor* actor, Event event)
{
    Actor_ProcessEvent(actor, event);
}

void Sence_Actor_SendEvent(Sence* sence, Event event)
{
    Queue_ForEach(sence->m_actor_queue, (ProcessDataFunc)Sence_Actor_ProcessEachActorEvent, (tptr)event);
}

tptr Sence_GetActorQueue(Sence* sence)
{
    return sence->m_actor_queue;
}
