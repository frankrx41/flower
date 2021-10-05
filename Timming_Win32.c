#define PLATFORM_WIN32      1
#include "CoreMini.h"

#define PUBLIC_TIMMING      1
#include "Timming.h"
#include "Memory.h"

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

TimmingPlatformData* Engine_Timming_GetInstance_Plat()
{
    return Engine_Timming_GetInstance()->m_platform_data;
}

void Engine_Timming_Initialize_Plat()
{
    Engine_Timming_GetInstance()->m_platform_data = MemNew("Timming", TimmingPlatformData);
}

void Engine_Timming_SetFrameRate_Plat(float rate)
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

    Engine_Timming_GetInstance_Plat()->m_CounterOld   = counter_old;
    Engine_Timming_GetInstance_Plat()->m_FrameCnt     = frame_count;
    Engine_Timming_GetInstance()->m_frame_rate    = rate;
    Engine_Timming_GetInstance_Plat()->m_OneMSFrameCnt = cpu_frequency / 1000;
}

void Engine_Timming_TrimSpeed_Plat()
{
    LARGE_INTEGER counter_cur;
    INT64 curt64,oldt64;

    QueryPerformanceCounter(&counter_cur);
    curt64 = counter_cur.QuadPart;

    oldt64 = Engine_Timming_GetInstance_Plat()->m_CounterOld.QuadPart;

    INT64 passed_frame_count = curt64 - oldt64;

    while( passed_frame_count < Engine_Timming_GetInstance_Plat()->m_FrameCnt )
    {
        if(Engine_Timming_GetInstance_Plat()->m_FrameCnt-passed_frame_count > Engine_Timming_GetInstance_Plat()->m_OneMSFrameCnt*2)
            Sleep(1);
        QueryPerformanceCounter(&counter_cur);
        curt64 = counter_cur.QuadPart;
        passed_frame_count = curt64 - oldt64;
    }

    Engine_Timming_GetInstance()->m_prev_frame_delta_seconds  = (curt64 - oldt64) / (Engine_Timming_GetInstance_Plat()->m_FrameCnt * Engine_Timming_GetInstance()->m_frame_rate);
    Engine_Timming_GetInstance_Plat()->m_CounterOld   = counter_cur;
}



