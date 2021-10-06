#pragma once

struct vec2
{
    float m_x;
    float m_y;
};

struct vec3
{
    float m_x;
    float m_y;
    float m_z;
};

typedef struct vec2 vec2;
typedef struct vec3 vec3;

vec3 Vec3(float x, float y, float z);
vec2 Vec2(float x, float y);
