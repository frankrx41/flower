#pragma once

typedef struct Thread Thread;
typedef void (*CB_ThreadRun_Void_Thread_tPtr)(Thread*, void*);


Thread*                         Thread_Create           (const strcrc* local_name, CB_ThreadRun_Void_Thread_tPtr cb_thread_run_void_thread_tptr, const void* ptr);
void                            Thread_Destroy          (Thread* thread);
CB_ThreadRun_Void_Thread_tPtr   Thread_Run_CB_Get       (const Thread* thread);
void*                           Thread_Run_Data_Get     (const Thread* thread);

void                            Thread_Suspend          (Thread* thread, bool is_suspend);
bool                            Thread_IsSuspend        (Thread* thread);

void                            Thread_Sleep_This       (float seconds);
void                            Thread_Sleep_This_Tick  ();

void                            Thread_Sleep            (Thread* thread, float seconds);

const strcrc*                   Thread_LocalName_Get    (Thread* thread);
