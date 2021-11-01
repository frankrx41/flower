#include "CoreMini.h"

#include "MemoryManager.h"

#include "Component.h"
#include "Actor.h"

#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "ControlComponent.h"
#include "StorageComponent.h"

typedef void* (*CB_ComponentCreate_tPtr_tChar_Actor)    (const strcrc* local_name, Actor* actor);
typedef void (*CB_ComponentDestroy_Void_tPtr)           (void* component);


struct Component
{
    ComponentEnum   m_component_enum;
    Actor*          m_actor;
    void*           m_data;
};

static CB_ComponentCreate_tPtr_tChar_Actor Component_GetCreateCB(ComponentEnum component_enum)
{
    switch (component_enum)
    {
    case Component_Control: return Component_Control_Create;
    case Component_Physics: return Component_Physics_Create;
    case Component_Render:  return Component_Render_Create;
    case Component_Storage: return Component_Storage_Create;
    default: Assert(false, "");
    }
    return NULL;
}

static CB_ComponentDestroy_Void_tPtr Component_GetDestroyCB(ComponentEnum component_enum)
{
    switch (component_enum)
    {
    case Component_Control: return Component_Control_Destroy;
    case Component_Physics: return Component_Physics_Destroy;
    case Component_Render:  return Component_Render_Destroy;
    case Component_Storage: return Component_Storage_Destroy;
    default: Assert(false, "");
    }
    return NULL;
}

ComponentEnum Component_GetEnum(const Component* component)
{
    return component->m_component_enum;
}

Component* Component_Create(const strcrc* local_name, Actor* actor, ComponentEnum component_enum)
{
    Component* component = MemNew(local_name, Component);
    component->m_actor = actor;
    component->m_component_enum = component_enum;
    component->m_data = Component_GetCreateCB(component_enum)(local_name, actor);

    return component;
};

void Component_Destroy(Component* component)
{
    Component_GetDestroyCB(component->m_component_enum)(component);
    MemDel(component);
}
