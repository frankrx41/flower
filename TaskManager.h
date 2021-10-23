#pragma once

typedef struct TaskManager TaskManager;

typedef struct Task Task;
typedef struct Thread Thread;
typedef void (*CB_TaskRun_Void_Task_tPtr)(Task*, void*);
typedef bool (*CB_TaskRun_Condition_Bool_Task_tPtr)(Task*, const void*);
typedef void (*CB_TaskEnd_ClearData_Void_tPtr)(void*);

Thread*     TaskManager_Thread_Work_Get         (TaskManager* task_manager, uint32 index);

Task*       TaskManager_Task_Work_Add           (TaskManager* task_manager, const strcrc* local_name, Thread* thread, uint32 priority, bool is_auto_destroy, CB_TaskRun_Condition_Bool_Task_tPtr cb_task_run_condition_bool_task_tptr, CB_TaskRun_Void_Task_tPtr cb_task_run_void_task_tptr, CB_TaskEnd_ClearData_Void_tPtr cb_task_end_clear_data_void_tptr, void* task_data);
void        TaskManager_Task_Render_Add         (TaskManager* task_manager, const strcrc* local_name, CB_TaskRun_Void_Task_tPtr cb_task_run_void_task_tptr, CB_TaskEnd_ClearData_Void_tPtr cb_task_end_clear_data_void_tptr, void* task_data);

TaskManager* TaskManager_GetInstance();

#define TaskManager_Task_Work_Simple_Auto_Add(local_name)
#define TaskManager_Task_Work_Auto_Add(local_name)
#define TaskManager_Task_Work_Add(local_name, thread_id, priority, is_auto_destroy, cb_task_run_condition_bool_task_tptr, cb_task_run_void_task_tptr, cb_task_end_clear_data_void_tptr, task_data)   TaskManager_Task_Work_Add(TaskManager_GetInstance(), local_name, TaskManager_Thread_Work_Get(TaskManager_GetInstance(), thread_id), priority, is_auto_destroy, cb_task_run_condition_bool_task_tptr, cb_task_run_void_task_tptr, cb_task_end_clear_data_void_tptr, task_data)
#define TaskManager_Task_Render_Add(local_name, cb_task_run_void_task_tptr, cb_task_end_clear_data_void_tptr, task_data)   TaskManager_Task_Render_Add(TaskManager_GetInstance(), local_name, cb_task_run_void_task_tptr, cb_task_end_clear_data_void_tptr, task_data)
