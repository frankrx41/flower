#include "CoreMini.h"

#include "Event.h"

#include "MemoryManager.h"
#include "ActionComponent.h"

#include "Queue.h"
#include "String.h"

typedef struct EventRespond EventRespond;
typedef struct ActorEvent ActorEvent;

struct EventRespond
{
    Event           m_event;
    CB_EventRespond_Void_Actor_EventInfo m_cb_actor_respond;
};

struct ActionComponent
{
    String*                 m_local_name;
    Queue(EventRespond*)*   m_event_respond_queue;
};


ActionComponent* Component_Action_Create(const tchar* local_name)
{
    ActionComponent* event_respond_component  = MemNew(local_name, ActionComponent);
    event_respond_component->m_event_respond_queue  = Queue_Create(local_name, EventRespond*);
    event_respond_component->m_local_name           = String_New(local_name, local_name);
    return event_respond_component;
}

static void CallBack_EventRespond_Destroy(EventRespond* event_respond)
{
    MemDel(event_respond);
}

void Component_Action_Destroy(ActionComponent* action_component)
{
    Assert(action_component != NULL, "");
    Component_Action_EventRespond_Clear(action_component);
    Queue_Destroy(action_component->m_event_respond_queue, CallBack_EventRespond_Destroy);
    String_Del(action_component->m_local_name);
    MemDel(action_component);
}

void Component_Action_EventRespond_Add(ActionComponent* action_component, Event event, CB_EventRespond_Void_Actor_EventInfo cb_event_respond)
{
    Assert(action_component != NULL, "");
    EventRespond* event_respond = MemNew(String_CStr(action_component->m_local_name), EventRespond);
    event_respond->m_event              = event;
    event_respond->m_cb_actor_respond   = cb_event_respond;
    Queue_Push(EventRespond*, NULL, action_component->m_event_respond_queue, event_respond);
}

static bool CallBack_EventRespond_FindEvent(EventRespond* event_respond, Event event)
{
    return event_respond->m_event == event;
}

void Component_Action_EventRespond_Del(ActionComponent* action_component, Event event)
{
    Assert(action_component != NULL, "");
    EventRespond* event_respond = Queue_RemoveFindFirst(EventRespond*)(action_component->m_event_respond_queue, (CB_FindData_Bool_Ptr32_Ptr32)CallBack_EventRespond_FindEvent, (ptr32)event);
    MemSafeDel(event_respond);
}

void Component_Action_EventRespond_Clear(ActionComponent* action_component)
{
    Assert(action_component != NULL, "");
    Queue_Clear(action_component->m_event_respond_queue, CallBack_EventRespond_Destroy);
}

CB_EventRespond_Void_Actor_EventInfo Component_Action_EventRespond_Get(ActionComponent* action_component, Event event)
{
    Assert(action_component != NULL, "");
    EventRespond* event_respond = Queue_Find(EventRespond*)(action_component->m_event_respond_queue, (CB_FindData_Bool_Ptr32_Ptr32)CallBack_EventRespond_FindEvent, (ptr32)event);
    if( event_respond )
    {
        return event_respond->m_cb_actor_respond;
    }
    return NULL;
}

