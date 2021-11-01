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
    EventId                                     m_event;
    CB_RespondCondition_Bool_Actor_EventInfo    m_cb_respond_condition_bool_actor_eventinfo;
    CB_RespondAction_Void_Actor_EventInfo       m_cb_respond_action_void_actor_eventinfo;
};

typedef struct ControlComponentData ControlComponentData;
struct ControlComponentData
{
    strcrc                          m_local_name;
    Queue(ControlEventRespond*)*    m_control_event_respond_queue;
};


void* ComponentData_Control_Create(const strcrc* local_name, Actor* actor)
{
    ControlComponentData* control_component_data  = MemNew(local_name, ControlComponentData);
    control_component_data->m_control_event_respond_queue  = Queue_Create(local_name, ControlEventRespond*);
    StrCrc_Copy(local_name, &control_component_data->m_local_name);

    Scene_EventGroup_Actor_Add(Actor_GetExistScene(actor), actor, Event_Control_Any);

    return control_component_data;
}

static void CallBack_EventRespond_Destroy(ControlEventRespond* event_respond)
{
    MemDel(event_respond);
}

void ComponentData_Control_Destroy(Component* component)
{
    Assert(component != NULL, "");
    Assert(Component_GetEnum(component) == Component_Control, "");

    ControlComponentData* control_component_data = Component_GetData(component);
    Component_Control_EventRespond_Clear(component);
    Queue_Destroy(control_component_data->m_control_event_respond_queue, CallBack_EventRespond_Destroy);

    MemDel(control_component_data);
}

void Component_Control_EventRespond_Add(Component* component, EventId event_id, CB_RespondCondition_Bool_Actor_EventInfo cb_respond_condition_bool_actor_eventinfo, CB_RespondAction_Void_Actor_EventInfo cb_respond_action_void_actor_eventinfo)
{
    Assert(event_id != Event_Scene_Update_Physics, "Update_Physics is handle by engine");
    Assert(Component_GetEnum(component) == Component_Control, "");

    Actor* actor = Component_GetActor(component);

    Scene_EventGroup_Actor_Add(Actor_GetExistScene(actor), actor, event_id);

    Assert(component != NULL, "");
    ControlComponentData* control_component_data = Component_GetData(component);
    ControlEventRespond* control_event_respond  = MemNew(&control_component_data->m_local_name, ControlEventRespond);
    control_event_respond->m_event              = event_id;
    control_event_respond->m_cb_respond_action_void_actor_eventinfo     = cb_respond_action_void_actor_eventinfo;
    control_event_respond->m_cb_respond_condition_bool_actor_eventinfo  = cb_respond_condition_bool_actor_eventinfo;
    Queue_Push(ControlEventRespond*, NULL, control_component_data->m_control_event_respond_queue, control_event_respond);
}

static bool CallBack_EventRespond_FindEvent(ControlEventRespond* event_respond, EventId event)
{
    return event_respond->m_event == event;
}

void Component_Control_EventRespond_Del(Component* component, EventId event_id)
{
    Assert(component != NULL, "");
    Assert(Component_GetEnum(component) == Component_Control, "");

    ControlComponentData* control_component_data = Component_GetData(component);
    ControlEventRespond* control_event_respond = Queue_RemoveFindFirst(ControlEventRespond*)(control_component_data->m_control_event_respond_queue, (CB_FindData_Bool_tPtr_tPtr)CallBack_EventRespond_FindEvent, (void*)event_id, NULL);
    MemSafeDel(control_event_respond);
}

void Component_Control_EventRespond_Clear(Component* component)
{
    Assert(component != NULL, "");
    Assert(Component_GetEnum(component) == Component_Control, "");

    ControlComponentData* control_component_data = Component_GetData(component);
    Queue_Clear(control_component_data->m_control_event_respond_queue, CallBack_EventRespond_Destroy);
}


void CallBack_Actor_Receive_SceneEvent(Actor* actor, const EventInfo* event_info)
{
    Component* component = Actor_Component_Cast(actor, Component_Control);
    Assert(component != NULL, "");

    ControlComponentData* control_component_data = Component_GetData(component);
    ControlEventRespond* control_event_respond = Queue_Find(ControlEventRespond*)(control_component_data->m_control_event_respond_queue, (CB_FindData_Bool_tPtr_tPtr)CallBack_EventRespond_FindEvent, (void*)event_info->m_event);
    
    Assert(control_event_respond != NULL, "");
    Assert(control_event_respond->m_cb_respond_action_void_actor_eventinfo != NULL, "");

    if( control_event_respond->m_cb_respond_condition_bool_actor_eventinfo )
    {
        if( control_event_respond->m_cb_respond_condition_bool_actor_eventinfo(actor, event_info) )
        {
            control_event_respond->m_cb_respond_action_void_actor_eventinfo(actor, event_info);
        }
    }
    else
    {
        control_event_respond->m_cb_respond_action_void_actor_eventinfo(actor, event_info);
    }
}

void CallBack_Actor_Receive_ControlEvent(Actor* actor, const EventInfo* event_info)
{
    Component* component = Actor_Component_Cast(actor, Component_Control);
    Assert(component != NULL, "");

    ControlComponentData* control_component_data = Component_GetData(component);
    ControlEventRespond* control_event_respond = Queue_Find(ControlEventRespond*)(control_component_data->m_control_event_respond_queue, (CB_FindData_Bool_tPtr_tPtr)CallBack_EventRespond_FindEvent, (void*)event_info->m_event);

    // We storage all event in same queue, so we need to check if the event has a respond or not
    if( control_event_respond == NULL )
    {
        return;
    }

    bool is_can_respond = true;
    Assert(control_event_respond->m_cb_respond_action_void_actor_eventinfo != NULL, "");


    if( control_event_respond->m_cb_respond_condition_bool_actor_eventinfo )
    {
        is_can_respond = control_event_respond->m_cb_respond_condition_bool_actor_eventinfo(actor, event_info);
    }

    if( is_can_respond )
    {
        control_event_respond->m_cb_respond_action_void_actor_eventinfo(actor, event_info);
    }
}

