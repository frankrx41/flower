#pragma once

typedef struct Scene Scene;
typedef struct SceneManager SceneManager;

/*
SceneManager_Scene_Destroy will call Scene_Actor_Destroy, and then call Actor_Destroy, you no need call Scene_Actor_Destroy
*/

Scene*  SceneManager_Scene_Create           (SceneManager* scene_manager, const tchar* local_name);
Scene*  SceneManager_Scene_Destroy          (SceneManager* scene_manager, Scene* scene);
void    SceneManager_Scene_SetCurrent       (SceneManager* scene_manager, Scene* scene);
Scene*  SceneManager_Scene_GetCurrent       (SceneManager* scene_manager);

tptr    SceneManager_GetSceneQueue          (SceneManager* scene_manager);

SceneManager* SceneManager_GetInstance  ();

#define SceneManager_Scene_Create(local_name)   SceneManager_Scene_Create(SceneManager_GetInstance(), local_name)
#define SceneManager_Scene_Destroy(scene)       SceneManager_Scene_Destroy(SceneManager_GetInstance(), scene)
#define SceneManager_Scene_SetCurrent(scene)    SceneManager_Scene_SetCurrent(SceneManager_GetInstance(), scene)
#define SceneManager_Scene_GetCurrent()         SceneManager_Scene_GetCurrent(SceneManager_GetInstance())
