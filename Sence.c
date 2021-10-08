#include "CoreMini.h"

#include "MemoryManager.h"
#include "Queue.h"
#include "String.h"
#include "Sence.h"
#include "Actor.h"
#include "Event.h"
#include "EventManager.h"


struct Sence
{
    uint32          m_alloc_actor_id;
    String*         m_local_name;
    Queue(Actor*)*  m_actor_queue;
    Queue(Actor*)*  m_actor_event_queue_list[Event_Max];
};

Sence* Sence_Create(const tchar* local_name)
{
    Sence* sence = MemNew(local_name, Sence);
    sence->m_actor_queue    = Queue_Create(local_name, Actor*);
    sence->m_alloc_actor_id = 0;
    sence->m_local_name     = String_New(local_name, local_name);

    for(uint32 i=0; i<Event_Max; i++)
    {
        sence->m_actor_event_queue_list[i] = Queue_Create(local_name, Actor*);
    }

    return sence;
}

void Sence_Destroy(Sence* sence)
{
    Queue_Destroy(sence->m_actor_queue, Actor_Destroy);
    for(uint32 i=0; i<Event_Max; i++)
    {
        Queue_Destroy(sence->m_actor_event_queue_list[i], NULL);
    }
    String_Del(sence->m_local_name);
    MemDel(sence);
}

Actor* Sence_Actor_Create(const tchar* local_name, Sence* sence, CB_ActorCreate cb_actor_create, tptr ptr)
{
    Actor* actor = Actor_Create(local_name, sence, sence->m_alloc_actor_id++, cb_actor_create, ptr);
    Queue_Push(Actor*, local_name, sence->m_actor_queue, actor);
    return actor;
}

void Sence_Actor_Destroy(Sence* sence, CB_FindData cb_find_actor, tptr ptr)
{
    if( cb_find_actor == NULL )
    {
        Queue_RemoveFindFirst(Actor*)(sence->m_actor_queue, NULL, ptr);
        Actor_Destroy(ptr);
    }
    else
    {
        Actor* actor = Queue_RemoveFindFirst(Actor*)(sence->m_actor_queue, cb_find_actor, ptr);
        Actor_Destroy(actor);
    }
}

void CallBack_Actor_ProcessEachActorEvent(Actor* actor, EventInfo* event_struct);

void Sence_Actor_SendEvent(Sence* sence, EventInfo* event_info)
{
    Event event = event_info->m_event;
    Queue_ForEach(sence->m_actor_event_queue_list[event], (CB_ProcessData)CallBack_Actor_ProcessEachActorEvent, event_info);
}

void Sence_Actor_AddEventGroup(Sence* sence, Actor* actor, Event event)
{
    Assert(event > Event_Null && event < Event_Max, "Invalid event!");
    Queue_Push(Actor*, NULL, sence->m_actor_event_queue_list[event], actor);
}

tptr Sence_GetActorQueue(Sence* sence)
{
    return sence->m_actor_queue;
}
