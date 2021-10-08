#pragma once

typedef struct RenderManager RenderManager;
typedef struct Sence Sence;

void    RenderManager_RenderSence       (RenderManager* render_manager, Sence* sence);
void    RenderManager_RenderToScreen    (RenderManager* render_manager);

RenderManager* RenderManager_GetInstance();
