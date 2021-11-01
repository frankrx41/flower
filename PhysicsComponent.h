#pragma once

typedef struct vec3 vec3;
typedef struct Component Component;


void*   ComponentData_Physics_Create        (const strcrc* local_name, Actor* actor);
void    ComponentData_Physics_Destroy       (Component* component);

vec3*   Component_Physics_GetLocation       (Component* component);
void    Component_Physics_SetLocation       (Component* component, const vec3* vec);
void    Component_Physics_MoveLocation      (Component* component, const vec3* offset_vec);
vec3*   Component_Physics_GetVelocity       (Component* component);
void    Component_Physics_SetVelocity       (Component* component, const vec3* velocity);
vec3*   Component_Physics_GetAcceleration   (Component* component);
void    Component_Physics_SetAcceleration   (Component* component, const vec3* acceleration);


void    Component_Physics_SetEnableSimulate (Component* component, bool is_enable_simulate);
bool    Component_Physics_IsEnableSimulate  (Component* component);

void    Component_Physics_Simulate          (Component* component, float delta_seconds);
