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
#include "TaskManager.h"

#include "String.h"


typedef struct Engine   Engine;

struct Engine
{

    bool    m_is_initialized;
    bool    m_is_exit;

    TimingManager*  m_timing_manager;
    RenderManager*  m_render_manager;
    EventManager*   m_event_manager;
    SceneManager*   m_scene_manager;
    InputManager*   m_input_manager;
    TaskManager*    m_task_manager;
};

static Engine global_engine;

RenderManager*      RenderManager_Create    (const strcrc* local_name);
void                RenderManager_Destroy   (RenderManager* render_manager);
TimingManager*      TimingManager_Create    (const strcrc* local_name);
void                TimingManager_Destroy   (TimingManager* timing_manager);
EventManager*       EventManager_Create     (const strcrc* local_name);
void                EventManager_Destroy    (EventManager* event_manager);
SceneManager*       SceneManager_Create     (const strcrc* local_name);
void                SceneManager_Destroy    (SceneManager* scene_manager);
InputManager*       InputManager_Create     (const strcrc* local_name);
void                InputManager_Destroy    (InputManager* input_manager);
TaskManager*        TaskManager_Create      (const strcrc* local_name);
void                TaskManager_Destroy     (TaskManager* task_manager);

void                Engine_Debug_Memory_Check_Leak          ();


void    Engine_Debug_UnitTesting0   ();
void    Engine_Debug_UnitTesting1   ();

void    Engine_Initialize   (void);
void    Engine_MainLoop     (void);
void    Engine_UnInitialize (void);

static Engine* Engine_GetInstance()
{
    return &global_engine;
}

void Engine_Initialize()
{
    Engine* engine = Engine_GetInstance();

    strcrc manager_strcrc;

    manager_strcrc = StrCrc("RenderManager", 0);
    engine->m_render_manager    = RenderManager_Create(&manager_strcrc);
    
    manager_strcrc = StrCrc("TimingManager", 0);
    engine->m_timing_manager    = TimingManager_Create(&manager_strcrc);
    
    manager_strcrc = StrCrc("EventManager", 0);
    engine->m_event_manager     = EventManager_Create(&manager_strcrc);
    
    manager_strcrc = StrCrc("SceneManager", 0);
    engine->m_scene_manager     = SceneManager_Create(&manager_strcrc);
    
    manager_strcrc = StrCrc("InputManager", 0);
    engine->m_input_manager     = InputManager_Create(&manager_strcrc);
    
    manager_strcrc = StrCrc("TaskManager", 0);
    engine->m_task_manager      = TaskManager_Create(&manager_strcrc);

    engine->m_is_initialized    = true;
    engine->m_is_exit           = false;
}

void Engine_Main(const tchar* command_line)
{
    Engine_Initialize();

    Engine_Debug_UnitTesting0();
    Engine_Debug_UnitTesting1();

    Engine_MainLoop();

    Engine_UnInitialize();
}

void Engine_MainLoop()
{
    Engine* engine = Engine_GetInstance();
    Assert(engine->m_is_initialized == true, "");

    float delta_second = TimingManager_GetPrevFrameDeltaSeconds(TimingManager_GetInstance());

    for(;;)
    {
        InputManager_KeysState_Update(InputManager_GetInstance(), delta_second);
        InputManager_ControlEvent_Distribute(InputManager_GetInstance());

        SceneManager_TryRunNextCommand(SceneManager_GetInstance());

        if( Engine_IsExit() )
        {
            break;
        }

        RenderManager_RenderAllScene(RenderManager_GetInstance(), SceneManager_GetInstance());

        TimingManager_TrimSpeed(TimingManager_GetInstance());
        delta_second = TimingManager_GetPrevFrameDeltaSeconds(TimingManager_GetInstance());
    }
}

void Engine_UnInitialize()
{
    Engine* engine = Engine_GetInstance();

    TaskManager_Destroy(engine->m_task_manager);
    RenderManager_Destroy(engine->m_render_manager);
    TimingManager_Destroy(engine->m_timing_manager);
    EventManager_Destroy(engine->m_event_manager);
    SceneManager_Destroy(engine->m_scene_manager);
    InputManager_Destroy(engine->m_input_manager);

    Memory_Stat();
    Engine_Debug_Memory_Check_Leak();
}

void Engine_NotifyExit()
{
    Assert(Engine_GetInstance()->m_is_exit == false, "Try repeated exit engine!");
    Engine_GetInstance()->m_is_exit = true;
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

// TaskManager
TaskManager* TaskManager_GetInstance()
{
    return Engine_GetInstance()->m_task_manager;
}
