#include "CoreMini.h"

#include "Vec.h"

#if CONFIG_DEBUG

#define WITH_CHECK_NULL_OUT 1

#endif

const vec3 vec3_zero = {0, 0, 0};
const vec3 vec3_unit = {0, 0, 0};
const vec2 vec2_zero = {0, 0};
const vec2 vec2_unit = {1, 1};

vec3 Vec3(float x, float y, float z)
{
    vec3 vec;
    vec.m_x = x;
    vec.m_y = y;
    vec.m_z = z;
    return vec;
}

vec3* Vec3_Add(vec3* v1, vec3* v2, vec3* out)
{
    Assert(out != NULL, "");

    out->m_x = v1->m_x + v2->m_x;
    out->m_y = v1->m_y + v2->m_y;
    out->m_z = v1->m_z + v2->m_z;

    return out;
}

vec3* Vec3_Multiply(vec3* v, float k, vec3* out)
{
    Assert(out != NULL, "");

    out->m_x = v->m_x * k;
    out->m_y = v->m_y * k;
    out->m_z = v->m_z * k;

    return out;
}

bool Vec3_IsZero(vec3* v)
{
    return v->m_x == 0 && v->m_y == 0 && v->m_z == 0;
}

////////////////////////////////////////////////////////////////////////////////
vec2 Vec2(float x, float y)
{
    vec2 vec;
    vec.m_x = x;
    vec.m_y = y;
    return vec;
}

////////////////////////////////////////////////////////////////////////////////
aabb AABB(vec3* v1, vec3* v2)
{
    aabb aabb;
    aabb.m_v1 = *v1;
    aabb.m_v2 = *v2;
    return aabb;
}
