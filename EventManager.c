#include "CoreMini.h"

#include "Event.h"
#include "KeyId.h"

#include "EventManager.h"
#include "MemoryManager.h"
#include "SceneManager.h"

#include "Scene.h"
#include "Queue.h"
#include "String.h"


struct EventManager
{
    int32   m_dump;
};

EventManager* EventManager_Create(strcrc* local_name)
{
    EventManager* event_manger = MemNew(local_name, EventManager);
    return event_manger;
}

void EventManager_Destroy(EventManager* event_manager)
{
    MemDel(event_manager);
}

EventInfo* EventInfo_Create(const strcrc* local_name, Event event, Scene* scene, Actor* actor, KeyId key_id, float delta_second)
{
    Assert(local_name != NULL, "");

    EventInfo* event_info   = MemNew(local_name, EventInfo);
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

void EventManager_SendEvent_Update(EventManager* event_manager, Event event, float delta_seconds)
{
    strcrc local_name = StrCrc("EventManager_Tick", 0);

    EventInfo* event_info = EventInfo_Create(&local_name, event, NULL, NULL, KeyId_Null, delta_seconds);
    SceneManager_SceneEvent_SendTo_Scene(SceneManager_GetInstance(), event_info);
    EventInfo_Destroy(event_info);
}

////////////////////////////////////////////////////////////////////////////////
static void CallBack_SendEvent_Actor_Action(Scene* scene, const EventInfo* event_info)
{
    strcrc local_name = StrCrc("EventManager_Control", 0);

    Assert(scene != NULL, "");
    EventInfo* event_info_actor_action = EventInfo_Create(&local_name, event_info->m_event, scene, NULL, KeyId_Null, event_info->m_delta_seconds);
    Scene_ControlEvent_SendTo_Actor(scene, event_info_actor_action);
    EventInfo_Destroy(event_info_actor_action);
}

void EventManager_SendEvent_Control(EventManager* event_manager, Event event)
{
    strcrc local_name = StrCrc("EventManager_Action", 0);

    Assert(IS_IN_RANGE(event, Event_Control_Min, Event_Control_Max), "");
    EventInfo* event_info = EventInfo_Create(&local_name, event, NULL, NULL, KeyId_Null, 0);
    Queue_ForEach(SceneManager_SceneQueue_Foreground_Get(SceneManager_GetInstance()), CallBack_SendEvent_Actor_Action, event_info);
    EventInfo_Destroy(event_info);
}
