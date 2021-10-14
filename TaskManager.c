#include "CoreMini.h"

#include "Task.h"

#include "MemoryManager.h"
#include "TaskManager.h"

#include "Queue.h"
#include "String.h"
#include "Thread.h"


struct TaskManager
{
    Thread*         m_thread;
    String*         m_local_name;
    Queue(Task*)*   m_task_queue;
};

static void TaskManager_RunTask(Thread* thread,TaskManager* task_manager);

TaskManager* TaskManager_Create(const tchar* local_name)
{
    TaskManager* task_manager = MemNew(local_name, TaskManager);

    task_manager->m_local_name  = String_New(local_name, local_name, true);
    task_manager->m_thread      = Thread_Create(local_name, TaskManager_RunTask, task_manager);
    task_manager->m_task_queue  = Queue_Create(local_name, Task*);

    return task_manager;
}

void TaskManager_Destroy(TaskManager* task_manager)
{
    Queue_Destroy(task_manager->m_task_queue, Task_Destroy);
    String_Del(task_manager->m_local_name);

    Thread_Destroy(task_manager->m_thread);

    MemDel(task_manager);
}


Task* TaskManager_Task_Add(TaskManager* task_manager, const tchar* local_name, uint32 priority, bool is_auto_free, CB_TaskRun_Void_Task_tPtr cb_task_run_void_task_tptr, tptr task_data)
{
    Task* task = Task_Create(local_name, priority, is_auto_free, cb_task_run_void_task_tptr, task_data);

    Queue_Push(Task*, local_name, task_manager->m_task_queue, task);

    if( is_auto_free )
    {
        // Maybe you should not keep the handle if auto free
        // return NULL;
    }

    return task;
}

void TaskManager_RunTask(Thread* thread, TaskManager* task_manager)
{
    for(;;)
    {
        if (!Queue_IsEmpty(task_manager->m_task_queue))
        {
            Task* task = Queue_Dequeue(Task*)(task_manager->m_task_queue);

            if( !Task_TryRun(task) )
            {
                Queue_Push(Task*, String_CStr(task_manager->m_local_name), task_manager->m_task_queue, task);
            }
        }
        else
        {
            Thread_Sleep(NULL, 0.001f);
        }
    }

    Assert(false, "");
}