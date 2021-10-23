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
#include "Viewport.h"


struct Scene
{
    uint32                      m_alloc_actor_id;
    CB_SceneDestroy_Void_Scene  m_cb_scene_destroy_void_scene;
    strcrc                      m_local_name;
    Storage*                    m_storage;
    
    bool                        m_is_pause_event;
    bool                        m_is_pause_render;

    Viewport*                   m_viewport;

    Queue(Actor*)*              m_child_actor_queue;
    Queue(Actor*)*              m_actor_queue_scene_event_list[Event_Scene_Max-Event_Scene_Min];
    Queue(Actor*)*              m_actor_queue_control;
    Queue(Actor*)*              m_actor_queue_renderable;
    Queue(Actor*)*              m_actor_queue_physics;
};

Actor* Actor_Create(const strcrc* local_name, Scene* scene, uint32 id, CB_ActorCreate_Void_Actor_tPtr cb_actor_create_void_actor_tptr, CB_ActorDestroy_Void_Actor cb_actor_destroy_void_actor, const void* ptr);

Scene* Scene_Create(const strcrc* local_name, SceneManager* scene_manager, CB_SceneCreate_Void_Scene_Ptr cb_scene_create_void_scene_ptr, CB_SceneDestroy_Void_Scene cb_scene_destroy_void_scene, void* ptr)
{
    Scene* scene = MemNew(local_name, Scene);
    scene->m_child_actor_queue          = Queue_Create(local_name, Actor*);
    scene->m_alloc_actor_id             = 0;
    StrCrc_Copy(local_name, &scene->m_local_name);
    scene->m_storage                    = Storage_Create(local_name);
    scene->m_is_pause_event             = false;
    scene->m_is_pause_render            = false;
    scene->m_cb_scene_destroy_void_scene = cb_scene_destroy_void_scene;
    scene->m_actor_queue_control    = Queue_Create(local_name, Actor*);
    scene->m_actor_queue_physics    = Queue_Create(local_name, Actor*);
    scene->m_actor_queue_renderable = Queue_Create(local_name, Actor*);
    scene->m_viewport               = NULL;

    Assert(ARRAY_SIZE(scene->m_actor_queue_scene_event_list) == Event_Scene_Max-Event_Scene_Min, "");
    for(uint32 i=0, max_i = ARRAY_SIZE(scene->m_actor_queue_scene_event_list); i<max_i; i++)
    {
        scene->m_actor_queue_scene_event_list[i] = Queue_Create(local_name, Actor*);
    }

    if( cb_scene_create_void_scene_ptr )
    {
        cb_scene_create_void_scene_ptr(scene, ptr);
    }

    return scene;
}

void Scene_Viewport_Create(Scene* scene, float width, float height, const vec2* scale, const vec2* offset)
{
    scene->m_viewport = Viewport_Create(&scene->m_local_name, width, height, scale, offset);
}

void Scene_Destroy(Scene* scene)
{
    Assert(scene != false, "");

    if( scene->m_cb_scene_destroy_void_scene )
    {
        scene->m_cb_scene_destroy_void_scene(scene);
    }

    if( scene->m_viewport )
    {
        Viewport_Destroy(scene->m_viewport);
    }

    Queue_Destroy(scene->m_child_actor_queue, Actor_Destroy);
    for(uint32 i=0, max_i=ARRAY_SIZE(scene->m_actor_queue_scene_event_list); i<max_i; i++)
    {
        Queue_Destroy(scene->m_actor_queue_scene_event_list[i], NULL);
    }

    Queue_Destroy(scene->m_actor_queue_control, NULL);
    Queue_Destroy(scene->m_actor_queue_physics, NULL);
    Queue_Destroy(Scene_ActorQueue_Renderable_Get(scene), NULL);


    Storage_Destroy(scene->m_storage);
    MemDel(scene);
}

static Queue(Actor*)* Scene_EventQueue_Get(Scene* scene, Event event)
{
    Assert(IS_IN_RANGE(event, Event_Scene_Min, Event_Scene_Max), "" );

    return scene->m_actor_queue_scene_event_list[event-Event_Scene_Min];
}

void Scene_Pause_All(Scene* scene)
{
    scene->m_is_pause_render    = true;
    scene->m_is_pause_event     = true;
}

bool Scene_Is_Hide(Scene* scene)
{
    return scene->m_is_pause_render;
}

void Scene_Hide(Scene* scene, bool is_pause)
{
    scene->m_is_pause_render = is_pause;
}

bool Scene_Is_Pause(Scene* scene)
{
    return scene->m_is_pause_event;
}

void Scene_Pause(Scene* scene, bool is_pause)
{
    scene->m_is_pause_event = is_pause;
}

////////////////////////////////////////////////////////////////////////////////
Viewport* Scene_Viewport_Get(const Scene* scene)
{
    return scene->m_viewport;
}

const strcrc* Scene_LocalName_Get(const Scene* scene)
{
    return &scene->m_local_name;
}

Actor* Scene_Actor_Create(const strcrc* local_name, Scene* scene, CB_ActorCreate_Void_Actor_tPtr cb_actor_create_void_actor_tptr, CB_ActorDestroy_Void_Actor cb_actor_destroy_void_actor, const void* ptr)
{
    Actor* actor = Actor_Create(local_name, scene, scene->m_alloc_actor_id++, cb_actor_create_void_actor_tptr, cb_actor_destroy_void_actor, ptr);
    Queue_Push(Actor*, local_name, scene->m_child_actor_queue, actor);
    return actor;
}

void Scene_Actor_Destroy(Scene* scene, CB_FindData_Bool_tPtr_tPtr cb_find_actor_bool_tptr_tptr, void* ptr)
{
    Queue_RemoveFindFirst(Actor*)(scene->m_child_actor_queue, cb_find_actor_bool_tptr_tptr, ptr, Actor_Destroy);
}

void Scene_Actor_Destroy_All(Scene* scene)
{
    Queue_Destroy(scene->m_child_actor_queue, Actor_Destroy);
}

void CallBack_Actor_Receive_SceneEvent(Actor* actor, const EventInfo* event_info);
void CallBack_Actor_Receive_ControlEvent(Actor* actor, const EventInfo* event_info);

void Scene_SceneEvent_SendTo_Actor(Scene* scene, EventInfo* event_info)
{
    if( event_info->m_event == Event_Scene_Update_Physics )
    {
        Scene_PhysicsGroup_Actor_Update(scene, event_info->m_delta_seconds);
    }
    else
    if( event_info->m_event == Event_Scene_Update_Anime )
    {
        TODO("Update Anime");
    }
    else
    {
        Event event = event_info->m_event;

        // Scene event
        Assert(IS_IN_RANGE(event, Event_Scene_Min, Event_Scene_Max), "");
        Queue_ForEach(Scene_EventQueue_Get(scene, event), CallBack_Actor_Receive_SceneEvent, event_info);
    }
}

void Scene_ControlEvent_SendTo_Actor(Scene* scene, EventInfo* event_info)
{
    Event event = event_info->m_event;

    Assert(IS_IN_RANGE(event, Event_Control_Min, Event_Control_Max), "");
    Queue_ForEach(scene->m_actor_queue_control, CallBack_Actor_Receive_ControlEvent, event_info);
}

void Scene_SceneEventGroup_Actor_Add(Scene* scene, Actor* actor, Event event)
{
    Assert(IS_IN_RANGE(event, Event_Scene_Min, Event_Scene_Max), "Invalid event!");
    Queue_Push(Actor*, NULL, Scene_EventQueue_Get(scene, event), actor);
}

void Scene_ControlEventGroup_Actor_Add(Scene* scene, Actor* actor, Event event)
{
    Assert(IS_IN_RANGE(event, Event_Control_Min, Event_Control_Max), "Invalid event!");
    Queue_Push(Actor*, NULL, scene->m_actor_queue_control, actor);
}

void Scene_PhysicsGroup_Actor_Add(Scene* scene, Actor* actor)
{
    Queue_Push(Actor*, NULL, scene->m_actor_queue_physics, actor);
}

void Scene_PhysicsGroup_Actor_Remove(Scene* scene, Actor* actor)
{
    Queue_RemoveFindFirst(Actor*)(scene->m_actor_queue_physics, NULL, actor, NULL);
}

void Scene_PhysicsGroup_Actor_Update(Scene* scene, float delta_seconds)
{
    float delta_second_copy = delta_seconds;
    Queue_ForEach(scene->m_actor_queue_physics, CallBack_Actor_Component_Physics_Simulate, &delta_second_copy);
}

void Scene_Storage_StoreData(Scene* scene, crc32 variable, tdata data)
{
    Storage_Data_Store(scene->m_storage, variable, data);
}

bool Scene_Storage_IsExistVariable(Scene* scene, crc32 variable)
{
    return
    Storage_Is_ExistVariable(scene->m_storage, variable);
}

tdata Scene_Storage_ReadData(Scene* scene, crc32 variable)
{
    return
    Storage_Data_Read(scene->m_storage, variable);
}

void Scene_Storage_DeleteVariable(Scene* scene, crc32 variable)
{
    Storage_Variable_Delete(scene->m_storage, variable);
}

void* Scene_ActorQueue_Child_Get(Scene* scene)
{
    return scene->m_child_actor_queue;
}

void* Scene_ActorQueue_Renderable_Get(Scene* scene)
{
    return scene->m_actor_queue_renderable;
}

void Scene_ActorQueue_Renderable_Add(Scene* scene, Actor* actor)
{
    Queue_Push(Actor*, NULL, Scene_ActorQueue_Renderable_Get(scene), actor);
}

void Scene_ActorQueue_Renderable_Remove(Scene* scene, Actor* actor)
{
    Queue_RemoveFindFirst(Actor*)(Scene_ActorQueue_Renderable_Get(scene), NULL, actor, NULL);
}
