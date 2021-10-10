#pragma once

typedef struct RenderManager RenderManager;
typedef struct SceneManager SceneManager;
typedef struct Scene Scene;

void    RenderManager_Render_BufferToScreen (RenderManager* render_manager);
void    RenderManager_Render_InBackBuffer   (RenderManager* render_manager, int32 x, int32 y, const tchar* str);

void    RenderManager_RenderAllScene        (RenderManager* render_manager, SceneManager* scene_manager);

RenderManager* RenderManager_GetInstance();
