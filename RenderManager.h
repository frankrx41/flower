#pragma once

typedef struct RenderManager RenderManager;
typedef struct Sence Sence;


void    RenderManager_SetPlatformData   (RenderManager* render_manager, tptr ptr);
tptr    RenderManager_GetPlatformData   (RenderManager* render_manager);
void    RenderManager_RenderSence       (RenderManager* render_manager, Sence* sence);
void    RenderManager_RenderToScreen    (RenderManager* render_manager);

RenderManager* RenderManager_GetInstance(const Engine* engine);
