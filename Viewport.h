#pragma once

typedef struct Viewport Viewport;
typedef struct vec2 vec2;
typedef struct strcrc strcrc;
typedef struct ShaderText ShaderText;

Viewport*   Viewport_Create     (const strcrc* local_name, float width, float height, const vec2* scale, const vec2* offset);
void        Viewport_Destroy    (Viewport* viewport);

void        Viewport_Clean      (Viewport* viewport);

void        Viewport_RenderTo_Screen    (const Viewport* back_viewport, const Viewport* front_viewport);
void        Viewport_RenderTo_Viewport  (const Viewport* viewport, Viewport* out_viewport);
void        Viewport_Render_ShaderText  (Viewport* viewport, ShaderText* shader_text);
