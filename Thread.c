#include "CoreMini.h"

#include "MemoryManager.h"

#include "String.h"
#include "Thread.h"


typedef struct Thread Thread;

struct Thread
{
    CB_ThreadFun_Void_Thread_tPtr   m_cb_thread_fun_void_thread_tptr;
    tptr                            m_thread_input_data;
    tptr                            m_thread_platform_data;
    String*                         m_local_name;
    bool                            m_is_suspend;
    float                           m_need_sleep_seconds;
};

tptr    Thread_Create_Plat          (Thread* thread, const tchar* local_name);
void    Thread_Suspend_Plat         (Thread* thread, tptr platform_data, bool is_suspend);
void    Thread_Destroy_Plat         (Thread* thread, tptr platform_data);
void    Thread_This_Sleep_Plat      (float seconds);
void    Thread_This_Sleep_Tick_Plat ();

Thread* Thread_Create(const tchar* local_name, CB_ThreadFun_Void_Thread_tPtr cb_thread_fun_void_thread_tptr, tptr ptr)
{
    Thread* thread = MemNew(local_name, Thread);
    thread->m_cb_thread_fun_void_thread_tptr    = cb_thread_fun_void_thread_tptr;
    thread->m_thread_input_data                 = ptr;
    thread->m_local_name                        = String_New(local_name, local_name, true);
    thread->m_is_suspend                        = false;
    thread->m_thread_platform_data              = Thread_Create_Plat(thread, local_name);
    return thread;
}

void Thread_Destroy(Thread* thread)
{
    String_Del(thread->m_local_name);
    Thread_Destroy_Plat(thread, thread->m_thread_platform_data);
    MemDel(thread);
}


CB_ThreadFun_Void_Thread_tPtr Thread_Run_CB_Get(const Thread* thread)
{
    return thread->m_cb_thread_fun_void_thread_tptr;
}

tptr Thread_Run_Data_Get(const Thread* thread)
{
    return thread->m_thread_input_data;
}

void Thread_Suspend(Thread* thread, bool is_suspend)
{
    thread->m_is_suspend = is_suspend;
    Thread_Suspend_Plat(thread, thread->m_thread_platform_data, is_suspend);
}

bool Thread_IsSuspend(Thread* thread)
{
    return thread->m_is_suspend;
}

void Thread_Sleep_This(float seconds)
{
    Thread_This_Sleep_Plat(seconds);
}

void Thread_Sleep_This_Tick()
{
    Thread_This_Sleep_Tick_Plat();
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

const tchar* Thread_LocalName_Get(Thread* thread)
{
    return String_CStr(thread->m_local_name);
}
