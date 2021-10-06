#include "CoreMini.h"

#include "Storage.h"
#include "Event.h"
#include "MemoryManager.h"
#include "String.h"
#include "Actor.h"

#include "Data32.h"
#include "Queue.h"


#undef Actor_Component_Del
#undef Actor_Component_Add
#undef Actor_Component_Cast

typedef struct EventAction EventAction;

struct EventAction
{
    Event           m_event;
    ActorActionFunc m_actor_action;
};

struct Actor
{
    uint32                  m_id;
    String*                 m_local_name;
    Storage*                m_storage;
    Queue(EventAction*)*    m_event_action_queue;
    Sence*                  m_sence;
};

Actor* Actor_Create(const tchar* local_name, Sence* sence, uint32 id)
{
    Actor* actor = MemNew(local_name, Actor);
    actor->m_id         = id;
    actor->m_local_name = String_New(local_name, local_name);
    actor->m_storage    = Storage_Create(local_name);
    actor->m_event_action_queue = Queue_Create(local_name, EventAction*);
    actor->m_sence      = sence;
    return actor;
}

void Actor_Destroy(Actor* actor)
{
    Queue_Destroy(actor->m_event_action_queue);
    Storage_Destroy(actor->m_storage);
    String_Del(actor->m_local_name);
    MemDel(actor);
}


void Actor_RegisterEvent(Actor* actor, Event event, ActorActionFunc actor_action_func)
{
    EventAction* event_action   = MemNew(String_CStr(actor->m_local_name), EventAction);
    event_action->m_event           = event;
    event_action->m_actor_action    = actor_action_func;

    Queue_Push(EventAction*, actor->m_event_action_queue, event_action);
}

static bool Actor_FindEvent(EventAction* event_action, Event event)
{
    return event_action->m_event == event;
}

void Actor_UnregisterEvent(Actor* actor, Event event)
{
    EventAction* event_action;
    event_action = Queue_RemoveFrist(EventAction*)(actor->m_event_action_queue, (FindDataFunc)Actor_FindEvent, (tptr)event);
    MemDel(event_action);
}

typedef struct ActorEvent ActorEvent;
struct ActorEvent
{
    Actor*  m_actor;
    Event   m_event;
};

static void Actor_ProcessEachEventAction(EventAction* event_action, ActorEvent* actor_event)
{
    if(event_action->m_event == actor_event->m_event)
    {
        event_action->m_actor_action(actor_event->m_actor);
    }
}

void Actor_ProcessEvent(Actor* actor, Event event)
{
    ActorEvent* actor_event = MemNew(String_CStr(actor->m_local_name), ActorEvent);
    actor_event->m_event    = event;
    actor_event->m_actor    = actor;
    Queue_ForEach(actor->m_event_action_queue, Actor_ProcessEachEventAction, actor_event);
}

void Actor_Component_Add(Actor* actor, const tchar* component_name, Component* component)
{
    Storage_StoreData32(actor->m_storage, Str_CalcCrc(component_name, 0), Data32(tptr, component));
}

void Actor_Component_Del(Actor* actor, const tchar* component_name)
{
    Storage_DeleteVariable(actor->m_storage,Str_CalcCrc(component_name, 0));
}

tptr Actor_Component_Cast(Actor* actor, const tchar* component_name)
{
    tptr ptr = Storage_ReadData32(actor->m_storage, Str_CalcCrc(component_name, 0)).m_pointer;
    return ptr;
}

const tchar* Actor_GetLocalName(Actor* actor)
{
    return String_CStr(actor->m_local_name);
}
