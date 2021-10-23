#pragma once

typedef struct vec2 vec2;
typedef struct vec3 vec3;
typedef struct aabb aabb;

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

struct aabb
{
    vec3 m_v1;
    vec3 m_v2;
};

vec3    Vec3            (float x, float y, float z);
vec3*   Vec3_Add        (vec3* v1, vec3* v2, vec3* out);
vec3*   Vec3_Multiply   (vec3* v, float k, vec3* out);
vec3*   Vec3_Cross      (vec3* v1, vec3* v2, vec3* out);
float   Vec3_Dot        (vec3* v1, vec3* v2);
bool    Vec3_IsZero     (vec3* v);

vec2    Vec2            (float x, float y);


aabb    AABB            (vec3* v1, vec3* v2);
