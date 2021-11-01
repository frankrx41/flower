#pragma once

typedef union tdata tdata;
typedef struct Actor Actor;
typedef struct strcrc strcrc;
typedef struct Component Component;


void*   Component_Storage_Create            (const strcrc* local_name, Actor* actor);
void    Component_Storage_Destroy           (Component* component);

bool    Component_Storage_Is_ExistVariable  (Component* component, crc32 variable);
void    Component_Storage_Data_Store        (Component* component, crc32 variable, tdata data);
tdata   Component_Storage_Data_Read         (Component* component, crc32 variable);
void    Component_Storage_Variable_Delete   (Component* component, crc32 variable);
