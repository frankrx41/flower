#pragma once

typedef enum Event Event;

typedef struct ControlComponent ControlComponent;
typedef struct EventInfo EventInfo;
typedef struct Scene Scene;
typedef struct Actor Actor;

typedef void (*CB_RespondAction_Void_Actor_EventInfo)       (Actor*, const EventInfo*);
typedef bool (*CB_RespondCondition_Bool_Actor_EventInfo)    (Actor*, const EventInfo*);


ControlComponent*   Component_Control_Create     (const strcrc* local_name, Actor* actor);
void                Component_Control_Destroy    (ControlComponent* control_component);

void                Component_Control_EventRespond_Add      (ControlComponent* control_component, Event event, CB_RespondCondition_Bool_Actor_EventInfo cb_respond_condition_bool_actor_eventinfo, CB_RespondAction_Void_Actor_EventInfo cb_respond_action_void_actor_eventinfo);
void                Component_Control_EventRespond_Del      (ControlComponent* control_component, Event event);
void                Component_Control_EventRespond_Clear    (ControlComponent* control_component);

