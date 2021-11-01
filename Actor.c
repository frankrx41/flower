#include "CoreMini.h"

#include "MemoryManager.h"
#include "Actor.h"
#include "Component.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "ControlComponent.h"
#include "StorageComponent.h"

#include "String.h"


struct Actor
{
    uint32  m_id;
    strcrc  m_local_name;

    void*   m_components[Component_Max - Component_Min];

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

    for( uint32 i=0, max_i = ARRAY_SIZE(actor->m_components); i<max_i; i++ )
    {
        actor->m_components[i]              = NULL;
    }

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

    for(Component component_enum = Component_Min; component_enum<Component_Max; component_enum++)
    {
        if( Actor_Component_Cast(actor, component_enum) )
        {
            Actor_Component_Del(actor, component_enum);
        }
    }
    
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
typedef void* (*CB_ComponentCreate_tPtr_tChar_Actor)    (const strcrc* local_name, Actor* actor);
typedef void (*CB_ComponentDestroy_Void_tPtr)           (void* component);

static CB_ComponentCreate_tPtr_tChar_Actor Actor_Component_Create_CB_Get(Component component_enum)
{
    switch(component_enum)
    {
    case Component_Control: return Component_Control_Create;
    case Component_Physics: return Component_Physics_Create;
    case Component_Render:  return Component_Render_Create;
    case Component_Storage: return Component_Storage_Create;
    default: Assert(false, "");
    }
    return NULL;
}

static CB_ComponentDestroy_Void_tPtr Actor_Component_Destroy_CB_Get(Component component_enum)
{
    switch (component_enum)
    {
    case Component_Control:  return Component_Control_Destroy;
    case Component_Physics: return Component_Physics_Destroy;
    case Component_Render:  return Component_Render_Destroy;
    case Component_Storage: return Component_Storage_Destroy;
    default: Assert(false, "");
    }
    return NULL;
}

static void Actor_Component_Set(Actor* actor, Component component_enum, void* component)
{
    Assert(IS_IN_RANGE(component_enum, Component_Min, Component_Max), "");
    actor->m_components[component_enum - Component_Min] = component;
}

void Actor_Component_New(Actor* actor, Component component_enum)
{
    Assert(actor != NULL, "");
    Assert(IS_IN_RANGE(component_enum, Component_Min, Component_Max), "");

    void* component = Actor_Component_Create_CB_Get(component_enum)(Actor_GetLocalName(actor), actor);
    Actor_Component_Set(actor, component_enum, component);
}

void Actor_Component_Del(Actor* actor, Component component_enum)
{
    Assert(actor != NULL, "");
    Assert(IS_IN_RANGE(component_enum, Component_Min, Component_Max), "");

    void* component = Actor_Component_Cast(actor, component_enum);
    Assert(component != NULL, "You try to delete a not exist component!");
    
    Actor_Component_Destroy_CB_Get(component_enum)(component);
    Actor_Component_Set(actor, component_enum, NULL);
}

void* Actor_Component_Cast(const Actor* actor, Component component_enum)
{
    Assert(IS_IN_RANGE(component_enum, Component_Min, Component_Max), "");
    return actor->m_components[component_enum - Component_Min];
}

const strcrc* Actor_GetLocalName(Actor* actor)
{
    return &actor->m_local_name;
}

Scene* Actor_GetExistScene(const Actor* actor)
{
    return actor->m_scene;
}

