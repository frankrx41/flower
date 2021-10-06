#pragma once

typedef struct RenderComponent RenderComponent;
typedef struct ShaderText ShaderText;


RenderComponent*    Component_Render_Create                 (const tchar* local_name);
void                Component_Render_Destroy                (RenderComponent* render_component);

void    Component_Render_ShaderText_Add         (RenderComponent* render_component, ShaderText* render_data);
void    Component_Render_ShaderText_Del         (RenderComponent* render_component, ShaderText* render_data);
tptr    Component_Render_ShaderText_GetQueue    (RenderComponent* render_component);
void    Component_Render_ShaderText_ClearAll    (RenderComponent* render_component);
