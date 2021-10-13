#include "CoreMini.h"

#include "MemoryManager.h"

#include "Task.h"


struct Task
{
    uint32                      m_priority;
    CB_TaskRun_Void_Task_tPtr   m_cb_task_run_void_task_tptr;
    tptr                        m_task_data;

    bool                        m_is_auto_destroy;
    bool                        m_is_finish;
    bool                        m_is_running;
    bool                        m_is_cancel;
};

/*
You should delete task_data in your task_run function
*/
Task* Task_Create(const tchar* local_name, uint32 priority, bool is_auto_destroy, CB_TaskRun_Void_Task_tPtr cb_task_run_void_task_tptr, tptr task_data)
{
    Task* task = MemNew(local_name, Task);

    Assert(IS_IN_RANGE(priority, 0, 5), "0 is Highest and 4 is Lowest");
    task->m_priority = priority;
    task->m_cb_task_run_void_task_tptr  = cb_task_run_void_task_tptr;
    task->m_task_data                   = task_data;

    task->m_is_auto_destroy             = is_auto_destroy;
    task->m_is_finish                   = false;
    task->m_is_running                  = false;
    task->m_is_cancel                   = false;
    return task;
}

void Task_Destroy(Task* task)
{
    MemDel(task);
}

void Task_Set_IsCancel(Task* task, bool is_cancel)
{
    task->m_is_cancel = is_cancel;
}

bool Task_IsCancel(Task* task)
{
    return task->m_is_cancel;
}

bool Task_IsFinish(Task* task)
{
    return task->m_is_finish;
}

bool Task_IsRunning(Task* task)
{
    return task->m_is_running;
}

static void Task_TryDestory(Task* task)
{
    if (task->m_is_auto_destroy)
    {
        Task_Destroy(task);
    }
}

bool Task_TryRun(Task* task)
{
    if( Task_IsCancel(task) )
    {
        Task_TryDestory(task);
        return true;
    }

    if( task->m_priority == 0 )
    {
        task->m_is_running  = true;
        task->m_cb_task_run_void_task_tptr(task, task->m_task_data);
        task->m_is_running  = false;
        task->m_is_finish   = true;

        Task_TryDestory(task);
        return true;
    }

    task->m_priority--;
    return false;
}
