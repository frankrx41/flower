#include "CoreMini.h"

#include "Event.h"
#include "KeyId.h"

#include "MemoryManager.h"
#include "EventManager.h"

#include "ActorComponent.h"

#include "Scene.h"
#include "Actor.h"

#include "Queue.h"
#include "String.h"
#include "tData.h"
#include "Storage.h"
#include "Vec.h"


struct Scene
{
    uint32                      m_alloc_actor_id;
    CB_SceneDestroy_Void_Scene  m_cb_scene_destroy_void_scene;
    String*                     m_local_name;
    Storage*                    m_storage;
    bool                        m_is_pause;
    vec2                        m_render_offset_vec;
    Queue(Actor*)*              m_child_actor_queue;
    Queue(Actor*)*              m_actor_scene_event_queue_list[Event_Scene_Max-Event_Scene_Min];
    Queue(Actor*)*              m_actor_action_event_queue;
    Queue(Actor*)*              m_actor_renderable_able_queue;
    Queue(Actor*)*              m_physics_actor_queue;
};

Actor* Actor_Create(const tchar* local_name, Scene* scene, uint32 id, CB_ActorCreate_Void_Actor_tPtr cb_actor_create_void_actor_tptr, tptr ptr);

Scene* Scene_Create(const tchar* local_name)
{
    Scene* scene = MemNew(local_name, Scene);
    scene->m_child_actor_queue          = Queue_Create(local_name, Actor*);
    scene->m_alloc_actor_id             = 0;
    scene->m_local_name                 = String_New(local_name, local_name, true);
    scene->m_storage                    = Storage_Create(local_name);
    scene->m_is_pause                   = false;
    scene->m_cb_scene_destroy_void_scene = NULL;
    scene->m_actor_action_event_queue   = Queue_Create(local_name, Actor*);
    scene->m_physics_actor_queue        = Queue_Create(local_name, Actor*);
    scene->m_render_offset_vec          = vec2_null;

    Assert(ARRAY_SIZE(scene->m_actor_scene_event_queue_list) == Event_Scene_Max-Event_Scene_Min, "");
    for(uint32 i=0, max_i = ARRAY_SIZE(scene->m_actor_scene_event_queue_list); i<max_i; i++)
    {
        scene->m_actor_scene_event_queue_list[i] = Queue_Create(local_name, Actor*);
    }

    return scene;
}

void Scene_Destroy(Scene* scene)
{
    Assert(scene != false, "");

    if( scene->m_cb_scene_destroy_void_scene )
    {
        scene->m_cb_scene_destroy_void_scene(scene);
    }

    Queue_Destroy(scene->m_child_actor_queue, Actor_Destroy);
    for(uint32 i=0, max_i=ARRAY_SIZE(scene->m_actor_scene_event_queue_list); i<max_i; i++)
    {
        Queue_Destroy(scene->m_actor_scene_event_queue_list[i], NULL);
    }

    Queue_Destroy(scene->m_actor_action_event_queue, NULL);
    Queue_Destroy(scene->m_physics_actor_queue, NULL);


    String_Del(scene->m_local_name);
    Storage_Destroy(scene->m_storage);
    MemDel(scene);
}

void Scene_Render_Offset_Set(Scene* scene, vec2 vec)
{
    Assert(scene != NULL, "");
    scene->m_render_offset_vec = vec;
}

vec2 Scene_Render_Offset_Get(Scene* scene)
{
    Assert(scene != NULL, "");
    return scene->m_render_offset_vec;
}

static Queue(Actor*)* Scene_EventQueue_Get(Scene* scene, Event event)
{
    Assert(IS_IN_RANGE(event, Event_Scene_Min, Event_Scene_Max), "" );

    return scene->m_actor_scene_event_queue_list[event-Event_Scene_Min];
}

void Scene_Destroy_CB_Set(Scene* scene, CB_SceneDestroy_Void_Scene cb_scene_destroy_void_scene)
{
    scene->m_cb_scene_destroy_void_scene = cb_scene_destroy_void_scene;
}

bool Scene_IsPause(Scene* scene)
{
    return scene->m_is_pause;
}

void Scene_SetIsPause(Scene* scene, bool is_pause)
{
    scene->m_is_pause = is_pause;
}

Actor* Scene_Actor_Create(const tchar* local_name, Scene* scene, CB_ActorCreate_Void_Actor_tPtr cb_actor_create_void_actor_tptr, tptr ptr)
{
    Actor* actor = Actor_Create(local_name, scene, scene->m_alloc_actor_id++, cb_actor_create_void_actor_tptr, ptr);
    Queue_Push(Actor*, local_name, scene->m_child_actor_queue, actor);
    return actor;
}

void Scene_Actor_Destroy(Scene* scene, CB_FindData_Bool_tPtr_tPtr cb_find_actor_bool_tptr_tptr, tptr ptr)
{
    if( cb_find_actor_bool_tptr_tptr == NULL )
    {
        Queue_RemoveFindFirst(Actor*)(scene->m_child_actor_queue, NULL, ptr);
        Actor_Destroy(ptr);
    }
    else
    {
        Actor* actor = Queue_RemoveFindFirst(Actor*)(scene->m_child_actor_queue, cb_find_actor_bool_tptr_tptr, ptr);
        Actor_Destroy(actor);
    }
}

void Scene_Actor_Destroy_All(Scene* scene)
{
    Queue_Destroy(scene->m_child_actor_queue, Actor_Destroy);
}

void CallBack_Actor_ProcessSceneEvent(Actor* actor, const EventInfo* event_info);
void CallBack_Actor_ProcessActionEvent(Actor* actor, const EventInfo* event_info);

void Scene_SceneEvent_Send_Actor(Scene* scene, EventInfo* event_info)
{
    Event event = event_info->m_event;

    // Scene event
    Assert(IS_IN_RANGE(event, Event_Scene_Min, Event_Scene_Max), "");
    Queue_ForEach(Scene_EventQueue_Get(scene, event), (CB_ProcessData_Void_tPtr_tPtr)CallBack_Actor_ProcessSceneEvent, event_info);
}

void Scene_ActionEvent_Send_Actor(Scene* scene, EventInfo* event_info)
{
    Event event = event_info->m_event;

    Assert(IS_IN_RANGE(event, Event_Actor_Action_Min, Event_Actor_Action_Max), "");
    Queue_ForEach(scene->m_actor_action_event_queue, CallBack_Actor_ProcessActionEvent, event_info);
}

void Scene_SceneEventGroup_Actor_Add(Scene* scene, Actor* actor, Event event)
{
    Assert(IS_IN_RANGE(event, Event_Scene_Min, Event_Scene_Max), "Invalid event!");
    Queue_Push(Actor*, NULL, Scene_EventQueue_Get(scene, event), actor);
}

void Scene_ActionEventGroup_Actor_Add(Scene* scene, Actor* actor, Event event)
{
    Assert(IS_IN_RANGE(event, Event_Actor_Action_Min, Event_Actor_Action_Max), "Invalid event!");
    Queue_Push(Actor*, NULL, scene->m_actor_action_event_queue, actor);
}

void Scene_PhysicsGroup_Actor_Add(Scene* scene, Actor* actor)
{
    Queue_Push(Actor*, NULL, scene->m_physics_actor_queue, actor);
}

void Scene_PhysicsGroup_Actor_Remove(Scene* scene, Actor* actor)
{
    Queue_RemoveFindFirst(Actor*)(scene->m_physics_actor_queue, NULL, actor);
}

void Scene_PhysicsActor_Update(Scene* scene, float delta_seconds)
{
    float delta_second_copy = delta_seconds;
    Queue_ForEach(scene->m_physics_actor_queue, CallBack_Actor_Component_Physics_Simulate, &delta_second_copy);
}

void Scene_Storage_StoreData(Scene* scene, crc32 variable, tdata data)
{
    Storage_StoreData(scene->m_storage, variable, data);
}

bool Scene_Storage_IsExistVariable(Scene* scene, crc32 variable)
{
    return
    Storage_IsExistVariable(scene->m_storage, variable);
}

tdata Scene_Storage_ReadData(Scene* scene, crc32 variable)
{
    return
    Storage_ReadData(scene->m_storage, variable);
}

void Scene_Storage_DeleteVariable(Scene* scene, crc32 variable)
{
    Storage_DeleteVariable(scene->m_storage, variable);
}

tptr Scene_GetChildActorQueue(Scene* scene)
{
    return scene->m_child_actor_queue;
}

tptr Scene_GetRenderableActorQueue(Scene* scene)
{
    return scene->m_actor_renderable_able_queue;
}
