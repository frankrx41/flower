#include "CoreMini.h"

#include "Engine.h"

#include "KeyId.h"
#include "Event.h"

#include "MemoryManager.h"
#include "TimingManager.h"
#include "RenderManager.h"
#include "EventManager.h"
#include "InputManager.h"
#include "SceneManager.h"

#define LOCAL_NAME          "GPYM"
#define LOCAL_NAME_EVENT    "Event"

typedef struct SceneManager SceneManager;
typedef struct Engine   Engine;

struct Engine
{

    bool    m_is_initialized;
    bool    m_is_exit;

    TimingManager*  m_timing_manager;
    MemoryManager*  m_memory_manager;
    RenderManager*  m_render_manager;
    EventManager*   m_event_manager;
    SceneManager*   m_scene_manager;
    InputManager*   m_input_manager;
};

static Engine global_engine;

RenderManager*      RenderManager_Create    (const tchar* local_name);
void                RenderManager_Destroy   (RenderManager* render_manager);
MemoryManager*      MemoryManager_Create    (const tchar* local_name);
void                MemoryManager_Destroy   (MemoryManager* memory_manager);
TimingManager*      TimingManager_Create    (const tchar* local_name);
void                TimingManager_Destroy   (TimingManager* timing_manager);
EventManager*       EventManager_Create     (const tchar* local_name);
void                EventManager_Destroy    (EventManager* event_manager);
SceneManager*       SceneManager_Create     (const tchar* local_name);
void                SceneManager_Destroy    (SceneManager* scene_manager);
InputManager*       InputManager_Create     (const tchar* local_name);
void                InputManager_Destroy    (InputManager* input_manager);

void                Engine_Debug_Memory_Check_Leak          ();
void                Engine_Debug_Memory_Static_Check_Leak   ();

static Engine* Engine_GetInstance()
{
    return &global_engine;
}

void Engine_Initialize()
{
    Engine* engine = Engine_GetInstance();
    engine->m_render_manager    = RenderManager_Create(LOCAL_NAME);
    engine->m_timing_manager    = TimingManager_Create(LOCAL_NAME);
    engine->m_memory_manager    = MemoryManager_Create(LOCAL_NAME);
    engine->m_event_manager     = EventManager_Create(LOCAL_NAME);
    engine->m_scene_manager     = SceneManager_Create(LOCAL_NAME);
    engine->m_input_manager     = InputManager_Create(LOCAL_NAME);

    engine->m_is_initialized    = true;
    engine->m_is_exit           = false;
}


void Engine_MainLoop()
{
    Engine* engine = Engine_GetInstance();
    Assert(engine->m_is_initialized == true, "");

    float delta_second = TimingManager_GetPrevFrameDeltaSeconds(TimingManager_GetInstance());
    for(;!engine->m_is_exit;)
    {
        InputManager_Keys_UpdateState(InputManager_GetInstance(), delta_second);
        InputManager_Event_Send(InputManager_GetInstance(), LOCAL_NAME_EVENT);

        Scene* current_scene = SceneManager_Scene_GetCurrent();
        RenderManager_RenderScene(RenderManager_GetInstance(), current_scene);

        RenderManager_Render_ToScreen(RenderManager_GetInstance());

        TimingManager_TrimSpeed(TimingManager_GetInstance());

        delta_second = TimingManager_GetPrevFrameDeltaSeconds(TimingManager_GetInstance());
        EventManager_SendEvent(Event_Scene_Tick, LOCAL_NAME_EVENT, current_scene, delta_second);
    }
}

void Engine_UnInitialize()
{
    Engine_SetExit(true);

    Engine_Debug_Memory_Check_Leak();

    Engine* engine = Engine_GetInstance();

    RenderManager_Destroy(engine->m_render_manager);
    TimingManager_Destroy(engine->m_timing_manager);
    MemoryManager_Destroy(engine->m_memory_manager);
    EventManager_Destroy(engine->m_event_manager);
    SceneManager_Destroy(engine->m_scene_manager);
    InputManager_Destroy(engine->m_input_manager);

    Engine_Debug_Memory_Static_Check_Leak();
}

void Engine_SetExit(bool is_exit)
{
    Engine_GetInstance()->m_is_exit = is_exit;
}

bool Engine_IsExit()
{
    return Engine_GetInstance()->m_is_exit;
}

// TimingManager
TimingManager* TimingManager_GetInstance()
{
    return Engine_GetInstance()->m_timing_manager;
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

// EventManager
EventManager* EventManager_GetInstance()
{
    return Engine_GetInstance()->m_event_manager;
}

// SceneManager
SceneManager* SceneManager_GetInstance()
{
    return Engine_GetInstance()->m_scene_manager;
}

// InputManager
InputManager* InputManager_GetInstance()
{
    return Engine_GetInstance()->m_input_manager;
}

