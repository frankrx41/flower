#include "CoreMini.h"

#include "Event.h"
#include "KeyId.h"

#include "MemoryManager.h"
#include "TimingManager.h"
#include "EventManager.h"

#include "String.h"
#include "Thread.h"


struct TimingManager
{
    bool    m_is_initialized;
    bool    m_is_limit_frame_rate;
    tptr    m_platform_data;
    String* m_local_name;

    int64   m_last_cpu_tick;
    int64   m_next_cpu_tick;
    int64   m_ticks_in_one_frame;

    float   m_prev_frame_delta_seconds;
    float   m_frame_rate;
};


tptr    TimingManager_PlatformData_Create  (TimingManager* timing_manager, const tchar* local_name);
void    TimingManager_PlatformData_Destroy (TimingManager* timing_manager, tptr platform_data);

uint64  TimingManager_Cpu_Tick_Get_Plat     (TimingManager* timing_manager, tptr platform_data);
uint64  TimingManager_Cpu_Frequency_Get_Plat(TimingManager* timing_manager, tptr platform_data);

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

void TimingManager_SetFrameRate(TimingManager* timing_manager, float rate)
{
    Assert(timing_manager->m_is_initialized == true, NULL);
    timing_manager->m_frame_rate = rate;

    // check for high precision timer
    int64 cpu_frequency = TimingManager_Cpu_Frequency_Get_Plat(timing_manager, timing_manager->m_platform_data);

    const int64 frame_count = (int64)(cpu_frequency / rate);
    timing_manager->m_last_cpu_tick         = TimingManager_Cpu_Tick_Get_Plat(timing_manager, timing_manager->m_platform_data);
    timing_manager->m_ticks_in_one_frame    = frame_count;
    timing_manager->m_next_cpu_tick         = timing_manager->m_last_cpu_tick + frame_count;

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
    int64 current_cpu_tick;
    const int64 last_cpu_tick = timing_manager->m_last_cpu_tick;
    const int64 next_cpu_tick = timing_manager->m_next_cpu_tick;

    for(;;)
    {

        current_cpu_tick = TimingManager_Cpu_Tick_Get_Plat(timing_manager, timing_manager->m_platform_data);
        
        if( current_cpu_tick >= next_cpu_tick )
        {
            break;
        }

        Thread_Sleep(NULL, 0.001f);
    }

    timing_manager->m_next_cpu_tick = timing_manager->m_next_cpu_tick + timing_manager->m_ticks_in_one_frame;
    timing_manager->m_last_cpu_tick = current_cpu_tick;

    timing_manager->m_prev_frame_delta_seconds = (current_cpu_tick - last_cpu_tick) / (timing_manager->m_ticks_in_one_frame * TimingManager_GetFrameRate(timing_manager));



    // EventManager_SendEvent(Event_Tick, String_CStr(timing_manager->m_local_name), timing_manager->m_prev_frame_delta_seconds);
    SendEvent_Tick(Event_Tick, timing_manager->m_prev_frame_delta_seconds);
    // TODO: Add fps profile
    // Log(0, "%f\n", timing_manager->m_prev_frame_delta_seconds);
    // Assert(timing_manager->m_prev_frame_delta_seconds < 1.f, "Fps is too low!");
}

float TimingManager_Cpu_Seconds_Get(TimingManager* timing_manager)
{
    return TimingManager_Cpu_Tick_Get_Plat(timing_manager, timing_manager->m_platform_data) / (timing_manager->m_ticks_in_one_frame * timing_manager->m_frame_rate);
}
