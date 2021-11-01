#pragma once

typedef enum Event EventId;
typedef struct Component Component;

typedef struct EventInfo EventInfo;
typedef struct Scene Scene;
typedef struct Actor Actor;

typedef void (*CB_RespondAction_Void_Actor_EventInfo)       (Actor*, const EventInfo*);
typedef bool (*CB_RespondCondition_Bool_Actor_EventInfo)    (Actor*, const EventInfo*);


void*   ComponentData_Control_Create        (const strcrc* local_name, Actor* actor);
void    ComponentData_Control_Destroy       (Component* component);

void    Component_Control_EventRespond_Add      (Component* component, EventId event_id, CB_RespondCondition_Bool_Actor_EventInfo cb_respond_condition_bool_actor_eventinfo, CB_RespondAction_Void_Actor_EventInfo cb_respond_action_void_actor_eventinfo);
void    Component_Control_EventRespond_Del      (Component* component, EventId event_id);
void    Component_Control_EventRespond_Clear    (Component* component);

