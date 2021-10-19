#include "CoreMini.h"

#include "Event.h"
#include "KeyId.h"

#include "MemoryManager.h"
#include "EventManager.h"

#include "Actor.h"
#include "Scene.h"
#include "Component.h"
#include "ControlComponent.h"

#include "Queue.h"
#include "String.h"


typedef struct ControlEventRespond ControlEventRespond;

struct ControlEventRespond
{
    Event                                       m_event;
    CB_RespondCondition_Bool_Actor_EventInfo    m_cb_respond_condition_bool_actor_eventinfo;
    CB_RespondAction_Void_Actor_EventInfo       m_cb_respond_action_void_actor_eventinfo;
};

struct ControlComponent
{
    String*                         m_local_name;
    Queue(ControlEventRespond*)*    m_control_event_respond_queue;
};


ControlComponent* Component_Control_Create(const tchar* local_name, Actor* actor)
{
    ControlComponent* event_respond_component  = MemNew(local_name, ControlComponent);
    event_respond_component->m_control_event_respond_queue  = Queue_Create(local_name, ControlEventRespond*);
    event_respond_component->m_local_name           = String_New(local_name, local_name, true);

    Scene_ControlEventGroup_Actor_Add(Actor_OwnerScene_Get(actor), actor, Event_Control_Min);

    return event_respond_component;
}

static void CallBack_EventRespond_Destroy(ControlEventRespond* event_respond)
{
    MemDel(event_respond);
}

void Component_Control_Destroy(ControlComponent* control_component)
{
    Assert(control_component != NULL, "");
    Component_Control_EventRespond_Clear(control_component);
    Queue_Destroy(control_component->m_control_event_respond_queue, CallBack_EventRespond_Destroy);
    String_Del(control_component->m_local_name);
    MemDel(control_component);
}

void Component_Control_EventRespond_Add(ControlComponent* control_component, Event event, CB_RespondCondition_Bool_Actor_EventInfo cb_respond_condition_bool_actor_eventinfo, CB_RespondAction_Void_Actor_EventInfo cb_respond_action_void_actor_eventinfo)
{
    Assert(control_component != NULL, "");
    ControlEventRespond* control_event_respond = MemNew(String_CStr(control_component->m_local_name), ControlEventRespond);
    control_event_respond->m_event              = event;
    control_event_respond->m_cb_respond_action_void_actor_eventinfo     = cb_respond_action_void_actor_eventinfo;
    control_event_respond->m_cb_respond_condition_bool_actor_eventinfo  = cb_respond_condition_bool_actor_eventinfo;
    Queue_Push(ControlEventRespond*, NULL, control_component->m_control_event_respond_queue, control_event_respond);
}

static bool CallBack_EventRespond_FindEvent(ControlEventRespond* event_respond, Event event)
{
    return event_respond->m_event == event;
}

void Component_Control_EventRespond_Del(ControlComponent* control_component, Event event)
{
    Assert(control_component != NULL, "");
    ControlEventRespond* event_respond = Queue_RemoveFindFirst(ControlEventRespond*)(control_component->m_control_event_respond_queue, (CB_FindData_Bool_tPtr_tPtr)CallBack_EventRespond_FindEvent, (tptr)event, NULL);
    MemSafeDel(event_respond);
}

void Component_Control_EventRespond_Clear(ControlComponent* control_component)
{
    Assert(control_component != NULL, "");
    Queue_Clear(control_component->m_control_event_respond_queue, CallBack_EventRespond_Destroy);
}


void CallBack_Actor_ProcessSceneEvent(Actor* actor, const EventInfo* event_info)
{
    ControlComponent* control_component = Actor_Component_Cast(actor, Component_Control);
    Assert(control_component != NULL, "");

    ControlEventRespond* event_respond = Queue_Find(ControlEventRespond*)(control_component->m_control_event_respond_queue, (CB_FindData_Bool_tPtr_tPtr)CallBack_EventRespond_FindEvent, (tptr)event_info->m_event);
    
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
    ControlComponent* control_component = Actor_Component_Cast(actor, Component_Control);
    Assert(control_component != NULL, "");

    ControlEventRespond* event_respond = Queue_Find(ControlEventRespond*)(control_component->m_control_event_respond_queue, (CB_FindData_Bool_tPtr_tPtr)CallBack_EventRespond_FindEvent, (tptr)event_info->m_event);

    // We storage all event in same queue, so we need to check if the event has a respond or not
    if( event_respond == NULL )
    {
        return;
    }

    bool is_can_respond = true;
    Assert(event_respond->m_cb_respond_action_void_actor_eventinfo != NULL, "");


    if( event_respond->m_cb_respond_condition_bool_actor_eventinfo )
    {
        is_can_respond = event_respond->m_cb_respond_condition_bool_actor_eventinfo(actor, event_info);
    }

    if( is_can_respond )
    {
        event_respond->m_cb_respond_action_void_actor_eventinfo(actor, event_info);
    }
}

