#include "CoreMini.h"

#include "Engine.h"
#include "MemoryManager.h"
#include "TimmingManager.h"
#include "RenderManager.h"
#include "EventManager.h"
#include "Event.h"
#include "SenceManager.h"

#define LOCAL_NAME          "GPYM"

typedef struct SenceManager SenceManager;

struct Engine
{

    bool    m_is_initialized;
    bool    m_is_exit;

    TimmingManager* m_timming_manager;
    MemoryManager*  m_memory_manager;
    RenderManager*  m_render_manager;
    EventManager*   m_event_manager;
    SenceManager*   m_sence_manager;
};

RenderManager*      RenderManager_Create    (const tchar* local_name);
MemoryManager*      MemoryManager_Create    (const tchar* local_name);
TimmingManager*     TimmingManager_Create   (const tchar* local_name);
EventManager*       EventManager_Create     (const tchar* local_name);
SenceManager*       SenceManager_Create     (const tchar* local_name);

void                Engine_Memory_Check_Memory_Leak ();

void Engine_Initialize()
{
    Engine* engine = Engine_GetInstance();

    engine->m_render_manager    = RenderManager_Create(LOCAL_NAME);
    engine->m_timming_manager   = TimmingManager_Create(LOCAL_NAME);
    engine->m_memory_manager    = MemoryManager_Create(LOCAL_NAME);
    engine->m_event_manager     = EventManager_Create(LOCAL_NAME);
    engine->m_sence_manager     = SenceManager_Create(LOCAL_NAME);

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

    const Engine* engine = Engine_GetInstance();

    for(;!engine->m_is_exit;)
    {
        Sence* curent_sence = SenceManager_Sence_GetCurrent();
        RenderManager_RenderSence(RenderManager_GetInstance(engine), curent_sence);

        RenderManager_RenderToScreen(RenderManager_GetInstance(engine));

        TimmingManager_TrimSpeed(TimmingManager_GetInstance(engine));

        EventManager_SendEvent(Event_Tick, TimmingManager_GetPrevFrameDeltaSeconds(TimmingManager_GetInstance(engine)));
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

// TimmingManager
TimmingManager* TimmingManager_GetInstance(const Engine* engine)
{
    return engine->m_timming_manager;
}

// MemoryManager
MemoryManager* MemoryManager_GetInstance(const Engine* engine)
{
    return engine->m_memory_manager;
}

// RenderManager
RenderManager* RenderManager_GetInstance(const Engine* engine)
{
    return engine->m_render_manager;
}

// EventManager
EventManager* EventManager_GetInstance(const Engine* engine)
{
    return engine->m_event_manager;
}

// SenceManager
SenceManager* SenceManager_GetInstance(const Engine* engine)
{
    return engine->m_sence_manager;
}

