#pragma once

typedef struct Scene Scene;
typedef struct SceneManager SceneManager;

typedef void(*CB_Command_Void)(void);

/*
SceneManager_Scene_Destroy will call Scene_Actor_Destroy, and then call Actor_Destroy, you no need call Scene_Actor_Destroy
*/

Scene*  SceneManager_Scene_Create           (SceneManager* scene_manager, const tchar* local_name);
void    SceneManager_Scene_Destroy          (SceneManager* scene_manager, Scene* scene);
void    SceneManager_Scene_SetCurrent       (SceneManager* scene_manager, Scene* scene);
Scene*  SceneManager_Scene_GetCurrent       (SceneManager* scene_manager);

tptr    SceneManager_SceneQueue_Get         (SceneManager* scene_manager);

void    SceneManager_Command_Add            (SceneManager* scene_manager, CB_Command_Void cb_command_void);
void    SceneManager_Command_Clear          (SceneManager* scene_manager);
void    SceneManager_TryRunNextCommand      (SceneManager* scene_manager);

SceneManager* SceneManager_GetInstance  ();

#define SceneManager_Scene_Create(local_name)       SceneManager_Scene_Create(SceneManager_GetInstance(), local_name)
#define SceneManager_Scene_Destroy(scene)           SceneManager_Scene_Destroy(SceneManager_GetInstance(), scene)
#define SceneManager_Scene_SetCurrent(scene)        SceneManager_Scene_SetCurrent(SceneManager_GetInstance(), scene)
#define SceneManager_Scene_GetCurrent()             SceneManager_Scene_GetCurrent(SceneManager_GetInstance())
#define SceneManager_Command_Add(cb_command_void)   SceneManager_Command_Add(SceneManager_GetInstance(), cb_command_void)
#define SceneManager_Command_Clear()                SceneManager_Command_Clear(SceneManager_GetInstance())
