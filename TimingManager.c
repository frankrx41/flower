#include "CoreMini.h"

#include "Event.h"
#include "KeyId.h"

#include "MemoryManager.h"
#include "TimingManager.h"
#include "EventManager.h"

#include "String.h"


struct TimingManager
{
    bool    m_is_initialized;
    bool    m_is_limit_frame_rate;
    tptr    m_platform_data;
    String* m_local_name;

    float   m_prev_frame_delta_seconds;
    float   m_frame_rate;
};

tptr    TimingManager_PlatformData_Create  (TimingManager* timing_manager, const tchar* local_name);
void    TimingManager_PlatformData_Destroy (TimingManager* timing_manager, tptr platform_data);
float   TimingManager_TrimSpeed_Plat       (TimingManager* timing_manager, tptr platform_data);
void    TimingManager_SetFrameRate_Plat    (TimingManager* timing_manager, tptr platform_data, float fps);


TimingManager* TimingManager_Create(const tchar* local_name)
{
    TimingManager* timing_manager           = MemNew(local_name, TimingManager);
    timing_manager->m_frame_rate            = 60;
    timing_manager->m_is_limit_frame_rate   = false;
    timing_manager->m_platform_data         = TimingManager_PlatformData_Create(timing_manager, local_name);
    timing_manager->m_is_initialized        = true;
    timing_manager->m_local_name            = String_New(local_name, local_name, true);

    TimingManager_SetFrameRate(timing_manager, 60);

    return timing_manager;
}

void TimingManager_Destroy(TimingManager* timing_manager)
{
    TimingManager_PlatformData_Destroy(timing_manager, timing_manager->m_platform_data);

    String_Del(timing_manager->m_local_name);
    MemDel(timing_manager);
}

float TimingManager_GetPrevFrameDeltaSeconds(TimingManager* timing_manager)
{
    Assert(timing_manager->m_is_initialized == true, NULL);
    return timing_manager->m_prev_frame_delta_seconds;
}

float TimingManager_GetFrameRate(TimingManager* timing_manager)
{
    Assert(timing_manager->m_is_initialized == true, NULL);
    return timing_manager->m_frame_rate;
}

void TimingManager_SetFrameRate(TimingManager* timing_manager, float fps)
{
    Assert(timing_manager->m_is_initialized == true, NULL);
    timing_manager->m_frame_rate = fps;
    TimingManager_SetFrameRate_Plat(timing_manager, timing_manager->m_platform_data, fps);
}

void TimingManager_SetFrameRateLimit(TimingManager* timing_manager, bool is_limit)
{
    Assert(timing_manager->m_is_initialized == true, NULL);
    timing_manager->m_is_limit_frame_rate = is_limit;
}

bool TimingManager_IsLimitFrameRate(TimingManager* timing_manager)
{
    Assert(timing_manager->m_is_initialized == true, NULL);
    return timing_manager->m_is_limit_frame_rate;
}

void TimingManager_TrimSpeed(TimingManager* timing_manager)
{
    timing_manager->m_prev_frame_delta_seconds = TimingManager_TrimSpeed_Plat(timing_manager, timing_manager->m_platform_data);

    // EventManager_SendEvent(Event_Tick, String_CStr(timing_manager->m_local_name), timing_manager->m_prev_frame_delta_seconds);
    EventManager_SendEvent(Event_Tick, timing_manager->m_prev_frame_delta_seconds);
    // TODO: Add fps profile
    // Log(0, "%f\n", timing_manager->m_prev_frame_delta_seconds);
    // Assert(timing_manager->m_prev_frame_delta_seconds < 1.f, "Fps is too low!");
}
