#include "CoreMini.h"

#include "MemoryManager.h"
#include "TimmingManager.h"


struct TimmingManager
{
    bool    m_is_initialized;
    bool    m_is_limit_frame_rate;
    ptr32   m_platform_data;

    float   m_prev_frame_delta_seconds;
    float   m_frame_rate;
};

ptr32   TimmingManager_PlatformData_Create  (TimmingManager* timming_manager, const tchar* local_name);
void    TimmingManager_PlatformData_Destroy (TimmingManager* timming_manager, ptr32 ptr);
float   TimmingManager_TrimSpeed_Plat       (TimmingManager* timming_manager, ptr32 ptr);
void    TimmingManager_SetFrameRate_Plat    (TimmingManager* timming_manager, ptr32 ptr, float fps);


TimmingManager* TimmingManager_Create(const tchar* local_name)
{
    TimmingManager* timming_manager         = MemNew(local_name, TimmingManager);
    timming_manager->m_frame_rate           = 60;
    timming_manager->m_is_limit_frame_rate  = false;
    timming_manager->m_platform_data        = TimmingManager_PlatformData_Create(timming_manager, local_name);
    timming_manager->m_is_initialized       = true;

    TimmingManager_SetFrameRate(timming_manager, 60);

    return timming_manager;
}

void TimmingManager_Destroy(TimmingManager* timming_manager)
{
    TimmingManager_PlatformData_Destroy(timming_manager, timming_manager->m_platform_data);

    MemDel(timming_manager);
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
    TimmingManager_SetFrameRate_Plat(timming_manager, timming_manager->m_platform_data, fps);
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
