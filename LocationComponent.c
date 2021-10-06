#include "CoreMini.h"

#include "Actor.h"
#include "MemoryManager.h"
#include "Vec.h"
#include "LocationComponent.h"

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

void Component_Location_Set(LocationComponent* location_component, vec3 vec)
{
    Assert(location_component != NULL, "");
    location_component->m_vec3 = vec;
}

void Component_Location_Move(LocationComponent* location_component, vec3 offset_vec)
{
    Assert(location_component != NULL, "");
    location_component->m_vec3 = Vec3_Add(location_component->m_vec3, offset_vec);
}

vec3 Component_Location_Get(LocationComponent* location_component)
{
    Assert(location_component != NULL, "");
    return location_component->m_vec3;
}
