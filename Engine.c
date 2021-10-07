#include "CoreMini.h"

#include "Engine.h"
#include "MemoryManager.h"
#include "TimmingManager.h"
#include "RenderManager.h"
#include "EventManager.h"
#include "Sence.h"
#include "Event.h"

#define LOCAL_NAME          "GPYM"


struct Engine
{

    bool    m_is_initialized;
    bool    m_is_exit;

    TimmingManager* m_timming_manager;
    MemoryManager*  m_memory_manager;
    RenderManager*  m_render_manager;
    EventManager*   m_event_manager;

    // TODO: Add SenceManager
    Sence*          m_current_sence;
};

RenderManager*      RenderManager_Create    (const tchar* local_name);
MemoryManager*      MemoryManager_Create    (const tchar* local_name);
TimmingManager*     TimmingManager_Create   (const tchar* local_name);
EventManager*       EventManager_Create     (const tchar* local_name);

void                Engine_Memory_Check_Memory_Leak ();

void Engine_Initialize()
{
    Engine* engine = Engine_GetInstance();

    engine->m_render_manager    = RenderManager_Create(LOCAL_NAME);
    engine->m_timming_manager   = TimmingManager_Create(LOCAL_NAME);
    engine->m_memory_manager    = MemoryManager_Create(LOCAL_NAME);
    engine->m_event_manager     = EventManager_Create(LOCAL_NAME);
    TimmingManager_SetFrameRate(engine->m_timming_manager, 60);
    
    engine->m_is_initialized    = true;
    engine->m_is_exit           = false;
}

Engine* Engine_GetInstance()
{
    static Engine engine;
    return &engine;
}

void Engine_MainLoop()
{
    Assert(Engine_GetInstance()->m_is_initialized == true, "");

    for(;!Engine_GetInstance()->m_is_exit;)
    {
        Sence* curent_sence = Engine_Sence_GetCurrentSence();
        RenderManager_RenderSence(RenderManager_GetInstance(), curent_sence);

        RenderManager_RenderToScreen(RenderManager_GetInstance());

        TimmingManager_TrimSpeed(TimmingManager_GetInstance());

        EventManager_SendEvent(Event_Tick, TimmingManager_GetPrevFrameDeltaSeconds(TimmingManager_GetInstance()));
    }
}

void Engine_Exit()
{
    Engine_GetInstance()->m_is_exit = true;
}

void Engine_UnInitialize()
{
    Engine_Memory_Check_Memory_Leak();
}

Sence* Engine_Sence_GetCurrentSence()
{
    return Engine_GetInstance()->m_current_sence;
}

void Engine_Sence_SetCurrentSence(Sence* sence)
{
    Engine_GetInstance()->m_current_sence = sence;
}

// TimmingManager
TimmingManager* TimmingManager_GetInstance()
{
    return Engine_GetInstance()->m_timming_manager;
}

// MemoryManager
MemoryManager* MemoryManager_GetInstance()
{
    return Engine_GetInstance()->m_memory_manager;
}

// RenderManager
RenderManager* RenderManager_GetInstance()
{
    return Engine_GetInstance()->m_render_manager;
}

// Sence
void Engin_Sence_SetCurrentSence(Sence* sence)
{
    Engine_GetInstance()->m_current_sence;
}

// EventManager
EventManager* EventManager_GetInstance()
{
    return Engine_GetInstance()->m_event_manager;
}
