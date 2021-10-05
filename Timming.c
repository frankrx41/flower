#include "CoreMini.h"

#define PUBLIC_TIMMING 1
#include "Timming.h"


void Engine_Timming_Initialize()
{
    Timming* timming = Engine_Timming_GetInstance();
    timming->m_frame_rate           = 60;
    timming->m_is_limit_frame_rate  = false;

    Engine_Timming_Initialize_Plat();

    timming->m_is_initialized       = true;
}

float Engine_Timming_GetPrevFrameDeltaSeconds()
{
    Assert(Engine_Timming_GetInstance()->m_is_initialized == true, NULL);
    return Engine_Timming_GetInstance()->m_prev_frame_delta_seconds;
}

float Engine_Timming_GetFrameRate()
{
    Assert(Engine_Timming_GetInstance()->m_is_initialized == true, NULL);
    return Engine_Timming_GetInstance()->m_frame_rate;
}

void Engine_Timming_SetFrameRate(float fps)
{
    Assert(Engine_Timming_GetInstance()->m_is_initialized == true, NULL);
    Engine_Timming_GetInstance()->m_frame_rate = fps;
    Engine_Timming_SetFrameRate_Plat(fps);
}

void Engine_Timming_SetFrameRateLimit(bool is_limit)
{
    Assert(Engine_Timming_GetInstance()->m_is_initialized == true, NULL);
    Engine_Timming_GetInstance()->m_is_limit_frame_rate = is_limit;
}

bool Engine_Timming_IslimitFrameRate()
{
    Assert(Engine_Timming_GetInstance()->m_is_initialized == true, NULL);
    return Engine_Timming_GetInstance()->m_is_limit_frame_rate;
}

