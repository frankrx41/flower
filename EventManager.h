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

void    EventManager_SendEvent_Scene_Tick           (EventManager* event_manager, const tchar* local_name, Event event, Scene* scene, float delta_seconds);
void    EventManager_SendEvent_Key_Any_Down         (EventManager* event_manager, const tchar* local_name, Event event, KeyId key_id);
void    EventManager_SendEvent_Key_Any_Down_Once    (EventManager* event_manager, const tchar* local_name, Event event, KeyId key_id);
void    EventManager_SendEvent_Key_Any_Up_Once      (EventManager* event_manager, const tchar* local_name, Event event, KeyId key_id);
void    EventManager_SendEvent_Key_Any_Hold         (EventManager* event_manager, const tchar* local_name, Event event, KeyId key_id, float delta_seconds);
void    EventManager_SendEvent_Key_Any_Toggle       (EventManager* event_manager, const tchar* local_name, Event event, KeyId key_id);


EventManager* EventManager_GetInstance();


#define EventManager_SendEvent(event, local_name, ...)   MACRO_CONNNECT(EventManager_Send, event)(EventManager_GetInstance(), local_name, event, __VA_ARGS__)

