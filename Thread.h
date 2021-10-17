#pragma once

typedef struct Thread Thread;
typedef void (*CB_ThreadFun_Void_Thread_tPtr)(Thread*,tptr);


Thread*                         Thread_Create           (const tchar* local_name, CB_ThreadFun_Void_Thread_tPtr cb_thread_fun_void_thread_tptr, tptr ptr);
void                            Thread_Destroy          (Thread* thread);
CB_ThreadFun_Void_Thread_tPtr   Thread_Run_CB_Get       (const Thread* thread);
tptr                            Thread_Run_Data_Get     (const Thread* thread);

void                            Thread_Suspend          (Thread* thread, bool is_suspend);
bool                            Thread_IsSuspend        (Thread* thread);

void                            Thread_This_Sleep       (float seconds);
void                            Thread_This_Sleep_Tick  ();

void                            Thread_Sleep            (Thread* thread, float seconds);

