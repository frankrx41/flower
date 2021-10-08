#include "CoreMini.h"

#include "MemoryManager.h"
#include "TimmingManager.h"


typedef struct TimmingPlatformData TimmingPlatformData;

struct TimmingManager
{
    bool    m_is_initialized;
    bool    m_is_limit_frame_rate;
    tptr    m_platform_data;

    float   m_prev_frame_delta_seconds;
    float   m_frame_rate;
};

float   TimmingManager_TrimSpeed_Plat       (TimmingManager* timming_manager, TimmingPlatformData* timming_platform_data);
void    TimmingManager_SetFrameRate_Plat    (TimmingPlatformData* timming_manager, float fps);
tptr    TimmingManager_PlatformData_Create  (const tchar* local_name);


TimmingManager* TimmingManager_Create(const tchar* local_name)
{
    TimmingManager* timming_manager         = MemNew(local_name, TimmingManager);
    timming_manager->m_frame_rate           = 60;
    timming_manager->m_is_limit_frame_rate  = false;
    timming_manager->m_platform_data        = TimmingManager_PlatformData_Create(local_name);
    timming_manager->m_is_initialized       = true;

    return timming_manager;
}

float TimmingManager_GetPrevFrameDeltaSeconds(TimmingManager* timming_manager)
{
    Assert(timming_manager->m_is_initialized == true, NULL);
    return timming_manager->m_prev_frame_delta_seconds;
}

float TimmingManager_GetFrameRate(TimmingManager* timming_manager)
{
    Assert(timming_manager->m_is_initialized == true, NULL);
    return timming_manager->m_frame_rate;
}

void TimmingManager_SetFrameRate(TimmingManager* timming_manager, float fps)
{
    Assert(timming_manager->m_is_initialized == true, NULL);
    timming_manager->m_frame_rate = fps;
    TimmingManager_SetFrameRate_Plat(timming_manager->m_platform_data, fps);
}

void TimmingManager_SetFrameRateLimit(TimmingManager* timming_manager, bool is_limit)
{
    Assert(timming_manager->m_is_initialized == true, NULL);
    timming_manager->m_is_limit_frame_rate = is_limit;
}

bool TimmingManager_IslimitFrameRate(TimmingManager* timming_manager)
{
    Assert(timming_manager->m_is_initialized == true, NULL);
    return timming_manager->m_is_limit_frame_rate;
}

void TimmingManager_TrimSpeed(TimmingManager* timming_manager)
{
    timming_manager->m_prev_frame_delta_seconds = TimmingManager_TrimSpeed_Plat(timming_manager, timming_manager->m_platform_data);
    // TODO: Add fps profile
    // Log(0, "%f\n", timming_manager->m_prev_frame_delta_seconds);
    // Assert(timming_manager->m_prev_frame_delta_seconds < 1.f, "Fps is too low!");
}
