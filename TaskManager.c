#include "CoreMini.h"

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

/*
You should delete task_data in your task_run function
*/
Task* Task_Create(const tchar* local_name, uint32 priority, bool is_auto_free, CB_TaskRun_Void_Task_tPtr cb_task_run_void_task_tptr, tptr task_data)
{
    Task* task = MemNew(local_name, Task);

    task->m_priority                    = priority;
    task->m_is_auto_free                = is_auto_free;
    task->m_cb_task_run_void_task_tptr  = cb_task_run_void_task_tptr;
    task->m_task_data                   = task_data;
    task->m_is_finish                   = false;
    task->m_is_running                  = false;
    return task;
}

void Task_Destroy(Task* task)
{
    MemDel(task);
}

Task* TaskManager_Task_Add(TaskManager* task_manager, const tchar* local_name, uint32 priority, bool is_auto_free, CB_TaskRun_Void_Task_tPtr cb_task_run_void_task_tptr, tptr task_data)
{
    Task* task = Task_Create(local_name, priority, is_auto_free, cb_task_run_void_task_tptr, task_data);

    Queue_Push(Task*, local_name, task_manager->m_task_queue, task);

    if( task->m_is_auto_free )
    {
        return NULL;
    }

    return task;
}

void TaskManager_RunTask(Thread* thread, TaskManager* task_manager)
{
    for(;;)
    {
        if (!Queue_IsEmpty(task_manager->m_task_queue))
        {
            Task* task = Queue_Pop(Task*)(task_manager->m_task_queue);
            if (task->m_priority == 0)
            {
                task->m_is_running  = true;
                task->m_cb_task_run_void_task_tptr(task, task->m_task_data);
                task->m_is_finish   = true;
                task->m_is_running  = false;

                if( task->m_is_auto_free )
                {
                    Task_Destroy(task);
                }
            }
            else
            {
                task->m_priority -= 1;
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
