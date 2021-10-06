#pragma once

typedef struct RenderComponent RenderComponent;
typedef struct ActionComponent ActionComponent;
typedef struct RenderDataText RenderDataText;
typedef struct Actor Actor;

typedef enum Component Component;

enum Component
{
    Component_Render,
    Component_Action,
    Component_Location,
};

RenderComponent*    Component_Render_Create     (const tchar* local_name);
void                Component_Render_Destroy    (RenderComponent* render_component);

ActionComponent*    Component_Action_Create     (const tchar* local_name);
void                Component_Action_Destroy    (ActionComponent* component);

ActionComponent*    Component_Location_Create   (const tchar* local_name);
void                Component_Location_Destroy  (ActionComponent* component);
