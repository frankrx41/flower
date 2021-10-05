#pragma once

typedef struct RenderComponent RenderComponent;
typedef struct ActionComponent ActionComponent;
typedef struct RenderData RenderData;


RenderComponent*    Component_Render_Create         (const tchar* local_name);
void                Component_Render_Destroy        (RenderComponent* component);

void                Component_Render_AddRenderData  (RenderComponent* render_component, RenderData* render_data);

ActionComponent*    Component_Action_Create         (const tchar* local_name);
void                Component_Action_Destroy        (ActionComponent* component);
