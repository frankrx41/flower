#include "CoreMini.h"

#include "Component.h"
#include "Actor.h"
#include "MemoryManager.h"
#include "Vec.h"

struct LocationComponent
{
    vec3 m_vec3;
};

LocationComponent* Component_Location_Create(const tchar* local_name)
{
    LocationComponent* location_component = MemNew(local_name, LocationComponent);
    location_component->m_vec3 = Vec3(0, 0, 0);
    return location_component;
}

void Component_Location_Destroy(LocationComponent* location_component)
{
    Assert(location_component != NULL, "");
    MemDel(location_component);
}


void Actor_Component_Location_Set(Actor* actor, vec3 vec)
{
    LocationComponent* location_component = Actor_Component_Cast(actor, Component_Location);
    if( location_component )
    {
        location_component->m_vec3 = vec;
    }
}

void Actor_Component_Location_Move(Actor* actor, vec3 offset_vec)
{
    LocationComponent* location_component = Actor_Component_Cast(actor, Component_Location);
    if( location_component )
    {
        location_component->m_vec3 = Vec3_Add(location_component->m_vec3, offset_vec);
    }
}

vec3 Actor_Component_Location_Get(Actor* actor)
{
    LocationComponent* location_component = Actor_Component_Cast(actor, Component_Location);
    if( location_component )
    {
        return location_component->m_vec3;
    }
    return vec3_null;
}


