#pragma once

typedef struct Viewport Viewport;
typedef struct vec2 vec2;
typedef struct strcrc strcrc;
typedef struct ShaderText ShaderText;
typedef struct RenderManager RenderManager;

extern Viewport*    Viewport_Create     (const strcrc* local_name, float width, float height, const vec2* scale, const vec2* offset);
extern void         Viewport_Destroy    (Viewport* viewport);

extern void         Viewport_Offset_Set (Viewport* viewport, vec2* offset);
extern void         Viewport_Scale_Set  (Viewport* viewport, vec2* scale);

extern void         Viewport_Clean      (Viewport* viewport);

extern void         Viewport_RenderTo_Screen    (RenderManager* render_manager, void* platform_data, const Viewport* back_viewport, const Viewport* front_viewport);
extern void         Viewport_RenderTo_Viewport  (const Viewport* viewport, Viewport* out_viewport);
extern void         Viewport_Render_ShaderText  (Viewport* viewport, ShaderText* shader_text);
