#include "CoreMini.h"

#include "Event.h"
#include "KeyId.h"

#include "MemoryManager.h"
#include "EventManager.h"

#include "Actor.h"
#include "Scene.h"
#include "Component.h"
#include "ActionComponent.h"

#include "Queue.h"
#include "String.h"


typedef struct EventRespond EventRespond;
typedef struct ActorEvent ActorEvent;

struct EventRespond
{
    Event           m_event;
    CB_RespondCondition_Bool_Actor_EventInfo    m_cb_respond_condition_bool_actor_eventinfo;
    CB_RespondAction_Void_Actor_EventInfo       m_cb_respond_action_void_actor_eventinfo;
};

struct ActionComponent
{
    String*                 m_local_name;
    Queue(EventRespond*)*   m_event_respond_queue;
};


ActionComponent* Component_Action_Create(const tchar* local_name, Actor* actor)
{
    ActionComponent* event_respond_component  = MemNew(local_name, ActionComponent);
    event_respond_component->m_event_respond_queue  = Queue_Create(local_name, EventRespond*);
    event_respond_component->m_local_name           = String_New(local_name, local_name, true);

    Scene_ActionEventGroup_Actor_Add(Actor_ParentScene_Get(actor), actor, Event_Actor_Action_Min);

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

void Component_Action_EventRespond_Add(ActionComponent* action_component, Event event, CB_RespondCondition_Bool_Actor_EventInfo cb_respond_condition_bool_actor_eventinfo, CB_RespondAction_Void_Actor_EventInfo cb_respond_action_void_actor_eventinfo)
{
    Assert(action_component != NULL, "");
    EventRespond* event_respond = MemNew(String_CStr(action_component->m_local_name), EventRespond);
    event_respond->m_event              = event;
    event_respond->m_cb_respond_action_void_actor_eventinfo     = cb_respond_action_void_actor_eventinfo;
    event_respond->m_cb_respond_condition_bool_actor_eventinfo  = cb_respond_condition_bool_actor_eventinfo;
    Queue_Push(EventRespond*, NULL, action_component->m_event_respond_queue, event_respond);
}

static bool CallBack_EventRespond_FindEvent(EventRespond* event_respond, Event event)
{
    return event_respond->m_event == event;
}

void Component_Action_EventRespond_Del(ActionComponent* action_component, Event event)
{
    Assert(action_component != NULL, "");
    EventRespond* event_respond = Queue_RemoveFindFirst(EventRespond*)(action_component->m_event_respond_queue, (CB_FindData_Bool_tPtr_tPtr)CallBack_EventRespond_FindEvent, (tptr)event);
    MemSafeDel(event_respond);
}

void Component_Action_EventRespond_Clear(ActionComponent* action_component)
{
    Assert(action_component != NULL, "");
    Queue_Clear(action_component->m_event_respond_queue, CallBack_EventRespond_Destroy);
}


void CallBack_Actor_ProcessSceneEvent(Actor* actor, const EventInfo* event_info)
{
    ActionComponent* action_component = Actor_Component_Cast(actor, Component_Action);
    Assert(action_component != NULL, "");

    EventRespond* event_respond = Queue_Find(EventRespond*)(action_component->m_event_respond_queue, (CB_FindData_Bool_tPtr_tPtr)CallBack_EventRespond_FindEvent, (tptr)event_info->m_event);
    
    Assert(event_respond != NULL, "");
    Assert(event_respond->m_cb_respond_action_void_actor_eventinfo != NULL, "");

    if( event_respond->m_cb_respond_condition_bool_actor_eventinfo )
    {
        if( event_respond->m_cb_respond_condition_bool_actor_eventinfo(actor, event_info) )
        {
            event_respond->m_cb_respond_action_void_actor_eventinfo(actor, event_info);
        }
    }
    else
    {
        event_respond->m_cb_respond_action_void_actor_eventinfo(actor, event_info);
    }
}

void CallBack_Actor_ProcessActionEvent(Actor* actor, const EventInfo* event_info)
{
    ActionComponent* action_component = Actor_Component_Cast(actor, Component_Action);
    Assert(action_component != NULL, "");

    EventRespond* event_respond = Queue_Find(EventRespond*)(action_component->m_event_respond_queue, (CB_FindData_Bool_tPtr_tPtr)CallBack_EventRespond_FindEvent, (tptr)event_info->m_event);

    // We storage all event in same queue, so we need to check if the event has a respond or not
    if( event_respond == NULL )
    {
        return;
    }

    Assert(event_respond->m_cb_respond_action_void_actor_eventinfo != NULL, "");
    if (event_respond->m_cb_respond_condition_bool_actor_eventinfo)
    {
        if (event_respond->m_cb_respond_condition_bool_actor_eventinfo(actor, event_info))
        {
            event_respond->m_cb_respond_action_void_actor_eventinfo(actor, event_info);
        }
    }
    else
    {
        event_respond->m_cb_respond_action_void_actor_eventinfo(actor, event_info);
    }
}

