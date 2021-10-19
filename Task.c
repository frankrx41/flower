#include "CoreMini.h"

#include "MemoryManager.h"

#include "Task.h"

struct Task
{
    CB_TaskRun_Void_Task_tPtr           m_cb_task_run_void_task_tptr;
    CB_TaskRun_Condition_Bool_Task_tPtr m_cb_task_run_condition_bool_task_tptr;
    CB_TaskEnd_ClearData_Void_tPtr      m_cb_task_end_clear_data_void_tptr;

    tptr        m_task_data;
    uint8       m_priority;
    bool        m_is_auto_destroy;
    bool        m_is_finish;
    bool        m_is_running;
    bool        m_is_cancel;
    Thread*     m_thread;
};

/*
You should delete task_data in your task_run function
*/
Task* Task_Create(const tchar* local_name, Thread* thread, uint32 priority, bool is_auto_destroy, CB_TaskRun_Condition_Bool_Task_tPtr cb_task_run_condition_bool_task_tptr, CB_TaskRun_Void_Task_tPtr cb_task_run_void_task_tptr, CB_TaskEnd_ClearData_Void_tPtr cb_task_end_clear_data_void_tptr, tptr task_data)
{
    Task* task = MemNew(local_name, Task);

    Assert(IS_IN_RANGE(priority, 0, 5), "0 is Highest and 4 is Lowest");
    
    task->m_cb_task_run_void_task_tptr              = cb_task_run_void_task_tptr;
    task->m_cb_task_run_condition_bool_task_tptr    = cb_task_run_condition_bool_task_tptr;
    task->m_cb_task_end_clear_data_void_tptr        = cb_task_end_clear_data_void_tptr;

    task->m_priority        = priority;
    task->m_task_data       = task_data;
    task->m_thread          = thread;

    task->m_is_auto_destroy = is_auto_destroy;
    task->m_is_finish       = false;
    task->m_is_running      = false;
    task->m_is_cancel       = false;
    return task;
}

void Task_Destroy(Task* task)
{
    if( task->m_cb_task_end_clear_data_void_tptr )
    {
        task->m_cb_task_end_clear_data_void_tptr(task->m_task_data);
    }

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

bool Task_TryDestory(Task* task)
{
    if (task->m_is_auto_destroy)
    {
        Task_Destroy(task);
        return true;
    }
    return false;
}

bool Task_TryExecute(Task* task)
{
    Assert(Task_IsFinish(task) == false, "");

    if( Task_IsCancel(task) )
    {
        return true;
    }

    if( task->m_cb_task_run_condition_bool_task_tptr )
    {
        if( !task->m_cb_task_run_condition_bool_task_tptr(task, task->m_task_data) )
        {
            return false;
        }
    }

    if( task->m_priority == 0 )
    {
        task->m_is_running  = true;
        task->m_cb_task_run_void_task_tptr(task, task->m_task_data);
        task->m_is_running  = false;
        task->m_is_finish   = true;

        return true;
    }

    task->m_priority--;
    return false;
}
