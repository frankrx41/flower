#pragma once

typedef struct ShaderText ShaderText;
typedef struct vec3 vec3;

ShaderText* ShaderText_Create       (const tchar* local_name, vec3 vec, const tchar* str);
void        ShaderText_Destory      (ShaderText* render_data_text);
void        ShaderText_Disable      (ShaderText* render_data_text);
void        ShaderText_Move         (ShaderText* render_data_text, int32 offset_x, int32 offset_y);
void        ShaderText_UpdateStr    (ShaderText* render_data_text, const tchar* str);
void        ShaderText_UpdateInfo   (ShaderText* render_data_text, uint32 info);
