#include "CoreMini.h"

#define PUBLIC_TIMMING 1
#include "Timming.h"

#if CONFIG_DEBUG
#define CHECK_TIMMING_IS_INITIALIZED    Assert(Engine_Timming_GetInstance()->m_is_initialized == true, NULL)
#else
#define CHECK_TIMMING_IS_INITIALIZED
#endif


error Engine_Timming_Initialize()
{
    Timming* timming = Engine_Timming_GetInstance();
    timming->m_frame_rate           = 60;
    timming->m_is_limit_frame_rate  = false;

    Engine_Timming_Initialize_Plat();

    timming->m_is_initialized       = true;
    return 0;
}

float Engine_Timming_GetPrevFrameDeltaSeconds()
{
    CHECK_TIMMING_IS_INITIALIZED;
    return Engine_Timming_GetInstance()->m_prev_frame_delta_seconds;
}

float Engine_Timming_GetFrameRate()
{
    CHECK_TIMMING_IS_INITIALIZED;
    return Engine_Timming_GetInstance()->m_frame_rate;
}

error Engine_Timming_SetFrameRate(float fps)
{
    CHECK_TIMMING_IS_INITIALIZED;
    Engine_Timming_GetInstance()->m_frame_rate = fps;
    return Engine_Timming_SetFrameRate_Plat(fps);
}

error Engine_Timming_SetFrameRateLimit(bool is_limit)
{
    CHECK_TIMMING_IS_INITIALIZED;
    return Engine_Timming_GetInstance()->m_is_limit_frame_rate = is_limit;
}

bool Engine_Timming_IslimitFrameRate()
{
    CHECK_TIMMING_IS_INITIALIZED;
    return Engine_Timming_GetInstance()->m_is_limit_frame_rate;
}

