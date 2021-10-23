#pragma once

typedef struct PhysicsComponent PhysicsComponent;
typedef struct vec3 vec3;


PhysicsComponent*   Component_Physics_Create    (const strcrc* local_name, Actor* actor);
void                Component_Physics_Destroy   (PhysicsComponent* physics_component);

vec3*   Component_Physics_GetLocation       (PhysicsComponent* physics_component);
void    Component_Physics_SetLocation       (PhysicsComponent* physics_component, const vec3* vec);
void    Component_Physics_MoveLocation      (PhysicsComponent* physics_component, const vec3* offset_vec);
vec3*   Component_Physics_GetVelocity       (PhysicsComponent* physics_component);
void    Component_Physics_SetVelocity       (PhysicsComponent* physics_component, const vec3* velocity);
vec3*   Component_Physics_GetAcceleration   (PhysicsComponent* physics_component);
void    Component_Physics_SetAcceleration   (PhysicsComponent* physics_component, const vec3* acceleration);


void    Component_Physics_SetEnableSimulate (PhysicsComponent* physics_component, bool is_enable_simulater);
bool    Component_Physics_IsEnableSimulate  (PhysicsComponent* physics_component);

void    Component_Physics_Simulate          (PhysicsComponent* physics_component, float delta_seconds);
