#include "CoreMini.h"

#include "Engine.h"
#include "MemoryManager.h"
#include "TimmingManager.h"
#include "Render.h"

#define LOCAL_NAME          "GPYM"


struct Engine
{

    bool    m_is_initialized;

    TimmingManager* m_timming_manager;
    MemoryManager*  m_memory_manager;

};


MemoryManager*      MemoryManager_Create    (const tchar* local_name);
TimmingManager*     TimmingManager_Create   (const tchar* local_name);

void Engine_Initialize()
{
    Engine* engine = Engine_GetInstance();

    RenderManager_Initialize();

    engine->m_timming_manager   = TimmingManager_Create(LOCAL_NAME);
    engine->m_memory_manager    = MemoryManager_Create(LOCAL_NAME);
    engine->m_is_initialized    = true;
}

Engine* Engine_GetInstance()
{
    static Engine engine;
    return &engine;
}

void Engine_MainLoop()
{
    Assert(Engine_GetInstance()->m_is_initialized == true, "");

    for(;;)
    {
        TimmingManager_TrimSpeed(Engine_GetInstance()->m_timming_manager);
    }
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
