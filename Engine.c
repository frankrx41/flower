#include "CoreMini.h"

#include "Engine.h"

#define PUBLIC_MEMORY 1
#include "Memory.h"

#define PUBLIC_TIMMING 1
#include "Timming.h"

#include "Render.h"

struct Engine
{

    bool    m_is_initialized;

    Timming m_timming;
    Memory  m_memory;

};

void Engine_Initialize()
{
    Engine_Memory_Initialize();
    Engine_Timming_Initialize();
    RenderManager_Initialize();

    Engine* engine = Engine_GetInstance();
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

// Memory
Memory* Engine_Memory_GetInstance()
{
    return &Engine_GetInstance()->m_memory;
}
