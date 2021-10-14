#include "CoreMini.h"

#include "Engine.h"

#include "Event.h"
#include "KeyId.h"

#include "MemoryManager.h"
#include "SceneManager.h"
#include "EventManager.h"
#include "TaskManager.h"

#include "Queue.h"
#include "Scene.h"
#include "String.h"


struct SceneManager
{
    Queue(Scene*)*          m_scene_queue;
    Queue(CB_Command_Void)* m_command_queue;
    Scene*                  m_current_scene;
    String*                 m_local_name;
    bool                    m_is_exit_current_scene;
    bool                    m_is_scene_loading;
};

Scene*  Scene_Create    (const tchar* local_name);
void    Scene_Destroy   (Scene* scene);

#undef SceneManager_Scene_Create
#undef SceneManager_SetCurrentScene
#undef SceneManager_GetCurrentScene
#undef SceneManager_Scene_Destroy
#undef SceneManager_Scene_SetCurrent
#undef SceneManager_Scene_GetCurrent
#undef SceneManager_Command_Add
#undef SceneManager_Command_Clear
#undef SceneManager_Scene_ExitCurrent
#undef SceneManager_Scene_IsLoading

////////////////////////////////////////////////////////////////////////////////
SceneManager* SceneManager_Create(const tchar* local_name)
{
    SceneManager* scene_manager     = MemNew(local_name, SceneManager);
    scene_manager->m_scene_queue    = Queue_Create(local_name, Sence*);
    scene_manager->m_command_queue  = Queue_Create(local_name, CB_Command_Void);
    scene_manager->m_current_scene  = NULL;
    scene_manager->m_local_name     = String_New(local_name, local_name, true);
    scene_manager->m_is_exit_current_scene  = true;
    scene_manager->m_is_scene_loading       = false;
    return scene_manager;
}

void SceneManager_Destroy(SceneManager* scene_manager)
{
    Queue_Destroy(scene_manager->m_scene_queue, Scene_Destroy);
    Queue_Destroy(scene_manager->m_command_queue, NULL);
    String_Del(scene_manager->m_local_name);
    MemDel(scene_manager);
}

Scene* SceneManager_Scene_Create(SceneManager* scene_manager, const tchar* local_name)
{
    Scene* scene = Scene_Create(local_name);
    Queue_Push(Sence*, local_name, scene_manager->m_scene_queue, scene);

    return scene;
}

void SceneManager_Scene_Destroy(SceneManager* scene_manager, Scene* scene)
{
    Queue_RemoveFindFirst(Scene*)(scene_manager->m_scene_queue, NULL, scene);
    Scene_Destroy(scene);
}

void SceneManager_Scene_SetCurrent(SceneManager* scene_manager, Scene* scene)
{
    scene_manager->m_current_scene = scene;
}

Scene* SceneManager_Scene_GetCurrent(SceneManager* scene_manager)
{
    return scene_manager->m_current_scene;
}

void SceneManager_Scene_ExitCurrent(SceneManager* scene_manager)
{
    scene_manager->m_is_exit_current_scene = true;
}

tptr SceneManager_SceneQueue_Get(SceneManager* scene_manager)
{
    return scene_manager->m_scene_queue;
}

void SceneManager_Command_Add(SceneManager* scene_manager, CB_Command_Void cb_command_void)
{
    Queue_Push(CB_Command_Void, NULL, scene_manager->m_command_queue, cb_command_void);
}

void SceneManager_Command_Clear(SceneManager* scene_manager)
{
    Queue_Destroy(scene_manager->m_command_queue, NULL);
}

bool SceneManager_Scene_IsLoading(SceneManager* scene_manager)
{
    return scene_manager->m_is_scene_loading;
}

static void SceneManager_LoadLastScene(Task* task, SceneManager* scene_manager)
{
    SceneManager_Scene_SetCurrent(scene_manager, NULL);
    scene_manager->m_is_exit_current_scene = false;
    Queue_Clear(scene_manager->m_scene_queue, Scene_Destroy);
    (Queue_Dequeue(CB_Command_Void)(scene_manager->m_command_queue))();
    scene_manager->m_is_scene_loading = false;
}

void SceneManager_TryRunNextCommand(SceneManager* scene_manager)
{
    if (Queue_IsEmpty(scene_manager->m_command_queue))
    {
        Engine_NotifyExit();
    }
    else
    {
        if( scene_manager->m_is_exit_current_scene || Queue_IsEmpty(scene_manager->m_scene_queue) )
        {
            if( !SceneManager_Scene_IsLoading(scene_manager) )
            {
                scene_manager->m_is_scene_loading = true;
                TaskManager_Task_Add(String_CStr(scene_manager->m_local_name), 0, true, SceneManager_LoadLastScene, scene_manager);
            }
        }
    }
}

static void CallBack_SendEvent_Scene_Tick(Scene* scene, EventInfo* event_info)
{
    EventInfo* event_info_scene_tick = EventInfo_Create(Scene_LocalName_Str_Get(scene), Event_Scene_Tick, scene, NULL, KeyId_Null, event_info->m_delta_seconds);
    Scene_SceneEvent_Send_Actor(scene, event_info_scene_tick);
    EventInfo_Destroy(event_info_scene_tick);

    // Scene physics update
    Scene_PhysicsActor_Update(scene, event_info->m_delta_seconds);
}

void SceneManager_OnEvent_Tick(SceneManager* scene_manager, EventInfo* event_info)
{
    if( SceneManager_Scene_IsLoading(scene_manager) )
    {
        return;
    }
    Queue_ForEach(SceneManager_SceneQueue_Get(SceneManager_GetInstance()), CallBack_SendEvent_Scene_Tick, event_info);
}
