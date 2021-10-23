#pragma once

typedef struct Scene Scene;
typedef struct SceneManager SceneManager;
typedef struct EventInfo EventInfo;

typedef void (*CB_DestroyData_Void_tPtr)        (tptr data);
typedef void (*CB_Command_Void)                 (void);
typedef void (*CB_SceneDestroy_Void_Scene)      (Scene* scene);

/*
SceneManager_Scene_Destroy will call Scene_Actor_Destroy, and then call Actor_Destroy, you no need call Scene_Actor_Destroy
*/

Scene*  SceneManager_Scene_Create           (SceneManager* scene_manager, strcrc* local_name, CB_SceneDestroy_Void_Scene cb_scene_destroy_void_scene);
void    SceneManager_Scene_Destroy          (SceneManager* scene_manager, Scene* scene);
void    SceneManager_Scene_Foreground_Queue_Add     (SceneManager* scene_manager, Scene* scene);
void    SceneManager_Scene_Foreground_Queue_Remove  (SceneManager* scene_manager, Scene* scene);
void    SceneManager_Scene_Foreground_Queue_Clear   (SceneManager* scene_manager);

void    SceneManager_Scene_ExitCurrent      (SceneManager* scene_manager);

tptr    SceneManager_SceneQueue_Foreground_Get         (SceneManager* scene_manager);

void    SceneManager_Command_Add            (SceneManager* scene_manager, CB_Command_Void cb_command_void);
void    SceneManager_Command_Clear          (SceneManager* scene_manager);

bool    SceneManager_Scene_IsLoading        (SceneManager* scene_manager);

void    SceneManager_TryRunNextCommand      (SceneManager* scene_manager);

void    SceneManager_SceneEvent_SendTo_Scene           (SceneManager* scene_manager, EventInfo* event_info);


SceneManager* SceneManager_GetInstance  ();

#define SceneManager_Scene_Create(local_name, cb_scene_destroy_void_scene)       SceneManager_Scene_Create(SceneManager_GetInstance(), local_name, cb_scene_destroy_void_scene)
#define SceneManager_Scene_Destroy(scene)           SceneManager_Scene_Destroy(SceneManager_GetInstance(), scene)

#define SceneManager_Scene_Foreground_Queue_Add(scene)      SceneManager_Scene_Foreground_Queue_Add(SceneManager_GetInstance(), scene)
#define SceneManager_Scene_Foreground_Queue_Remove(scene)   SceneManager_Scene_Foreground_Queue_Remove(SceneManager_GetInstance(), scene)
#define SceneManager_Scene_Foreground_Queue_Clear()         SceneManager_Scene_Foreground_Queue_Clear(SceneManager_GetInstance())

#define SceneManager_Command_Add(cb_command_void)   SceneManager_Command_Add(SceneManager_GetInstance(), cb_command_void)
#define SceneManager_Command_Clear()                SceneManager_Command_Clear(SceneManager_GetInstance())
#define SceneManager_Scene_ExitCurrent()            SceneManager_Scene_ExitCurrent(SceneManager_GetInstance())
#define SceneManager_Scene_IsLoading()              SceneManager_Scene_IsLoading(SceneManager_GetInstance())
