#pragma once

typedef struct EventManager EventManager;
typedef struct Sence Sence;
typedef enum Event Event;
typedef struct EventInfo EventInfo;
typedef struct Actor Actor;

struct EventInfo
{
    Event   m_event;
    Actor*  m_actor;
    Sence*  m_sence;
    float   m_delta_seconds;
};

void EventManger_SendEvent_Tick(EventManager* event_manager, Event event, float delta_seconds);


EventManager* EventManager_GetInstance(const Engine* engine);


#define EventManager_SendEvent(event, ...)   MACRO_CONNNECT(EventManger_Send, event)(EventManager_GetInstance(Engine_GetInstance()), event, __VA_ARGS__)

