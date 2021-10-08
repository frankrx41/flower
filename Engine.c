#include "CoreMini.h"

#include "Engine.h"
#include "MemoryManager.h"
#include "TimmingManager.h"
#include "RenderManager.h"
#include "EventManager.h"
#include "Event.h"
#include "InputManager.h"
#include "SenceManager.h"

#define LOCAL_NAME          "GPYM"
#define LOCAL_NAME_EVENT    "Event"

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
    InputManager*   m_input_manager;
};

RenderManager*      RenderManager_Create    (const tchar* local_name);
MemoryManager*      MemoryManager_Create    (const tchar* local_name);
TimmingManager*     TimmingManager_Create   (const tchar* local_name);
EventManager*       EventManager_Create     (const tchar* local_name);
SenceManager*       SenceManager_Create     (const tchar* local_name);
InputManager*       InputManager_Create     (const tchar* local_name);

void                Engine_Memory_Check_Memory_Leak ();

void Engine_Initialize(Engine* engine)
{
    engine->m_render_manager    = RenderManager_Create(LOCAL_NAME);
    engine->m_timming_manager   = TimmingManager_Create(LOCAL_NAME);
    engine->m_memory_manager    = MemoryManager_Create(LOCAL_NAME);
    engine->m_event_manager     = EventManager_Create(LOCAL_NAME);
    engine->m_sence_manager     = SenceManager_Create(LOCAL_NAME);
    engine->m_input_manager     = InputManager_Create(LOCAL_NAME);

    TimmingManager_SetFrameRate(engine->m_timming_manager, 60);
    
    engine->m_is_initialized    = true;
    engine->m_is_exit           = false;
}

Engine* Engine_GetInstance()
{
    static Engine engine;
    return &engine;
}

void Engine_MainLoop(Engine* engine)
{
    Assert(Engine_GetInstance()->m_is_initialized == true, "");

    float delta_second = TimmingManager_GetPrevFrameDeltaSeconds(TimmingManager_GetInstance(engine));
    for(;!engine->m_is_exit;)
    {
        InputManager_Keys_UpdateState(InputManager_GetInstance(engine), delta_second);
        InputManager_Event_Send(InputManager_GetInstance(engine), LOCAL_NAME_EVENT);

        Sence* curent_sence = SenceManager_Sence_GetCurrent();
        RenderManager_RenderSence(RenderManager_GetInstance(engine), curent_sence);

        RenderManager_RenderToScreen(RenderManager_GetInstance(engine));

        TimmingManager_TrimSpeed(TimmingManager_GetInstance(engine));

        delta_second = TimmingManager_GetPrevFrameDeltaSeconds(TimmingManager_GetInstance(engine));
        EventManager_SendEvent(Event_Sence_Tick, LOCAL_NAME_EVENT, curent_sence, delta_second);
    }
}

void Engine_UnInitialize(Engine* engine)
{
    Engine_Memory_Check_Memory_Leak();
}

void Engine_SetExit(Engine* engine, bool is_exit)
{
    engine->m_is_exit = is_exit;
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

// InputManager
InputManager* InputManager_GetInstance(const Engine* engine)
{
    return engine->m_input_manager;
}

