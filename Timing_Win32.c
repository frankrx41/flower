// #define PLATFORM_WIN32      1
#include "CoreMini.h"

#include "TimingManager.h"
#include "MemoryManager.h"

#define PLATFORM_WIN32      1
#include "Platform.h"
#include <Windows.h>

typedef struct TimingPlatformData TimingPlatformData;

struct TimingPlatformData
{
    int64   m_last_cpu_count;
    int64   m_next_cpu_count;
    int64   m_counts_in_one_frame;
};

tptr TimingManager_PlatformData_Create(TimingManager* timing_manager, const tchar* local_name)
{
    return MemNew(local_name, TimingPlatformData);
}

void TimingManager_PlatformData_Destroy(TimingManager* timing_manager, TimingPlatformData* platform_data)
{
    MemDel(platform_data);
}


void TimingManager_SetFrameRate_Plat(TimingManager* timing_manager, TimingPlatformData* platform_data, float rate)
{
    int64 cpu_frequency;
    bool success;

    // check for high precision timer
    {
        LARGE_INTEGER cpu_frequency_large_int;
        success = QueryPerformanceFrequency(&cpu_frequency_large_int);
        Assert( success == true, "" );
        cpu_frequency = cpu_frequency_large_int.QuadPart;
    }


    const int64 frame_count = (int64)(cpu_frequency / rate);

    LARGE_INTEGER current_cpu_count;
    success = QueryPerformanceCounter(&current_cpu_count);
    Assert( success == true, "" );

    platform_data->m_last_cpu_count         = current_cpu_count.QuadPart;
    platform_data->m_counts_in_one_frame    = frame_count;
    platform_data->m_next_cpu_count         = platform_data->m_last_cpu_count + frame_count;
}

float TimingManager_TrimSpeed_Plat(TimingManager* timing_manager, TimingPlatformData* platform_data)
{
    int64 current_cpu_count;
    const int64 last_cpu_count = platform_data->m_last_cpu_count;
    const int64 next_cpu_count = platform_data->m_next_cpu_count;

    for(;;)
    {
        LARGE_INTEGER current_cpu_count_large_int;

        QueryPerformanceCounter(&current_cpu_count_large_int);
        current_cpu_count = current_cpu_count_large_int.QuadPart;
        
        if( current_cpu_count >= next_cpu_count )
        {
            break;
        }

        Sleep(1);
    }

    platform_data->m_next_cpu_count = platform_data->m_next_cpu_count + platform_data->m_counts_in_one_frame;
    platform_data->m_last_cpu_count = current_cpu_count;

    return (current_cpu_count - last_cpu_count) / (platform_data->m_counts_in_one_frame * TimingManager_GetFrameRate(timing_manager));
}



