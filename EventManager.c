#include "CoreMini.h"

#include "EventManager.h"
#include "MemoryManager.h"
#include "Sence.h"
#include "Event.h"
#include "Engine.h"

struct EventManager
{
    int32   m_dump;
};

EventManager* EventManager_Create(const tchar* local_name)
{
    EventManager* event_manger = MemNew(local_name, EventManager);
    return event_manger;
}


void EventManger_SendEvent_Tick(EventManager* event_manager, Event event, float delta_seconds)
{
    Sence* sence = Engine_Sence_GetCurrentSence();
    EventInfo event_info;
    event_info.m_event = event;
    event_info.m_sence = sence;
    event_info.m_actor = NULL;
    Sence_Actor_SendEvent(sence, &event_info);
}

