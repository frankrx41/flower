#include "CoreMini.h"

#include "MemoryManager.h"
#include "Actor.h"
#include "Component.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "ActionComponent.h"
#include "StorageComponent.h"

#include "String.h"


struct Actor
{
    uint32                      m_id;
    String*                     m_local_name;

    tptr                        m_components[Component_Max - Component_Min];

    Scene*                      m_scene;
    bool                        m_is_pause;
    CB_ActorDestroy_Void_Actor  m_cb_actor_destroy_void_actor;
};

Actor* Actor_Create(const tchar* local_name, Scene* scene, uint32 id, CB_ActorCreate_Void_Actor_tPtr cb_actor_create_void_actor_tptr, tptr ptr)
{
    Actor* actor = MemNew(local_name, Actor);
    actor->m_id                             = id;
    actor->m_local_name                     = String_New(local_name, local_name, true);

    for( uint32 i=0, max_i = ARRAY_SIZE(actor->m_components); i<max_i; i++ )
    {
        actor->m_components[i]              = NULL;
    }

    actor->m_scene                          = scene;
    actor->m_is_pause                       = false;
    actor->m_cb_actor_destroy_void_actor    = NULL;

    if( cb_actor_create_void_actor_tptr )
    {
        cb_actor_create_void_actor_tptr(actor, ptr);
    }

    return actor;
}

void Actor_Destroy_CB_Set(Actor* actor, CB_ActorDestroy_Void_Actor cb_actor_destroy_void_actor)
{
    actor->m_cb_actor_destroy_void_actor = cb_actor_destroy_void_actor;
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
    
    String_Del(actor->m_local_name);
    MemDel(actor);
}

bool Actor_IsPause(Actor* actor)
{
    return actor->m_is_pause;
}

void Actor_SetIsPause(Actor* actor, bool is_pause)
{
    actor->m_is_pause = is_pause;
}

////////////////////////////////////////////////////////////////////////////////
typedef tptr(*CB_ComponentCreate_tPtr_tChar)   (const tchar* local_name);
typedef void (*CB_ComponentDestroy_Void_tPtr)   (tptr component);

static CB_ComponentCreate_tPtr_tChar Actor_Component_Create_CB_Get(Component component_enum)
{
    switch(component_enum)
    {
    case Component_Action:  return Component_Action_Create;
    case Component_Physics: return Component_Physics_Create;
    case Component_Render:  return Component_Render_Create;
    case Component_Storage: return Component_Storage_Create;
    default: Assert(false, "");
    }
    return NULL;
}

static  CB_ComponentDestroy_Void_tPtr Actor_Component_Destroy_CB_Get(Component component_enum)
{
    switch (component_enum)
    {
    case Component_Action:  return Component_Action_Destroy;
    case Component_Physics: return Component_Physics_Destroy;
    case Component_Render:  return Component_Render_Destroy;
    case Component_Storage: return Component_Storage_Destroy;
    default: Assert(false, "");
    }
    return NULL;
}

static void Actor_Component_Set(Actor* actor, Component component_enum, tptr component)
{
    Assert(IN_RANGE(component_enum, Component_Min, Component_Max), "");
    actor->m_components[component_enum - Component_Min] = component;
}

void Actor_Component_New(Actor* actor, Component component_enum)
{
    Assert(actor != NULL, "");
    Assert(IN_RANGE(component_enum, Component_Min, Component_Max), "");

    const tptr component = Actor_Component_Create_CB_Get(component_enum)(Actor_GetLocalName(actor));
    Actor_Component_Set(actor, component_enum, component);
}

void Actor_Component_Del(Actor* actor, Component component_enum)
{
    Assert(actor != NULL, "");
    Assert(IN_RANGE(component_enum, Component_Min, Component_Max), "");

    const tptr component = Actor_Component_Cast(actor, component_enum);
    Assert(component != NULL, "You try to delete a not exist component!");
    
    Actor_Component_Destroy_CB_Get(component_enum)(component);
    Actor_Component_Set(actor, component_enum, NULL);
}

tptr Actor_Component_Cast(Actor* actor, Component component_enum)
{
    Assert(IN_RANGE(component_enum, Component_Min, Component_Max), "");
    return actor->m_components[component_enum - Component_Min];
}

const tchar* Actor_GetLocalName(Actor* actor)
{
    return String_CStr(actor->m_local_name);
}

Scene* Actor_GetScene(Actor* actor)
{
    return actor->m_scene;
}

