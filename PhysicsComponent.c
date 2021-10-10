#include "CoreMini.h"

#include "Actor.h"
#include "MemoryManager.h"
#include "Vec.h"
#include "PhysicsComponent.h"

struct PhysicsComponent
{
    bool    m_is_enable_simulate;
    vec3    m_displacement;
    vec3    m_velocity;
    vec3    m_acceleration;
};

PhysicsComponent* Component_Physics_Create(const tchar* local_name)
{
    PhysicsComponent* physics_component = MemNew(local_name, PhysicsComponent);
    physics_component->m_is_enable_simulate = false;
    physics_component->m_displacement       = vec3_null;
    physics_component->m_velocity       = vec3_null;
    physics_component->m_acceleration   = vec3_null;
    return physics_component;
}

void Component_Physics_Destroy(PhysicsComponent* physics_component)
{
    Assert(physics_component != NULL, "");
    MemDel(physics_component);
}

vec3 Component_Physics_GetLocation(PhysicsComponent* physics_component)
{
    Assert(physics_component != NULL, "");
    return physics_component->m_displacement;
}

void Component_Physics_SetLocation(PhysicsComponent* physics_component, vec3 vec)
{
    Assert(physics_component != NULL, "");
    physics_component->m_displacement = vec;
}

void Component_Physics_MoveLocation(PhysicsComponent* physics_component, vec3 offset_vec)
{
    Assert(physics_component != NULL, "");
    physics_component->m_displacement = Vec3_Add(physics_component->m_displacement, offset_vec);
}

vec3 Component_Physics_GetVelocity(PhysicsComponent* physics_component)
{
    Assert(physics_component != NULL, "");
    return physics_component->m_velocity;
}

void Component_Physics_SetVelocity(PhysicsComponent* physics_component, vec3 velocity)
{
    Assert(physics_component != NULL, "");
    physics_component->m_velocity = velocity;
}

vec3 Component_Physics_GetAcceleration(PhysicsComponent* physics_component)
{
    Assert(physics_component != NULL, "");
    return physics_component->m_acceleration;
}

void Component_Physics_SetAcceleration(PhysicsComponent* physics_component, vec3 acceleration)
{
    Assert(physics_component != NULL, "");
    physics_component->m_acceleration = acceleration;
}



////////////////////////////////////////////////////////////////////////////////
void Component_Physics_SetEnableSimulate(PhysicsComponent* physics_component, bool is_enable_simulater)
{
    physics_component->m_is_enable_simulate = is_enable_simulater;
}

bool Component_Physics_IsEnableSimulate(PhysicsComponent* physics_component)
{
    return physics_component->m_is_enable_simulate;
}

void Component_Physics_Simulate(PhysicsComponent* physics_component, float delta_seconds)
{
    Assert( physics_component->m_is_enable_simulate != false, "" );
    // v = v + at
    physics_component->m_velocity = Vec3_Add(physics_component->m_velocity, Vec3_Multiply(physics_component->m_acceleration, delta_seconds));

    // s = s + vt
    physics_component->m_displacement = Vec3_Add(physics_component->m_displacement, Vec3_Multiply(physics_component->m_velocity, delta_seconds));
}
