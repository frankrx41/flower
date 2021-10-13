#pragma once

typedef struct TaskManager TaskManager;

typedef struct Task Task;
typedef void (*CB_TaskRun_Void_Task_tPtr)(Task*, tptr);

struct Task
{
    uint32                      m_priority;
    bool                        m_is_auto_free;
    CB_TaskRun_Void_Task_tPtr   m_cb_task_run_void_task_tptr;
    tptr                        m_task_data;
    bool                        m_is_finish;
    bool                        m_is_running;
};


void    Task_Destroy(Task* task);

Task*   TaskManager_Task_Add(TaskManager* task_manager, const tchar* local_name, uint32 priority, bool is_auto_free, CB_TaskRun_Void_Task_tPtr cb_task_run_void_task_tptr, tptr task_data);
