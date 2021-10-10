#pragma once

typedef struct RenderManager RenderManager;
typedef struct SceneManager SceneManager;
typedef struct ShaderText ShaderText;
typedef struct Scene Scene;
typedef struct vec3 vec3;
typedef enum RenderMode RenderMode;

enum RenderMode
{
    RenderMode_Text_Normal,
    RenderMode_Text_2x,
    RenderMode_Gui,
};

void    RenderManager_Render_ToScreen (RenderManager* render_manager);
void    RenderManager_Render_ToBackBuffer   (RenderManager* render_manager, vec3 vec, ShaderText* shader_text);

void    RenderManager_RenderAllScene        (RenderManager* render_manager, SceneManager* scene_manager);

void    RenderManager_RenderMode_Set        (RenderManager* render_manager, RenderMode );


RenderManager* RenderManager_GetInstance();
