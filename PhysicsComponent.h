#pragma once

typedef struct PhysicsComponent PhysicsComponent;
typedef struct vec3 vec3;


PhysicsComponent*   Component_Physics_Create    (const tchar* local_name);
void                Component_Physics_Destroy   (PhysicsComponent* physics_component);

void    Component_Physics_SetLocation   (PhysicsComponent* physics_component, vec3 vec);
void    Component_Physics_MoveLocation  (PhysicsComponent* physics_component, vec3 offset_vec);
vec3    Component_Physics_GetLocation   (PhysicsComponent* physics_component);

