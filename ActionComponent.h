#pragma once

typedef enum Event Event;
typedef struct ActionComponent ActionComponent;
typedef struct Sence Sence;
typedef struct Actor Actor;

typedef struct EventStruct EventStruct;
struct EventStruct
{
    Event   m_event;
    Actor*  m_actor;
    Sence*  m_sence;
};

typedef void (*CB_EventRespond)(Actor*, EventStruct*);


ActionComponent*    Component_Action_Create     (const tchar* local_name);
void                Component_Action_Destroy    (ActionComponent* event_respond_component);

void                Component_Action_EventRespond_Add   (ActionComponent* action_component, Event event, CB_EventRespond cb_event_respond);
void                Component_Action_EventRespond_Del   (ActionComponent* action_component, Event event);
void                Component_Action_EventRespond_Clear (ActionComponent* action_component);
CB_EventRespond     Component_Action_EventRespond_Get   (ActionComponent* action_component, Event event);
