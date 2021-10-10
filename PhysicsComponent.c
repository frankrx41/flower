#include "CoreMini.h"

#include "Actor.h"
#include "MemoryManager.h"
#include "Vec.h"
#include "PhysicsComponent.h"

struct PhysicsComponent
{
    vec3 m_location;
    vec3 m_speed;
    vec3 m_acceleration;
};

PhysicsComponent* Component_Physics_Create(const tchar* local_name)
{
    PhysicsComponent* physics_component = MemNew(local_name, PhysicsComponent);
    physics_component->m_location  = vec3_null;
    physics_component->m_speed     = vec3_null;
    physics_component->m_acceleration = vec3_null;
    return physics_component;
}

void Component_Physics_Destroy(PhysicsComponent* physics_component)
{
    Assert(physics_component != NULL, "");
    MemDel(physics_component);
}

void Component_Physics_SetLocation(PhysicsComponent* physics_component, vec3 vec)
{
    Assert(physics_component != NULL, "");
    physics_component->m_location = vec;
}

void Component_Physics_MoveLocation(PhysicsComponent* physics_component, vec3 offset_vec)
{
    Assert(physics_component != NULL, "");
    physics_component->m_location = Vec3_Add(physics_component->m_location, offset_vec);
}

vec3 Component_Physics_GetLocation(PhysicsComponent* physics_component)
{
    Assert(physics_component != NULL, "");
    return physics_component->m_location;
}
