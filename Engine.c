#include "CoreMini.h"

#include "Engine.h"

#define PUBLIC_MEMORY 1
#include "MemoryManager.h"

#define PUBLIC_TIMMING 1
#include "Timming.h"

#include "Render.h"

struct Engine
{

    bool    m_is_initialized;

    Timming         m_timming;
    MemoryManager*  m_memory_manager;

};


MemoryManager*    MemoryManager_Create        ();

void Engine_Initialize()
{
    Engine_Timming_Initialize();
    RenderManager_Initialize();

    Engine* engine = Engine_GetInstance();
    engine->m_memory_manager = MemoryManager_Create();
    engine->m_is_initialized = true;
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
        Engine_Timming_TrimSpeed_Plat();
    }
}

// Timming
Timming* Engine_Timming_GetInstance()
{
    return &Engine_GetInstance()->m_timming;
}

// MemoryManager
MemoryManager* MemoryManager_GetInstance()
{
    return Engine_GetInstance()->m_memory_manager;
}
