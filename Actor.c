#include "CoreMini.h"

#include "MemoryManager.h"
#include "Actor.h"
#include "Component.h"

#include "Queue.h"
#include "String.h"


struct Actor
{
    uint32  m_id;
    strcrc  m_local_name;

    Queue(Component*)*  m_components_queue;

    Scene*  m_scene;
    bool    m_is_pause;
    bool    m_is_hide;
    CB_ActorDestroy_Void_Actor  m_cb_actor_destroy_void_actor;
};

Actor* Actor_Create(const strcrc* local_name, Scene* scene, uint32 id, CB_ActorCreate_Void_Actor_tPtr cb_actor_create_void_actor_tptr, CB_ActorDestroy_Void_Actor cb_actor_destroy_void_actor, void* ptr)
{
    Actor* actor = MemNew(local_name, Actor);
    actor->m_id                             = id;
    StrCrc_Copy(local_name, &actor->m_local_name);

    actor->m_components_queue = Queue_Create(local_name, Component*);

    actor->m_scene                          = scene;
    actor->m_is_pause                       = false;
    actor->m_is_hide                        = false;
    actor->m_cb_actor_destroy_void_actor    = cb_actor_destroy_void_actor;

    if( cb_actor_create_void_actor_tptr )
    {
        cb_actor_create_void_actor_tptr(actor, ptr);
    }

    return actor;
}

void Actor_Destroy(Actor* actor)
{
    Assert(actor != NULL, "");

    if( actor->m_cb_actor_destroy_void_actor )
    {
        actor->m_cb_actor_destroy_void_actor(actor);
    }

    Queue_Destroy(actor->m_components_queue, Component_Destroy);
    MemDel(actor);
}

bool Actor_IsPause(Actor* actor)
{
    return actor->m_is_pause;
}

void Actor_Pause(Actor* actor, bool is_pause)
{
    actor->m_is_pause = is_pause;
}

bool Actor_IsHide(Actor* actor)
{
    return actor->m_is_hide;
}

void Actor_Hide(Actor* actor, bool is_hide)
{
    actor->m_is_hide = is_hide;
}

////////////////////////////////////////////////////////////////////////////////
void Actor_Component_New(Actor* actor, ComponentEnum component_enum)
{
    Assert(actor != NULL, "");
    Assert(IS_IN_RANGE(component_enum, Component_Min, Component_Max), "");
    Assert(Actor_Component_Cast(actor, component_enum) == NULL, "");

    Component* component = Component_Create(Actor_GetLocalName(actor), actor, component_enum);
    Queue_Push(Component*, Actor_GetLocalName(actor), actor->m_components_queue, component);
}

void Actor_Component_Del(Actor* actor, ComponentEnum component_enum)
{
    Assert(actor != NULL, "");
    Assert(IS_IN_RANGE(component_enum, Component_Min, Component_Max), "");

    Component* component = Actor_Component_Cast(actor, component_enum);
    Assert(component != NULL, "You try to delete a not exist component!");

    Queue_RemoveFindFirst(Component*)(actor->m_components_queue, NULL, component, Component_Destroy);
}

static bool CallBack_Actor_FindComponent(const Component* component, const ComponentEnum component_enum)
{
    return Component_GetEnum(component) == component_enum;
}

Component* Actor_Component_Cast(const Actor* actor, ComponentEnum component_enum)
{
    return Queue_Find(Component*)(actor->m_components_queue, CallBack_Actor_FindComponent, (void*)component_enum);
}

////////////////////////////////////////////////////////////////////////////////
const strcrc* Actor_GetLocalName(Actor* actor)
{
    return &actor->m_local_name;
}

Scene* Actor_GetExistScene(const Actor* actor)
{
    return actor->m_scene;
}

