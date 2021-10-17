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
    task_thread->m_thread       = Thread_Create(local_name, TaskManager_Thread_RunTask, task_thread);
    task_thread->m_task_queue   = Queue_Create(local_name, Task*);

    return task_thread;
}

void TaskThread_Destroy(TaskThread* task_thread)
{
    Queue_Destroy(task_thread->m_task_queue, Task_Destroy);
    Thread_Destroy(task_thread->m_thread);

    MemDel(task_thread);
}

////////////////////////////////////////////////////////////////////////////////
struct TaskManager
{
    String*         m_local_name;
    uint32          m_task_thread_id_max;
    TaskThread*     m_render_task_thread;
    TaskThread**    m_task_thread;
};

#undef TaskManager_Task_Add
#undef TaskManager_RenderTask_Add

TaskManager* TaskManager_Create(const tchar* local_name)
{
    TaskManager* task_manager = MemNew(local_name, TaskManager);

    task_manager->m_local_name      = String_New(local_name, local_name, true);
    task_manager->m_task_thread_id_max = 6;
    task_manager->m_task_thread     = MemNewSize(local_name, sizeof(TaskThread*)*task_manager->m_task_thread_id_max);
    for(uint32 i=0; i<task_manager->m_task_thread_id_max; i++)
    {
        task_manager->m_task_thread[i] = TaskThread_Create(local_name);
    }

    return task_manager;
}

void TaskManager_Destroy(TaskManager* task_manager)
{
    String_Del(task_manager->m_local_name);
    for(uint32 i=0; i<task_manager->m_task_thread_id_max; i++)
    {
        TaskThread_Destroy(task_manager->m_task_thread[i]);
    }

    MemDel(task_manager->m_task_thread);
    MemDel(task_manager);
}
Task* TaskManager_Task_Add(TaskManager* task_manager, const tchar* local_name, uint32 thread_id, uint32 priority, bool is_auto_destroy, CB_TaskRun_Condition_Bool_Task_tPtr cb_task_run_condition_bool_task_tptr, CB_TaskRun_Void_Task_tPtr cb_task_run_void_task_tptr, tptr task_data)
{
    Assert(thread_id >=0 && thread_id < task_manager->m_task_thread_id_max, "");
    TaskThread* task_thread = task_manager->m_task_thread[thread_id];
    Task* task = Task_Create(local_name, task_thread, priority, is_auto_destroy, cb_task_run_condition_bool_task_tptr, cb_task_run_void_task_tptr, task_data);

    Queue_Push(Task*, local_name, task_thread->m_task_queue, task);

    if( is_auto_destroy )
    {
        // Maybe you should not keep the handle if auto free
        // return NULL;
    }

    return task;
}

void TaskManager_RenderTask_Add(TaskManager* task_manager, const tchar* local_name, CB_TaskRun_Void_Task_tPtr cb_task_run_void_task_tptr, tptr task_data)
{
    TaskThread* task_thread = task_manager->m_render_task_thread;
    Task* task = Task_Create(local_name, task_thread, 0, true, NULL, cb_task_run_void_task_tptr, task_data);
    Queue_Push(Task*, local_name, task_thread->m_task_queue, task);
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
