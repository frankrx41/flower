#pragma once

typedef struct ShaderText ShaderText;
typedef struct Actor Actor;
typedef struct Component Component;


void*   ComponentData_Render_Create             (const strcrc* local_name, Actor* actor);
void    ComponentData_Render_Destroy            (Component* render_component);

void    Component_Render_ShaderText_Add         (Component* render_component, ShaderText* shader_text);
void    Component_Render_ShaderText_Del         (Component* render_component, ShaderText* shader_text);
void*   Component_Render_ShaderText_GetQueue    (Component* render_component);
void    Component_Render_ShaderText_ClearAll    (Component* render_component);
