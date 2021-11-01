#pragma once

typedef struct Actor Actor;
typedef struct Component Component;
typedef enum ComponentEnum ComponentEnum;

enum ComponentEnum
{
    Component_Min,
    Component_Render = Component_Min,
    Component_Control,
    Component_Physics,
    Component_Storage,
    Component_Max,
};

ComponentEnum Component_GetEnum     (const Component* component);

Component*  Component_Create        (const strcrc* local_name, Actor* actor, ComponentEnum component_enum);
void        Component_Destroy       (Component* component);
