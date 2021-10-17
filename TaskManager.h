#pragma once

typedef struct TaskManager TaskManager;

typedef struct Task Task;
typedef void (*CB_TaskRun_Void_Task_tPtr)(Task*, tptr);
typedef bool (*CB_TaskRun_Condition_Bool_Task_tPtr)(Task*, tptr);


Task*   TaskManager_Task_Add(TaskManager* task_manager, const tchar* local_name, uint32 thread_id, uint32 priority, bool is_auto_destroy, CB_TaskRun_Condition_Bool_Task_tPtr cb_task_run_condition_bool_task_tptr, CB_TaskRun_Void_Task_tPtr cb_task_run_void_task_tptr, tptr task_data);
void    TaskManager_RenderTask_Add(TaskManager* task_manager, const tchar* local_name, CB_TaskRun_Void_Task_tPtr cb_task_run_void_task_tptr, tptr task_data);

TaskManager* TaskManager_GetInstance();

#define TaskManager_Task_Add(local_name, thread_id, priority, is_auto_destroy, cb_task_run_condition_bool_task_tptr, cb_task_run_void_task_tptr, task_data)   TaskManager_Task_Add(TaskManager_GetInstance(), local_name, thread_id, priority, is_auto_destroy, cb_task_run_condition_bool_task_tptr, cb_task_run_void_task_tptr, task_data)
#define TaskManager_RenderTask_Add(local_name, cb_task_run_void_task_tptr, task_data)   TaskManager_RenderTask_Add(TaskManager_GetInstance(), local_name, cb_task_run_void_task_tptr, task_data)
