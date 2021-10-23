#include "CoreMini.h"

#if PLATFORM_WIN32

#include "MemoryManager.h"

#include "Thread.h"
#include "String.h"

#include "Platform.h"


static DWORD WINAPI Thread_Run_Function(LPVOID lpParam)
{
    Thread* thread = lpParam;
    Thread_Run_CB_Get(thread)(thread, Thread_Run_Data_Get(thread));

    return 0;
}

void* Thread_PlatformData_Create(Thread* thread, const strcrc* local_name)
{
    const HANDLE thread_handle = CreateThread(NULL, 0, Thread_Run_Function, (void*)thread, 0, 0);
    return thread_handle;
}

void Thread_PlatformData_Destroy(Thread* thread, void* platform_data)
{
    TerminateThread(platform_data, 0);
}

void Thread_Suspend_Platform(Thread* thread, void* platform_data, bool is_suspend)
{
    if( is_suspend )
    {
        SuspendThread(platform_data);
    }
    else
    {
        ResumeThread(platform_data);
    }
}

void Thread_This_Sleep_Platform(float seconds)
{
    Sleep((DWORD)(seconds*1000.f));
}

void Thread_This_Sleep_Tick_Platform()
{
    Sleep(1);
}

#endif
