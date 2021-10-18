#include "CoreMini.h"

#include "Task.h"

#include "MemoryManager.h"
#include "TaskManager.h"

#include "Queue.h"
#include "String.h"
#include "Thread.h"

typedef struct TaskThread TaskThread;

struct TaskThread
{
    Thread*         m_thread;
    Queue(Task*)*   m_task_queue;
};

static void TaskManager_Thread_RunTask(Thread* thread, TaskThread* task_thread);

TaskThread* TaskThread_Create(const tchar* local_name)
{
    TaskThread* task_thread = MemNew(local_name, TaskThread);
    task_thread->m_task_queue   = Queue_Create(local_name, Task*);

    // Must create thread at last, or it will read a uninit struct
    task_thread->m_thread       = Thread_Create(local_name, TaskManager_Thread_RunTask, task_thread);

    return task_thread;
}

void TaskThread_Destroy(TaskThread* task_thread)
{
    Thread_Destroy(task_thread->m_thread);
    Queue_Destroy(task_thread->m_task_queue, Task_Destroy);

    MemDel(task_thread);
}

////////////////////////////////////////////////////////////////////////////////
struct TaskManager
{
    String*         m_local_name;
    uint32          m_task_thread_job_max;
    TaskThread**    m_task_thread_work; // max is render
};

#undef TaskManager_Task_Work_Add
#undef TaskManager_Task_Render_Add

static uint32 TaskManager_TaskThread_Render_Index_Get(TaskManager* task_manager)
{
    return task_manager->m_task_thread_job_max-1;
}

TaskManager* TaskManager_Create(const tchar* local_name)
{
    TaskManager* task_manager = MemNew(local_name, TaskManager);

    task_manager->m_local_name      = String_New(local_name, local_name, true);
    task_manager->m_task_thread_job_max = 0;
    task_manager->m_task_thread_work    = MemNewSize(local_name, sizeof(TaskThread*)*task_manager->m_task_thread_job_max);

    for(uint32 i=0; i<task_manager->m_task_thread_job_max; i++)
    {
        const tchar* str = local_name;
        if( i == TaskManager_TaskThread_Render_Index_Get(task_manager) )
        {
            str = "Render_TaskThread";
        }
        task_manager->m_task_thread_work[i] = TaskThread_Create(str);
    }

    return task_manager;
}

void TaskManager_Destroy(TaskManager* task_manager)
{
    String_Del(task_manager->m_local_name);
    for(uint32 i=0; i<task_manager->m_task_thread_job_max; i++)
    {
        TaskThread_Destroy(task_manager->m_task_thread_work[i]);
    }

    MemDel(task_manager->m_task_thread_work);
    MemDel(task_manager);
}

TaskThread* TaskManager_TaskThread_Work_Get(TaskManager* task_manager, uint32 index)
{
    Assert(index >=0, "");
    if( task_manager->m_task_thread_job_max == 0 )
    {
        return NULL;
    }
    Assert(IS_IN_RANGE(index, 0, task_manager->m_task_thread_job_max), "");
    return task_manager->m_task_thread_work[index];
}

static TaskThread* TaskManager_TaskThread_Render_Get(TaskManager* task_manager)
{
    if( task_manager->m_task_thread_job_max == 0 )
    {
        return NULL;
    }
    return task_manager->m_task_thread_work[TaskManager_TaskThread_Render_Index_Get(task_manager)];
}

Task* TaskManager_Task_Work_Add(TaskManager* task_manager, const tchar* local_name, TaskThread* task_thread, uint32 priority, bool is_auto_destroy, CB_TaskRun_Condition_Bool_Task_tPtr cb_task_run_condition_bool_task_tptr, CB_TaskRun_Void_Task_tPtr cb_task_run_void_task_tptr, CB_TaskEnd_ClearData_Void_tPtr cb_task_end_clear_data_void_tptr, tptr task_data)
{
    Assert( cb_task_run_void_task_tptr != NULL, "" );
    if( task_thread == NULL )
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
        Task* task = Task_Create(local_name, task_thread, priority, is_auto_destroy, cb_task_run_condition_bool_task_tptr, cb_task_run_void_task_tptr, cb_task_end_clear_data_void_tptr, task_data);

        Queue_Push(Task*, local_name, task_thread->m_task_queue, task);

        if( is_auto_destroy )
        {
            // Maybe you should not keep the handle if auto free
            // return NULL;
        }

        return task;
    }
}

void TaskManager_Task_Render_Add(TaskManager* task_manager, const tchar* local_name, CB_TaskRun_Void_Task_tPtr cb_task_run_void_task_tptr, CB_TaskEnd_ClearData_Void_tPtr cb_task_end_clear_data_void_tptr, tptr task_data)
{
    TaskThread* task_thread = TaskManager_TaskThread_Render_Get(task_manager);
    TaskManager_Task_Work_Add(task_manager, local_name, task_thread, 0, true, NULL, cb_task_run_void_task_tptr, cb_task_end_clear_data_void_tptr, task_data);
}

static void TaskManager_Thread_RunTask(Thread* thread, TaskThread* task_thread)
{
    for(;;)
    {
        if (!Queue_IsEmpty(task_thread->m_task_queue))
        {
            // TODO: use pick
            Task* task = Queue_Dequeue(Task*)(task_thread->m_task_queue);

            if( !Task_TryRun(task) )
            {
                Queue_Push(Task*, NULL, task_thread->m_task_queue, task);
            }
        }
        else
        {
            Thread_Sleep_This_Tick();
        }
    }

    Assert(false, "");
}
