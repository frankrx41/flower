#include "CoreMini.h"

#include "Storage.h"
#include "Event.h"
#include "MemoryManager.h"
#include "String.h"
#include "Actor.h"


#include "ActorComponent.h"
#include "Component.h"
#include "Data32.h"
#include "LocationComponent.h"
#include "Queue.h"
#include "RenderComponent.h"
#include "Vec.h"


#undef Actor_Component_Del
#undef Actor_Component_New
#undef Actor_Component_Cast

struct Actor
{
    uint32                  m_id;
    String*                 m_local_name;
    Storage*                m_component;
    Sence*                  m_sence;
};

Actor* Actor_Create(const tchar* local_name, Sence* sence, uint32 id)
{
    Actor* actor = MemNew(local_name, Actor);
    actor->m_id         = id;
    actor->m_local_name = String_New(local_name, local_name);
    actor->m_component  = Storage_Create(local_name);
    actor->m_sence      = sence;
    return actor;
}

void Actor_Destroy(Actor* actor)
{
    Storage_Destroy(actor->m_component);
    String_Del(actor->m_local_name);
    MemDel(actor);
}

void Actor_Component_New(Actor* actor, const tchar* component_name, Component component_enum, CB_ComponentCreate cb_component_create_func)
{
    Assert(cb_component_create_func != NULL, "");
    tptr component = cb_component_create_func(Actor_GetLocalName(actor));
    Storage_StoreData32(actor->m_component, Str_CalcCrc(component_name, 0), Data32(tptr, component));
}

static tptr Actor_Component_CastByName(Actor* actor, const tchar* component_name)
{
    Assert(actor != NULL, "");
    tptr ptr = Storage_ReadData32(actor->m_component, Str_CalcCrc(component_name, 0)).m_pointer;
    return ptr;
}

void Actor_Component_Del(Actor* actor, const tchar* component_name, Component component_enum, CB_ComponentDestroy cb_component_destroy_func)
{
    tptr component = Actor_Component_CastByName(actor, component_name);
    cb_component_destroy_func(component);
    Storage_DeleteVariable(actor->m_component,Str_CalcCrc(component_name, 0));
}

tptr Actor_Component_Cast(Actor* actor, const tchar* component_name, Component component_enum)
{
    return Actor_Component_CastByName(actor, component_name);
}

const tchar* Actor_GetLocalName(Actor* actor)
{
    return String_CStr(actor->m_local_name);
}

Sence* Actor_GetSence(Actor* actor)
{
    return actor->m_sence;
}

