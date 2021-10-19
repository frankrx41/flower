#pragma once

typedef struct EventManager EventManager;
typedef struct Scene Scene;
typedef enum Event Event;
typedef struct EventInfo EventInfo;
typedef struct Actor Actor;
typedef enum KeyId KeyId;


struct EventInfo
{
    Event   m_event;
    Actor*  m_actor;
    Scene*  m_scene;
    KeyId   m_key_id;
    float   m_delta_seconds;
};

EventInfo*  EventInfo_Create    (const tchar* local_name, Event event, Scene* scene, Actor* actor, KeyId key_id, float delta_second);
void        EventInfo_Destroy   (EventInfo* event_info);


void    EventManager_SendEvent_Tick                 (EventManager* event_manager, Event event, float delta_seconds);
void    EventManager_SendEvent_Control         (EventManager* event_manager, Event event);

EventManager* EventManager_GetInstance();


#define SendEvent_Tick(event, ...)                  EventManager_SendEvent_Tick(EventManager_GetInstance(), event, __VA_ARGS__)
#define SendEvent_Actor_Action(event, ...)          EventManager_SendEvent_Control(EventManager_GetInstance(), event, __VA_ARGS__)


// gcc requires that enum must be fully declaration, but clang and msbuild do not have this requirement.
// We force these enumerations to be quoted to make sure our code can be compiled correctly by gcc.
COMPILE_TIME_REQUIRE(EVENT);
COMPILE_TIME_REQUIRE(KEYID);
