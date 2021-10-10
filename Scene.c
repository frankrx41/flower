#include "CoreMini.h"

#include "Event.h"
#include "KeyId.h"

#include "MemoryManager.h"
#include "EventManager.h"

#include "Scene.h"
#include "Actor.h"

#include "Queue.h"
#include "String.h"
#include "tData.h"
#include "Storage.h"


struct Scene
{
    uint32          m_alloc_actor_id;
    String*         m_local_name;
    Storage*        m_storage;
    Queue(Actor*)*  m_actor_queue;
    Queue(Actor*)*  m_actor_event_queue_list[Event_Max];
};

Scene* Scene_Create(const tchar* local_name)
{
    Scene* scene = MemNew(local_name, Scene);
    scene->m_actor_queue    = Queue_Create(local_name, Actor*);
    scene->m_alloc_actor_id = 0;
    scene->m_local_name     = String_New(local_name, local_name);
    scene->m_storage        = Storage_Create(local_name);
    for(uint32 i=0; i<Event_Max; i++)
    {
        scene->m_actor_event_queue_list[i] = Queue_Create(local_name, Actor*);
    }

    return scene;
}

void Scene_Destroy(Scene* scene)
{
    Queue_Destroy(scene->m_actor_queue, Actor_Destroy);
    for(uint32 i=0; i<Event_Max; i++)
    {
        Queue_Destroy(scene->m_actor_event_queue_list[i], NULL);
    }
    String_Del(scene->m_local_name);
    Storage_Destroy(scene->m_storage);
    MemDel(scene);
}

Actor* Scene_Actor_Create(const tchar* local_name, Scene* scene, CB_ActorCreate_Void_Actor_tPtr cb_actor_create, tptr ptr)
{
    Actor* actor = Actor_Create(local_name, scene, scene->m_alloc_actor_id++, cb_actor_create, ptr);
    Queue_Push(Actor*, local_name, scene->m_actor_queue, actor);
    return actor;
}

void Scene_Actor_Destroy(Scene* scene, CB_FindData_Bool_tPtr_tPtr cb_find_actor, tptr ptr)
{
    if( cb_find_actor == NULL )
    {
        Queue_RemoveFindFirst(Actor*)(scene->m_actor_queue, NULL, ptr);
        Actor_Destroy(ptr);
    }
    else
    {
        Actor* actor = Queue_RemoveFindFirst(Actor*)(scene->m_actor_queue, cb_find_actor, ptr);
        Actor_Destroy(actor);
    }
}

void Scene_Actor_Destroy_All(Scene* scene)
{
    Queue_Destroy(scene->m_actor_queue, Actor_Destroy);
}

void CallBack_Actor_ProcessEachActorEvent(Actor* actor, EventInfo* event_struct);

void Scene_Actor_SendEvent(Scene* scene, EventInfo* event_info)
{
    Event event = event_info->m_event;
    Queue_ForEach(scene->m_actor_event_queue_list[event], (CB_ProcessData_Void_tPtr_tPtr)CallBack_Actor_ProcessEachActorEvent, event_info);
}

void Scene_Actor_AddEventGroup(Scene* scene, Actor* actor, Event event)
{
    Assert(event > Event_Null && event < Event_Max, "Invalid event!");
    Queue_Push(Actor*, NULL, scene->m_actor_event_queue_list[event], actor);
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

tptr Scene_GetActorQueue(Scene* scene)
{
    return scene->m_actor_queue;
}
