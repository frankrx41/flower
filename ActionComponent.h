#pragma once

typedef enum Event Event;
typedef struct ActionComponent ActionComponent;
typedef struct Scene Scene;
typedef struct Actor Actor;

typedef struct EventInfo EventInfo;


typedef void (*CB_RespondAction_Void_Actor_EventInfo)    (Actor*, const EventInfo*);
typedef bool (*CB_RespondCondition_Bool_Actor_EventInfo)(Actor*, const EventInfo*);


ActionComponent*    Component_Action_Create     (const tchar* local_name, Actor* actor);
void                Component_Action_Destroy    (ActionComponent* action_component);

void                Component_Action_EventRespond_Add   (ActionComponent* action_component, Event event, CB_RespondCondition_Bool_Actor_EventInfo cb_respond_condition_bool_actor_eventinfo, CB_RespondAction_Void_Actor_EventInfo cb_respond_action_void_actor_eventinfo);
void                Component_Action_EventRespond_Del   (ActionComponent* action_component, Event event);
void                Component_Action_EventRespond_Clear (ActionComponent* action_component);

