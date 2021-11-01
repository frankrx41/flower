#include "CoreMini.h"

#include "MemoryManager.h"

#include "Component.h"
#include "PhysicsComponent.h"

#include "Scene.h"
#include "Actor.h"
#include "Vec.h"

typedef struct PhysicsComponentData PhysicsComponentData;
struct PhysicsComponentData
{
    bool    m_is_enable_simulate;
    vec3    m_displacement;
    vec3    m_velocity;
    vec3    m_acceleration;
};

void* ComponentData_Physics_Create(const strcrc* local_name, Actor* actor)
{
    PhysicsComponentData* physics_component = MemNew(local_name, PhysicsComponentData);
    physics_component->m_is_enable_simulate = false;
    physics_component->m_displacement       = vec3_zero;
    physics_component->m_velocity       = vec3_zero;
    physics_component->m_acceleration   = vec3_zero;
    return physics_component;
}

void ComponentData_Physics_Destroy(Component* component)
{
    Assert(component != NULL, "");
    Assert(Component_GetEnum(component) == Component_Physics, "");

    PhysicsComponentData* physics_component_data = Component_GetData(component);

    MemDel(physics_component_data);
}

vec3* Component_Physics_GetLocation(Component* component)
{
    Assert(component != NULL, "");
    Assert(Component_GetEnum(component) == Component_Physics, "");

    PhysicsComponentData* physics_component_data = Component_GetData(component);
    return &physics_component_data->m_displacement;
}

void Component_Physics_SetLocation(Component* component, const vec3* vec)
{
    Assert(component != NULL, "");
    Assert(Component_GetEnum(component) == Component_Physics, "");

    PhysicsComponentData* physics_component_data = Component_GetData(component);
    physics_component_data->m_displacement = *vec;
}

void Component_Physics_MoveLocation(Component* component, const vec3* offset_vec)
{
    Assert(component != NULL, "");
    PhysicsComponentData* physics_component_data = Component_GetData(component);
    Vec3_Add(&physics_component_data->m_displacement, offset_vec, &physics_component_data->m_displacement);
}

vec3* Component_Physics_GetVelocity(Component* component)
{
    Assert(component != NULL, "");
    Assert(Component_GetEnum(component) == Component_Physics, "");

    PhysicsComponentData* physics_component_data = Component_GetData(component);
    return &physics_component_data->m_velocity;
}

void Component_Physics_SetVelocity(Component* component, const vec3* velocity)
{
    Assert(component != NULL, "");
    Assert(Component_GetEnum(component) == Component_Physics, "");

    PhysicsComponentData* physics_component_data = Component_GetData(component);
    physics_component_data->m_velocity = *velocity;
}

vec3* Component_Physics_GetAcceleration(Component* component)
{
    Assert(component != NULL, "");
    Assert(Component_GetEnum(component) == Component_Physics, "");

    PhysicsComponentData* physics_component_data = Component_GetData(component);
    return &physics_component_data->m_acceleration;
}

void Component_Physics_SetAcceleration(Component* component, const vec3* acceleration)
{
    Assert(component != NULL, "");
    PhysicsComponentData* physics_component_data = Component_GetData(component);
    physics_component_data->m_acceleration = *acceleration;
}


////////////////////////////////////////////////////////////////////////////////
void Component_Physics_SetEnableSimulate(Component* component, bool is_enable_simulate)
{
    Assert(component != NULL, "");
    Assert(Component_GetEnum(component) == Component_Physics, "");

    PhysicsComponentData* physics_component_data = Component_GetData(component);
    // default is false, not do simulate
    if( physics_component_data->m_is_enable_simulate == is_enable_simulate )
    {
        return;
    }

    physics_component_data->m_is_enable_simulate = is_enable_simulate;
    Actor* actor = Component_GetActor(component);

    if( is_enable_simulate )
    {
        Scene_PhysicsGroup_Actor_Add(Actor_GetExistScene(actor), actor);
    }
    else
    {
        Scene_PhysicsGroup_Actor_Remove(Actor_GetExistScene(actor), actor);
    }
}

bool Component_Physics_IsEnableSimulate(Component* component)
{
    Assert(component != NULL, "");
    Assert(Component_GetEnum(component) == Component_Physics, "");

    PhysicsComponentData* physics_component_data = Component_GetData(component);
    return physics_component_data->m_is_enable_simulate;
}

void Component_Physics_Simulate(Component* component, float delta_seconds)
{
    Assert(component != NULL, "");
    Assert(Component_GetEnum(component) == Component_Physics, "");

    PhysicsComponentData* physics_component_data = Component_GetData(component);
    Assert(physics_component_data->m_is_enable_simulate != false, "" );
    // s = s + vt + 1/2 a t^2
    vec3 vec3_temp;
    vec3 s_vt = *Vec3_Add(&physics_component_data->m_displacement, Vec3_Multiply(&physics_component_data->m_velocity, delta_seconds, &vec3_temp), &vec3_temp);
    vec3 a_tt = vec3_zero;
    if( Vec3_IsZero(&physics_component_data->m_acceleration) )
    {
        a_tt = *Vec3_Multiply(&physics_component_data->m_acceleration, delta_seconds * delta_seconds * .5f, &vec3_temp);
    }
    physics_component_data->m_displacement = *Vec3_Add(&s_vt, &a_tt, &vec3_temp);

    // v = v + at
    physics_component_data->m_velocity = *Vec3_Add(&physics_component_data->m_velocity, Vec3_Multiply(&physics_component_data->m_acceleration, delta_seconds, &vec3_temp), &vec3_temp);

}
