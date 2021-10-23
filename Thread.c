#include "CoreMini.h"

#include "MemoryManager.h"

#include "String.h"
#include "Thread.h"


struct Thread
{
    CB_ThreadRun_Void_Thread_tPtr   m_cb_thread_run_void_thread_tptr;
    void*                           m_thread_input_data;
    strcrc                          m_local_name;
    bool                            m_is_suspend;

    TODO("Implement any thread sleep")
    float                           m_need_sleep_seconds;

    void*                           m_thread_platform_data;
};

void*   Thread_PlatformData_Create      (Thread* thread, const strcrc* local_name);
void    Thread_PlatformData_Destroy     (Thread* thread, void* platform_data);

void    Thread_Suspend_Platform         (Thread* thread, void* platform_data, bool is_suspend);
void    Thread_This_Sleep_Platform      (float seconds);
void    Thread_This_Sleep_Tick_Platform ();

Thread* Thread_Create(const strcrc* local_name, CB_ThreadRun_Void_Thread_tPtr cb_thread_run_void_thread_tptr, void* ptr)
{
    Thread* thread = MemNew(local_name, Thread);
    thread->m_cb_thread_run_void_thread_tptr    = cb_thread_run_void_thread_tptr;
    thread->m_thread_input_data                 = ptr;
    StrCrc_Copy(local_name, &thread->m_local_name);
    thread->m_is_suspend                        = false;
    thread->m_thread_platform_data              = Thread_PlatformData_Create(thread, local_name);
    return thread;
}

void Thread_Destroy(Thread* thread)
{
    Thread_PlatformData_Destroy(thread, thread->m_thread_platform_data);
    MemDel(thread);
}


CB_ThreadRun_Void_Thread_tPtr Thread_Run_CB_Get(const Thread* thread)
{
    return thread->m_cb_thread_run_void_thread_tptr;
}

void* Thread_Run_Data_Get(const Thread* thread)
{
    return thread->m_thread_input_data;
}

void Thread_Suspend(Thread* thread, bool is_suspend)
{
    thread->m_is_suspend = is_suspend;
    Thread_Suspend_Platform(thread, thread->m_thread_platform_data, is_suspend);
}

bool Thread_IsSuspend(Thread* thread)
{
    return thread->m_is_suspend;
}

void Thread_Sleep_This(float seconds)
{
    Thread_This_Sleep_Platform(seconds);
}

void Thread_Sleep_This_Tick()
{
    Thread_This_Sleep_Tick_Platform();
}

void Thread_Sleep(Thread* thread, float seconds)
{
    if( thread == NULL )
    {
        Thread_Sleep_This(seconds);
    }
    else
    {
        Assert(false, "Only support sleep current thread!");
        thread->m_need_sleep_seconds += seconds;
        Assert(thread->m_need_sleep_seconds >= 0, "");
    }
}

const strcrc* Thread_LocalName_Get(Thread* thread)
{
    return &thread->m_local_name;
}
