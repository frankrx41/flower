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
    LARGE_INTEGER   m_CounterOld;
    INT64           m_FrameCnt;
    BYTE            m_Rand8;

    INT64           m_OneMSFrameCnt;
    UINT32          m_StartTime;
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

    LARGE_INTEGER counter_old;
    success = QueryPerformanceCounter(&counter_old);
    Assert( success == true, "" );

    timming_platform_data->m_CounterOld   = counter_old;
    timming_platform_data->m_FrameCnt     = frame_count;
    timming_platform_data->m_OneMSFrameCnt = cpu_frequency / 1000;
}

float TimmingManager_TrimSpeed_Plat(TimmingManager* timming_manager, TimmingPlatformData* timming_platform_data)
{
    LARGE_INTEGER counter_cur;
    INT64 curt64,oldt64;

    QueryPerformanceCounter(&counter_cur);
    curt64 = counter_cur.QuadPart;

    oldt64 = timming_platform_data->m_CounterOld.QuadPart;

    INT64 passed_frame_count = curt64 - oldt64;

    while( passed_frame_count < timming_platform_data->m_FrameCnt )
    {
        if(timming_platform_data->m_FrameCnt-passed_frame_count > timming_platform_data->m_OneMSFrameCnt*2)
            Sleep(1);
        QueryPerformanceCounter(&counter_cur);
        curt64 = counter_cur.QuadPart;
        passed_frame_count = curt64 - oldt64;
    }

    timming_platform_data->m_CounterOld   = counter_cur;

    return (curt64 - oldt64) / (timming_platform_data->m_FrameCnt * TimmingManager_GetFrameRate(timming_manager));
}



