#pragma once

typedef struct RenderManager RenderManager;
typedef struct Sence Sence;

void    RenderManager_RenderSence           (RenderManager* render_manager, Sence* sence);
void    RenderManager_Render_ToScreen       (RenderManager* render_manager);

void    RenderManager_Render_InBackBuffer   (RenderManager* render_manager, int32 x, int32 y, const tchar* str);


RenderManager* RenderManager_GetInstance();
