#include "CoreMini.h"

#include "Engine.h"

#define PUBLIC_MEMORY 1
#include "Memory.h"

#define PUBLIC_TIMMING 1
#include "Timming.h"

struct Engine
{

    bool    m_is_initialized;
    error   m_last_error;

    Timming m_timming;
    Memory  m_memory;

};

// Engine
error Engine_GetLastError()
{
    return Engine_GetInstance()->m_last_error;
}

error Engine_SetLastError(int32 code)
{
    error prev_error = Engine_GetInstance()->m_last_error;
    Engine_GetInstance()->m_last_error = code;
    return prev_error;
}

error Engine_Initialize()
{
    Engine_Timming_Initialize();
    Engine_Memory_Initialize();

    Engine* engine = Engine_GetInstance();
    engine->m_is_initialized = true;
    return 0;
}

Engine* Engine_GetInstance()
{
    static Engine engine;
    return &engine;
}

error Engine_MainLoop()
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