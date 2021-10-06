#include "CoreMini.h"

#include "Vec.h"

vec3 Vec3(float x, float y, float z)
{
    vec3 vec;
    vec.m_x = x;
    vec.m_y = y;
    vec.m_z = z;
    return vec;
}

vec2 Vec2(float x, float y)
{
    vec2 vec;
    vec.m_x = x;
    vec.m_y = y;
    return vec;
}
