#include "CoreMini.h"

#include "Vec.h"

const vec3 vec3_null = {0, 0, 0};
const vec2 vec2_null = {0, 0};

vec3 Vec3(float x, float y, float z)
{
    vec3 vec;
    vec.m_x = x;
    vec.m_y = y;
    vec.m_z = z;
    return vec;
}

vec3 Vec3_Add(vec3 v1, vec3 v2)
{
    return Vec3(v1.m_x + v2.m_x,
                v1.m_y + v2.m_y,
                v1.m_z + v2.m_z);
}

vec2 Vec2(float x, float y)
{
    vec2 vec;
    vec.m_x = x;
    vec.m_y = y;
    return vec;
}

aabb AABB(vec3 v1, vec3 v2)
{
    aabb aabb;
    aabb.m_v1 = v1;
    aabb.m_v2 = v2;
    return aabb;
}
