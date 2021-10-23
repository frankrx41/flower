#include "CoreMini.h"

#include "Event.h"
#include "KeyId.h"

#include "MemoryManager.h"
#include "TimingManager.h"
#include "EventManager.h"

#include "String.h"
#include "Thread.h"
#include "tData.h"
#include "Stat.h"

static statcycle stat_cpu_seconds;
static statcycle stat_fps;

void TimingManager_Stat()
{
    Log(4, "Fps Profile:\n");
    Log(4, "=====================================================\n");

    Log(4, "%-20s: %7.2f  %7.2f  %7.2f\n", "Cpu seconds", stat_cpu_seconds.m_data_ave.m_float, stat_cpu_seconds.m_data_min.m_float, stat_cpu_seconds.m_data_max.m_float);
    Log(4, "%-20s: %7.2f  %7.2f  %7.2f\n", "Fps", stat_fps.m_data_ave.m_float, stat_fps.m_data_min.m_float, stat_fps.m_data_max.m_float);
    Log(4, "=====================================================\n");
}

////////////////////////////////////////////////////////////////////////////////
struct TimingManager
{
    bool    m_is_initialized;
    bool    m_is_limit_frame_rate;
    void*   m_platform_data;
    strcrc  m_local_name;

    int64   m_last_cpu_tick;
    int64   m_next_cpu_tick;
    int64   m_ticks_in_one_frame;

    float   m_prev_frame_delta_seconds;
    float   m_frame_rate;
    float   m_idle_seconds;
};


void*    TimingManager_PlatformData_Create  (TimingManager* timing_manager, const strcrc* local_name);
void    TimingManager_PlatformData_Destroy (TimingManager* timing_manager, void* platform_data);

uint64  TimingManager_Cpu_Tick_Get_Plat     (TimingManager* timing_manager, void* platform_data);
uint64  TimingManager_Cpu_Frequency_Get_Plat(TimingManager* timing_manager, void* platform_data);

TimingManager* TimingManager_Create(const strcrc* local_name)
{
    TimingManager* timing_manager           = MemNew(local_name, TimingManager);
    timing_manager->m_frame_rate            = 60;
    timing_manager->m_is_limit_frame_rate   = false;
    timing_manager->m_platform_data         = TimingManager_PlatformData_Create(timing_manager, local_name);
    timing_manager->m_is_initialized        = true;
    StrCrc_Copy(local_name, &timing_manager->m_local_name);
    timing_manager->m_idle_seconds          = 0;

    TimingManager_SetFrameRate(timing_manager, 60);

    return timing_manager;
}

void TimingManager_Destroy(TimingManager* timing_manager)
{
    TimingManager_PlatformData_Destroy(timing_manager, timing_manager->m_platform_data);

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
    const int64 last_cpu_tick = timing_manager->m_last_cpu_tick;
    const int64 next_cpu_tick = timing_manager->m_next_cpu_tick;
    int64 current_cpu_tick = TimingManager_Cpu_Tick_Get_Plat(timing_manager, timing_manager->m_platform_data);

    {
        float seconds = (current_cpu_tick - last_cpu_tick) / (timing_manager->m_ticks_in_one_frame * TimingManager_GetFrameRate(timing_manager));
        StatCycle_Float_Add(&stat_cpu_seconds, seconds);
    }

    for(;;)
    {
        current_cpu_tick = TimingManager_Cpu_Tick_Get_Plat(timing_manager, timing_manager->m_platform_data);
        
        if( current_cpu_tick >= next_cpu_tick )
        {
            break;
        }

        Thread_Sleep_This_Tick();
    }

    timing_manager->m_next_cpu_tick = timing_manager->m_next_cpu_tick + timing_manager->m_ticks_in_one_frame;
    timing_manager->m_last_cpu_tick = current_cpu_tick;

    timing_manager->m_prev_frame_delta_seconds = (current_cpu_tick - last_cpu_tick) / (timing_manager->m_ticks_in_one_frame * TimingManager_GetFrameRate(timing_manager));
    timing_manager->m_idle_seconds += timing_manager->m_prev_frame_delta_seconds;

    {
        StatCycle_Float_Add(&stat_fps, 1.f / timing_manager->m_prev_frame_delta_seconds);
    }

    EventManager_SendEvent_Update(EventManager_GetInstance(), Event_Scene_Tick, timing_manager->m_prev_frame_delta_seconds);
    EventManager_SendEvent_Update(EventManager_GetInstance(), Event_Scene_Update_Anime, timing_manager->m_prev_frame_delta_seconds);
    EventManager_SendEvent_Update(EventManager_GetInstance(), Event_Scene_Update_Physics, timing_manager->m_prev_frame_delta_seconds);
    if( timing_manager->m_idle_seconds > 8 )
    {
        EventManager_SendEvent_Update(EventManager_GetInstance(), Event_Scene_Idle, timing_manager->m_idle_seconds);
    }
}

float TimingManager_Cpu_Seconds_Get(TimingManager* timing_manager)
{
    return TimingManager_Cpu_Tick_Get_Plat(timing_manager, timing_manager->m_platform_data) / (timing_manager->m_ticks_in_one_frame * timing_manager->m_frame_rate);
}

void TimingManager_ResetIdleSecond(TimingManager* timing_manager)
{
    timing_manager->m_idle_seconds = 0;
}
