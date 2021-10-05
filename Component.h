#pragma once

typedef struct RenderComponent RenderComponent;
typedef struct ActionComponent ActionComponent;
typedef struct RenderData RenderData;
typedef struct Actor Actor;


RenderComponent*    Component_Render_Create             (const tchar* local_name);
void                Component_Render_Destroy            (RenderComponent* render_component);
tptr                Component_Render_GetRenderDataQueue (RenderComponent* render_component);
void                Component_Render_AddRenderData      (RenderComponent* render_component, RenderData* render_data);
void                Component_Render_ClearAllRenderData (RenderComponent* render_component);

void                Actor_RenderComponent_AddRenderData_Plat        (Actor* actor, uint32 x, uint32 y, const tchar* str);
void                Actor_RenderComponent_ClearAllRenderData        (Actor* actor);

ActionComponent*    Component_Action_Create         (const tchar* local_name);
void                Component_Action_Destroy        (ActionComponent* component);
