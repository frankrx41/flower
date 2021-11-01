#pragma once

typedef struct EventManager EventManager;
typedef struct Scene Scene;
typedef enum Event EventId;
typedef struct EventInfo EventInfo;
typedef struct Actor Actor;
typedef enum KeyId KeyId;
typedef struct strcrc strcrc;


struct EventInfo
{
    EventId m_event;
    Actor*  m_actor;
    Scene*  m_scene;
    KeyId   m_key_id;
    float   m_delta_seconds;
};

EventInfo*  EventInfo_Create    (const strcrc* local_name, EventId event, Scene* scene, Actor* actor, KeyId key_id, float delta_second);
void        EventInfo_Destroy   (EventInfo* event_info);

bool    EventId_IsSceneEventId      (EventId event_id);
bool    EventId_IsControlEventId    (EventId event_id);
bool    EventId_IsPhysicEventId     (EventId event_id);

void    EventManager_SendEvent_Update       (EventManager* event_manager, EventId event, float delta_seconds);
void    EventManager_SendEvent_Control      (EventManager* event_manager, EventId event);

EventManager* EventManager_GetInstance();

// gcc requires that enum must be fully declaration, but clang and msbuild do not have this requirement.
// We force these enumerations to be quoted to make sure our code can be compiled correctly by gcc.
COMPILE_TIME_REQUIRE(EVENT);
COMPILE_TIME_REQUIRE(KEYID);
