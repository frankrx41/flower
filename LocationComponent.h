#pragma once

typedef struct LocationComponent LocationComponent;
typedef struct vec3 vec3;


LocationComponent*  Component_Location_Create   (const tchar* local_name);
void                Component_Location_Destroy  (LocationComponent* location_component);

void    Component_Location_Set  (LocationComponent* location_component, vec3 vec);
void    Component_Location_Move (LocationComponent* location_component, vec3 offset_vec);
vec3    Component_Location_Get  (LocationComponent* location_component);

