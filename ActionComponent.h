#pragma once

typedef enum Event Event;
typedef struct ActionComponent ActionComponent;
typedef struct Scene Scene;
typedef struct Actor Actor;

typedef struct EventInfo EventInfo;


typedef void (*CB_EventRespond_Void_Actor_EventInfo)    (Actor*, const EventInfo*);
typedef bool (*CB_RespondCondition_Void_Actor_EventInfo)(Actor*, const EventInfo*);


ActionComponent*    Component_Action_Create     (const tchar* local_name);
void                Component_Action_Destroy    (ActionComponent* action_component);

void                Component_Action_EventRespond_Add   (ActionComponent* action_component, Event event, CB_RespondCondition_Void_Actor_EventInfo cb_respond_condition_void_actor_event_info, CB_EventRespond_Void_Actor_EventInfo cb_event_respond);
void                Component_Action_EventRespond_Del   (ActionComponent* action_component, Event event);
void                Component_Action_EventRespond_Clear (ActionComponent* action_component);

