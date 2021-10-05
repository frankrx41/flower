#pragma once

typedef struct RenderComponent RenderComponent;
typedef struct ActionComponent ActionComponent;
typedef struct RenderData2D RenderData2D;
typedef struct Actor Actor;


RenderComponent*    Component_Render_Create                 (const tchar* local_name);
void                Component_Render_Destroy                (RenderComponent* render_component);
tptr                Component_Render_GetRenderData2DQueue   (RenderComponent* render_component);
void                Component_Render_AddRenderData2D        (RenderComponent* render_component, RenderData2D* render_data);
void                Component_Render_ClearAllRenderData2D   (RenderComponent* render_component);

void                Actor_RenderComponent_AddRenderData2D       (Actor* actor, uint32 x, uint32 y, const tchar* str);
void                Actor_RenderComponent_ClearAllRenderData2D  (Actor* actor);

ActionComponent*    Component_Action_Create         (const tchar* local_name);
void                Component_Action_Destroy        (ActionComponent* component);
