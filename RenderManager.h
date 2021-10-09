#pragma once

typedef struct RenderManager RenderManager;
typedef struct Scene Scene;

void    RenderManager_RenderScene           (RenderManager* render_manager, Scene* scene);
void    RenderManager_Render_ToScreen       (RenderManager* render_manager);

void    RenderManager_Render_InBackBuffer   (RenderManager* render_manager, int32 x, int32 y, const tchar* str);


RenderManager* RenderManager_GetInstance();
