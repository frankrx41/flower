#include "CoreMini.h"

#if PLATFORM_WIN32

#include "MemoryManager.h"

#include "Thread.h"
#include "String.h"

#include <Windows.h>

static DWORD WINAPI Thread_Run_Function(LPVOID lpParam)
{
    Thread* thread = lpParam;
    Thread_Run_CB_Get(thread)(thread, Thread_Run_Data_Get(thread));

    return 0;
}

tptr Thread_Create_Plat(Thread* thread, const tchar* local_name)
{
    const HANDLE thread_handle = CreateThread(NULL, 0, Thread_Run_Function, (void*)thread, 0, 0);
    return thread_handle;
}

void Thread_Destroy_Plat(Thread* thread, tptr platform_data)
{
    TerminateThread(platform_data, 0);
}

void Thread_Suspend_Plat(Thread* thread, tptr platform_data, bool is_suspend)
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

void Thread_This_Sleep_Plat(float seconds)
{
    Sleep((DWORD)(seconds*1000.f));
}

void Thread_This_Sleep_Tick_Plat()
{
    Sleep(1);
}

#endif
