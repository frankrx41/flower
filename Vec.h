#pragma once

typedef struct vec2 vec2;
typedef struct vec3 vec3;

extern const vec3 vec3_null;
extern const vec2 vec2_null;

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


vec3 Vec3(float x, float y, float z);
vec3 Vec3_Add(vec3 v1, vec3 v2);
vec2 Vec2(float x, float y);
