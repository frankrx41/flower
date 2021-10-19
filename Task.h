#pragma once

typedef struct Task Task;
typedef void (*CB_TaskRun_Void_Task_tPtr)(Task*, tptr);
typedef bool (*CB_TaskRun_Condition_Bool_Task_tPtr)(Task*, tptr);
typedef void (*CB_TaskEnd_ClearData_Void_tPtr)(tptr);

typedef struct Thread Thread;

/*
You should delete task_data in your task_run function
*/
Task*   Task_Create     (const tchar* local_name, Thread* thread, uint32 priority, bool is_auto_destroy, CB_TaskRun_Condition_Bool_Task_tPtr cb_task_run_condition_bool_task_tptr, CB_TaskRun_Void_Task_tPtr cb_task_run_void_task_tptr, CB_TaskEnd_ClearData_Void_tPtr cb_task_end_clear_data_void_tptr, tptr task_data);
void    Task_Destroy    (Task* task);

void                        Task_Set_IsCancel   (Task* task, bool is_cancel);
bool                        Task_IsCancel       (Task* task);
bool                        Task_IsFinish       (Task* task);
bool                        Task_IsRunning      (Task* task);

bool                        Task_TryExecute     (Task* task);

#define TASK_IS_DEFINED 1
