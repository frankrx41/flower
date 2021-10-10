#include "CoreMini.h"

#include "MemoryManager.h"
#include "Actor.h"
#include "Component.h"
#include "LocationComponent.h"
#include "RenderComponent.h"
#include "ActionComponent.h"
#include "StorageComponent.h"

#include "Storage.h"
#include "String.h"
#include "tData.h"


struct Actor
{
    uint32                  m_id;
    String*                 m_local_name;
    Storage*                m_component;
    Scene*                  m_scene;
    CB_ActorDestroy_Void_Actor         m_cb_actor_destroy;
};

Actor* Actor_Create(const tchar* local_name, Scene* scene, uint32 id, CB_ActorCreate_Void_Actor_tPtr cb_actor_create, tptr ptr)
{
    Actor* actor = MemNew(local_name, Actor);
    actor->m_id                 = id;
    actor->m_local_name         = String_New(local_name, local_name, true);
    actor->m_component          = Storage_Create(local_name);
    actor->m_scene              = scene;
    actor->m_cb_actor_destroy   = NULL;
    if( cb_actor_create )
    {
        cb_actor_create(actor, ptr);
    }

    return actor;
}

void Actor_Set_CB_Destroy(Actor* actor, CB_ActorDestroy_Void_Actor cb_actor_destroy)
{
    actor->m_cb_actor_destroy = cb_actor_destroy;
}

void Actor_Destroy(Actor* actor)
{
    Assert(actor != NULL, "");

    if( actor->m_cb_actor_destroy )
    {
        actor->m_cb_actor_destroy(actor);
    }

    Actor_Component_Del(actor, Component_Render);
    Actor_Component_Del(actor, Component_Action);
    Actor_Component_Del(actor, Component_Storage);
    Actor_Component_Del(actor, Component_Location);

    Storage_Destroy(actor->m_component);
    String_Del(actor->m_local_name);
    MemDel(actor);
}

////////////////////////////////////////////////////////////////////////////////

#undef Actor_Component_Del
#undef Actor_Component_New
#undef Actor_Component_Cast

void Actor_Component_New(Actor* actor, const tchar* component_name, Component component_enum, CB_ComponentCreate_tPtr_tChar cb_component_create)
{
    Assert(actor != NULL, "");
    Assert(cb_component_create != NULL, "");
    const tptr component = cb_component_create(Actor_GetLocalName(actor));
    Storage_StoreData(actor->m_component, Str_CalcCrc(component_name, 0), tData(tptr, component));
}

static tptr Actor_Component_CastByName(Actor* actor, const tchar* component_name)
{
    Assert(actor != NULL, "");
    const tptr ptr = Storage_ReadData(actor->m_component, Str_CalcCrc(component_name, 0)).m_tptr;
    return ptr;
}

void Actor_Component_Del(Actor* actor, const tchar* component_name, Component component_enum, CB_ComponentDestroy_Void_tPtr cb_component_destroy)
{
    Assert(actor != NULL, "");
    const tptr component = Actor_Component_CastByName(actor, component_name);
    if( component )
    {
        cb_component_destroy(component);
        Storage_DeleteVariable(actor->m_component,Str_CalcCrc(component_name, 0));
    }
    // Assert(false, "You try to delete a not exist component!");
}

tptr Actor_Component_Cast(Actor* actor, const tchar* component_name, Component component_enum)
{
    return Actor_Component_CastByName(actor, component_name);
}

const tchar* Actor_GetLocalName(Actor* actor)
{
    return String_CStr(actor->m_local_name);
}

Scene* Actor_GetScene(Actor* actor)
{
    return actor->m_scene;
}

