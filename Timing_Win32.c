#include "CoreMini.h"

#if PLATFORM_WIN32

#include "TimingManager.h"
#include "MemoryManager.h"

#include "Platform.h"


void* TimingManager_PlatformData_Create(TimingManager* timing_manager, const tchar* local_name)
{
    return NULL;
}

void TimingManager_PlatformData_Destroy(TimingManager* timing_manager, void* platform_data)
{
}

uint64 TimingManager_Cpu_Frequency_Get_Platform(TimingManager* timing_manager, void* platform_data)
{
    bool success = false;

    LARGE_INTEGER cpu_frequency_large_int;
    success = QueryPerformanceFrequency(&cpu_frequency_large_int);
    Assert( success == true, "" );
    return cpu_frequency_large_int.QuadPart;
}

uint64 TimingManager_Cpu_Tick_Get_Platform(TimingManager* timing_manager, void* platform_data)
{
    bool success = false;
    LARGE_INTEGER current_cpu_count_large_int;
    success = QueryPerformanceCounter(&current_cpu_count_large_int);
    Assert( success == true, "" );

    return current_cpu_count_large_int.QuadPart;
}

#endif
