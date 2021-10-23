#include "CoreMini.h"

#include "Task.h"

#include "MemoryManager.h"
#include "TaskManager.h"

#include "Queue.h"
#include "String.h"
#include "Thread.h"

typedef struct TaskQueueThread TaskQueueThread;

struct TaskQueueThread
{
    Queue(Task*)*   m_task_queue;
    Thread*         m_thread;
};

static void TaskManager_TaskQueueThread_RunTask(Thread* thread, TaskQueueThread* task_queue_thread);

static TaskQueueThread* TaskQueueThread_Create(const strcrc* local_name)
{
    TaskQueueThread* task_queue_thread  = MemNew(local_name, TaskQueueThread);
    task_queue_thread->m_task_queue     = Queue_Create(local_name, Task*);

    // Must create thread at last, or it will read a uninit struct
    task_queue_thread->m_thread         = Thread_Create(local_name, TaskManager_TaskQueueThread_RunTask, task_queue_thread);

    return task_queue_thread;
}

static void TaskQueueThread_Destroy(TaskQueueThread* task_queue_thread)
{
    Thread_Destroy(task_queue_thread->m_thread);
    Queue_Destroy(task_queue_thread->m_task_queue, Task_Destroy);

    MemDel(task_queue_thread);
}

static Queue(Task*)* TaskQueueThread_TaskQueue_Get(Thread* thread)
{
    Assert(thread != NULL, "");
    TaskQueueThread* task_queue_thread = (TaskQueueThread*)Thread_Run_Data_Get(thread);

    Assert(task_queue_thread != NULL, "");
    return task_queue_thread->m_task_queue;
}

////////////////////////////////////////////////////////////////////////////////
struct TaskManager
{
    strcrc              m_local_name;
    uint32              m_task_queue_thread_job_max;
    TaskQueueThread**   m_task_queue_thread_work;       // max is render
};

#undef TaskManager_Task_Work_Add
#undef TaskManager_Task_Render_Add

static uint32 TaskManager_TaskQueueThread_Render_Index_Get(TaskManager* task_manager)
{
    return task_manager->m_task_queue_thread_job_max-1;
}

TaskManager* TaskManager_Create(const strcrc* local_name)
{
    TaskManager* task_manager = MemNew(local_name, TaskManager);
    StrCrc_Copy(local_name, &task_manager->m_local_name);
    task_manager->m_task_queue_thread_job_max = 0;
    task_manager->m_task_queue_thread_work    = MemNewSize(local_name, sizeof(TaskQueueThread*)*task_manager->m_task_queue_thread_job_max);

    for(uint32 i=0; i<task_manager->m_task_queue_thread_job_max; i++)
    {
        strcrc render_local_name = StrCrc("Render_TaskQueueThread", 0);

        if( i == TaskManager_TaskQueueThread_Render_Index_Get(task_manager) )
        {
            local_name = &render_local_name;
        }
        task_manager->m_task_queue_thread_work[i] = TaskQueueThread_Create(local_name);
    }

    return task_manager;
}

void TaskManager_Destroy(TaskManager* task_manager)
{
    for(uint32 i=0; i<task_manager->m_task_queue_thread_job_max; i++)
    {
        TaskQueueThread_Destroy(task_manager->m_task_queue_thread_work[i]);
    }

    MemDel(task_manager->m_task_queue_thread_work);
    MemDel(task_manager);
}

Thread* TaskManager_Thread_Work_Get(TaskManager* task_manager, uint32 index)
{
    Assert(index >=0, "");
    if( task_manager->m_task_queue_thread_job_max == 0 )
    {
        return NULL;
    }
    Assert(IS_IN_RANGE(index, 0, task_manager->m_task_queue_thread_job_max), "");
    return task_manager->m_task_queue_thread_work[index]->m_thread;
}

static Thread* TaskManager_TaskQueueThread_Render_Get(TaskManager* task_manager)
{
    if( task_manager->m_task_queue_thread_job_max == 0 )
    {
        return NULL;
    }
    uint32 index = TaskManager_TaskQueueThread_Render_Index_Get(task_manager);
    return task_manager->m_task_queue_thread_work[index]->m_thread;
}

Task* TaskManager_Task_Work_Add(TaskManager* task_manager, const strcrc* local_name, Thread* thread, uint32 priority, bool is_auto_destroy, CB_TaskRun_Condition_Bool_Task_tPtr cb_task_run_condition_bool_task_tptr, CB_TaskRun_Void_Task_tPtr cb_task_run_void_task_tptr, CB_TaskEnd_ClearData_Void_tPtr cb_task_end_clear_data_void_tptr, void* task_data)
{
    Assert( cb_task_run_void_task_tptr != NULL, "" );
    if( thread == NULL )
    {
        bool is_can_run = true;
        if( cb_task_run_condition_bool_task_tptr )
        {
            is_can_run = cb_task_run_condition_bool_task_tptr(NULL, task_data);
        }

        if( is_can_run )
        {
            cb_task_run_void_task_tptr(NULL, task_data);
            if( cb_task_end_clear_data_void_tptr )
            {
                cb_task_end_clear_data_void_tptr(task_data);
            }
        }
        return NULL;
    }
    else
    {
        Task* task = Task_Create(local_name, thread, priority, is_auto_destroy, cb_task_run_condition_bool_task_tptr, cb_task_run_void_task_tptr, cb_task_end_clear_data_void_tptr, task_data);

        Queue_Push(Task*, local_name, TaskQueueThread_TaskQueue_Get(thread), task);

        if( is_auto_destroy )
        {
            // Maybe you should not keep the handle if auto free
            // return NULL;
        }

        return task;
    }
}

void TaskManager_Task_Render_Add(TaskManager* task_manager, const strcrc* local_name, CB_TaskRun_Void_Task_tPtr cb_task_run_void_task_tptr, CB_TaskEnd_ClearData_Void_tPtr cb_task_end_clear_data_void_tptr, void* task_data)
{
    Thread* thread = TaskManager_TaskQueueThread_Render_Get(task_manager);
    TaskManager_Task_Work_Add(task_manager, local_name, thread, 0, true, NULL, cb_task_run_void_task_tptr, cb_task_end_clear_data_void_tptr, task_data);
}

static bool TaskQueueThread_Task_Is_Finish(const Task* task, const void* reserve)
{
    return Task_IsFinish(task);
}

static void TaskQueueThread_RunTask(Task* task, const void* reserve)
{
    Task_TryExecute(task);
}

static void TaskManager_TaskQueueThread_RunTask(Thread* thread, TaskQueueThread* task_queue_thread)
{
    for(;;)
    {
        if (!Queue_IsEmpty(task_queue_thread->m_task_queue))
        {
            // We need lock m_task_queue
            Queue_ForEach(task_queue_thread->m_task_queue, TaskQueueThread_RunTask, NULL);
            Queue_RemoveFindAll(task_queue_thread->m_task_queue, TaskQueueThread_Task_Is_Finish, NULL, Task_TryDestory);
        }
        else
        {
            Thread_Sleep_This_Tick();
        }
    }

    Assert(false, "");
}
