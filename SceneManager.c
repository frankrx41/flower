#include "CoreMini.h"

#include "Engine.h"

#include "Event.h"
#include "KeyId.h"

#include "MemoryManager.h"
#include "SceneManager.h"
#include "EventManager.h"
#include "TaskManager.h"
#include "TimingManager.h"

#include "Queue.h"
#include "Scene.h"
#include "String.h"


struct SceneManager
{
    Queue(Scene*)*          m_scene_queue_all;
    Queue(Scene*)*          m_scene_queue_foreground;
    Queue(CB_Command_Void)* m_command_queue;
    strcrc                  m_local_name;
    bool                    m_is_exit_current_scene;
    bool                    m_is_scene_loading;
};

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
#undef SceneManager_Scene_Foreground_Queue_Add
#undef SceneManager_Scene_Foreground_Queue_Remove
#undef SceneManager_Scene_Foreground_Queue_Clear


////////////////////////////////////////////////////////////////////////////////
SceneManager* SceneManager_Create(const strcrc* local_name)
{
    SceneManager* scene_manager             = MemNew(local_name, SceneManager);
    scene_manager->m_scene_queue_all        = Queue_Create(local_name, Scene*);
    scene_manager->m_scene_queue_foreground = Queue_Create(local_name, Scene*);
    scene_manager->m_command_queue          = Queue_Create(local_name, CB_Command_Void);
    StrCrc_Copy(local_name, &scene_manager->m_local_name);
    scene_manager->m_is_exit_current_scene  = true;
    scene_manager->m_is_scene_loading       = false;
    return scene_manager;
}

void SceneManager_Destroy(SceneManager* scene_manager)
{
    Queue_Destroy(scene_manager->m_scene_queue_all, Scene_Destroy);
    Queue_Destroy(scene_manager->m_scene_queue_foreground, NULL);
    Queue_Destroy(scene_manager->m_command_queue, NULL);

    MemDel(scene_manager);
}

Scene* SceneManager_Scene_Create(SceneManager* scene_manager, strcrc* local_name, CB_SceneDestroy_Void_Scene cb_scene_destroy_void_scene)
{
    Scene* scene = Scene_Create(local_name, scene_manager, cb_scene_destroy_void_scene);
    Queue_Push(Scene*, local_name, scene_manager->m_scene_queue_all, scene);

    return scene;
}

void SceneManager_Scene_Destroy(SceneManager* scene_manager, Scene* scene)
{
    Queue_RemoveFindFirst(Scene*)(scene_manager->m_scene_queue_all, NULL, scene, Scene_Destroy);
}

void SceneManager_Scene_Foreground_Queue_Add(SceneManager* scene_manager, Scene* scene)
{
    Queue_Push(Scene*, NULL, scene_manager->m_scene_queue_foreground, scene);
}

void SceneManager_Scene_Foreground_Queue_Remove(SceneManager* scene_manager, Scene* scene)
{
    Queue_RemoveFindFirst(Scene*)(scene_manager->m_scene_queue_foreground, NULL, scene, NULL);
}

void SceneManager_Scene_Foreground_Queue_Clear(SceneManager* scene_manager)
{
    Assert( Queue_IsEmpty(scene_manager->m_scene_queue_foreground) == false, "You try to clear empty scene foreground queue!");
    uint32 destroy_count = 0;
    for(;!Queue_IsEmpty(scene_manager->m_scene_queue_foreground);)
    {
        Scene* scene = Queue_Dequeue(Scene*)(scene_manager->m_scene_queue_foreground);
        SceneManager_Scene_Destroy(scene_manager, scene);
        destroy_count ++;
    }
}

void SceneManager_Scene_ExitCurrent(SceneManager* scene_manager)
{
    scene_manager->m_is_exit_current_scene = true;
    Log(4, "Scene End\n");
}

void* SceneManager_SceneQueue_Foreground_Get(SceneManager* scene_manager)
{
    return scene_manager->m_scene_queue_foreground;
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
    Log(4, "Scene Loading\n");
    (Queue_Dequeue(CB_Command_Void)(scene_manager->m_command_queue))();
    scene_manager->m_is_scene_loading = false;
}

void SceneManager_TryRunNextCommand(SceneManager* scene_manager)
{
    if( scene_manager->m_is_exit_current_scene || Queue_IsEmpty(scene_manager->m_scene_queue_all) )
    {
        scene_manager->m_is_exit_current_scene = false;

        if (!Queue_IsEmpty(scene_manager->m_scene_queue_foreground))
        {
            SceneManager_Scene_Foreground_Queue_Clear(scene_manager);
        }

        if( !SceneManager_Scene_IsLoading(scene_manager) )
        {
            if( !Queue_IsEmpty(scene_manager->m_command_queue) )
            {
                scene_manager->m_is_scene_loading = true;
                TaskManager_Task_Work_Add(&scene_manager->m_local_name, 0, 0, true, NULL, SceneManager_LoadLastScene, NULL, scene_manager);
                Memory_Stat();
                TimingManager_Stat();
            }
            else
            {
                Engine_NotifyExit();
            }
        }
    }
}

static void CallBack_Scene_Receive_SceneEvent(Scene* scene, const EventInfo* event_info)
{
    EventInfo* scene_event_info = EventInfo_Create(Scene_LocalName_Str_Get(scene), event_info->m_event, scene, NULL, KeyId_Null, event_info->m_delta_seconds);
    Scene_SceneEvent_SendTo_Actor(scene, scene_event_info);
    EventInfo_Destroy(scene_event_info);
}

void SceneManager_SceneEvent_SendTo_Scene(SceneManager* scene_manager, EventInfo* event_info)
{
    if( SceneManager_Scene_IsLoading(scene_manager) )
    {
        return;
    }
    Queue_ForEach(SceneManager_SceneQueue_Foreground_Get(SceneManager_GetInstance()), CallBack_Scene_Receive_SceneEvent, event_info);
}
