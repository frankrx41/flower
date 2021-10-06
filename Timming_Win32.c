// #define PLATFORM_WIN32      1
#include "CoreMini.h"

#include "TimmingManager.h"
#include "MemoryManager.h"

#define PLATFORM_WIN32      1
#include "Platform.h"
#include <Windows.h>

typedef struct TimmingPlatformData TimmingPlatformData;

struct TimmingPlatformData
{
    int64   m_current_cpu_count;
    int64   m_counts_in_one_frame;
};

tptr TimmingManager_PlatformData_Create(const tchar* local_name)
{
    return MemNew(local_name, TimmingPlatformData);
}

void TimmingManager_SetFrameRate_Plat(TimmingPlatformData* timming_platform_data, float rate)
{
    INT64 cpu_frequency;
    BOOL success;

    // check for high precision timer
    {
        LARGE_INTEGER cpu_frequency_large_int;
        success = QueryPerformanceFrequency(&cpu_frequency_large_int);
        Assert( success == true, "" );
        cpu_frequency = cpu_frequency_large_int.QuadPart;
    }

    
    INT64 frame_count = (INT64)(cpu_frequency / rate);

    LARGE_INTEGER current_cpu_count;
    success = QueryPerformanceCounter(&current_cpu_count);
    Assert( success == true, "" );

    timming_platform_data->m_current_cpu_count          = current_cpu_count.QuadPart;
    timming_platform_data->m_counts_in_one_frame        = frame_count;
}

float TimmingManager_TrimSpeed_Plat(TimmingManager* timming_manager, TimmingPlatformData* timming_platform_data)
{
    int64 passed_frame_count = 0;
    int64 current_cpu_count;
    int64 last_cpu_count = timming_platform_data->m_current_cpu_count;

    do
    {
        Sleep(1);

        LARGE_INTEGER current_cpu_count_large_int;

        QueryPerformanceCounter(&current_cpu_count_large_int);
        current_cpu_count = current_cpu_count_large_int.QuadPart;
        passed_frame_count = current_cpu_count - timming_platform_data->m_current_cpu_count;

    }while(passed_frame_count < timming_platform_data->m_counts_in_one_frame);


    timming_platform_data->m_current_cpu_count += timming_platform_data->m_counts_in_one_frame;

    return (current_cpu_count - last_cpu_count) / (timming_platform_data->m_counts_in_one_frame * TimmingManager_GetFrameRate(timming_manager));
}



