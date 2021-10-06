#pragma once

typedef struct RenderComponent RenderComponent;
typedef struct ActionComponent ActionComponent;
typedef struct RenderDataText RenderDataText;
typedef struct Actor Actor;


RenderComponent*    Component_Render_Create                 (const tchar* local_name);
void                Component_Render_Destroy                (RenderComponent* render_component);

RenderDataText*     Actor_RenderComponent_RenderDataText_Add        (Actor* actor, uint32 x, uint32 y, const tchar* str);
void                Actor_RenderComponent_RenderDataText_ClearAll   (Actor* actor);


ActionComponent*    Component_Action_Create         (const tchar* local_name);
void                Component_Action_Destroy        (ActionComponent* component);
