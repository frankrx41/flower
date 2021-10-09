#include "CoreMini.h"

#include "Event.h"
#include "KeyId.h"

#include "EventManager.h"
#include "MemoryManager.h"

#include "Scene.h"


struct EventManager
{
    int32   m_dump;
};

EventManager* EventManager_Create(const tchar* local_name)
{
    EventManager* event_manger = MemNew(local_name, EventManager);
    return event_manger;
}

void EventManager_Destroy(EventManager* event_manager)
{
    MemDel(event_manager);
}

EventInfo* EventInfo_Create(const tchar* local_name, Event event, Scene* scene, Actor* actor, KeyId key_id, float delta_second)
{
    Assert(local_name != NULL, "");
    EventInfo* event_info = MemNew(local_name, EventInfo);
    event_info->m_event     = event;
    event_info->m_scene     = scene;
    event_info->m_actor     = actor;
    event_info->m_key_id    = key_id;
    event_info->m_delta_seconds = delta_second;
    return event_info;
}

void EventInfo_Destroy(EventInfo* event_info)
{
    MemDel(event_info);
}

void EventManager_SendEvent_Scene_Tick(EventManager* event_manager, const tchar* local_name, Event event, Scene* scene, float delta_seconds)
{
    EventInfo* event_info = EventInfo_Create(local_name, event, scene, NULL, KeyId_Null, delta_seconds);
    Scene_Actor_SendEvent(scene, event_info);
    EventInfo_Destroy(event_info);
}

void EventManager_SendEvent_Key_Any_Down        (EventManager* event_manager, const tchar* local_name, Event event, KeyId key_id){}
void EventManager_SendEvent_Key_Any_Down_Once   (EventManager* event_manager, const tchar* local_name, Event event, KeyId key_id){}
void EventManager_SendEvent_Key_Any_Up_Once     (EventManager* event_manager, const tchar* local_name, Event event, KeyId key_id){}
void EventManager_SendEvent_Key_Any_Hold        (EventManager* event_manager, const tchar* local_name, Event event, KeyId key_id, float delta_seconds){}
void EventManager_SendEvent_Key_Any_Toggle      (EventManager* event_manager, const tchar* local_name, Event event, KeyId key_id){}


