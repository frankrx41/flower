#include "CoreMini.h"

#include "MemoryManager.h"
#include "SceneManager.h"

#include "Queue.h"


struct SceneManager
{
    Queue(Scene*)*  m_scene_queue;

    Scene*          m_current_scene;
};

Scene*  Scene_Create    (const tchar* local_name);
void    Scene_Destroy   (Scene* scene);

#undef SceneManager_Scene_Create
#undef SceneManager_SetCurrentScene
#undef SceneManager_GetCurrentScene
#undef SceneManager_Scene_Destroy
#undef SceneManager_Scene_SetCurrent
#undef SceneManager_Scene_GetCurrent

////////////////////////////////////////////////////////////////////////////////
SceneManager* SceneManager_Create(const tchar* local_name)
{
    SceneManager* scene_manager     = MemNew(local_name, SceneManager);
    scene_manager->m_scene_queue    = Queue_Create(local_name, Sence*);
    scene_manager->m_current_scene  = NULL;

    return scene_manager;
}

void SceneManager_Destroy(SceneManager* scene_manager)
{
    Queue_Destroy(scene_manager->m_scene_queue, Scene_Destroy);
    MemDel(scene_manager);
}

Scene* SceneManager_Scene_Create(SceneManager* scene_manager, const tchar* local_name)
{
    Scene* scene = Scene_Create(local_name);
    Queue_Push(Sence*, local_name, scene_manager->m_scene_queue, scene);

    return scene;
}

Scene* SceneManager_Scene_Destroy(SceneManager* scene_manager, Scene* scene)
{
    Queue_RemoveFindFirst(Scene*)(scene_manager->m_scene_queue, NULL, scene);
    Scene_Destroy(scene);
    return scene;
}

void SceneManager_Scene_SetCurrent(SceneManager* scene_manager, Scene* scene)
{
    scene_manager->m_current_scene = scene;
}

Scene* SceneManager_Scene_GetCurrent(SceneManager* scene_manager)
{
    return scene_manager->m_current_scene;
}

tptr SceneManager_GetSceneQueue(SceneManager* scene_manager)
{
    return scene_manager->m_scene_queue;
}

