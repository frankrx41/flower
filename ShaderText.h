#pragma once

typedef struct ShaderText ShaderText;
typedef struct vec3 vec3;

ShaderText* ShaderText_Create       (const tchar* local_name, vec3 vec, const tchar* str);
void        ShaderText_Destory      (ShaderText* shader_text);
void        ShaderText_Disable      (ShaderText* shader_text, bool is_disable);
void        ShaderText_Move         (ShaderText* shader_text, int32 offset_x, int32 offset_y);
void        ShaderText_UpdateStr    (ShaderText* shader_text, const tchar* str);
void        ShaderText_UpdateInfo   (ShaderText* shader_text, uint32 info);

bool        ShaderText_IsDisable    (ShaderText* shader_text);
vec3        ShaderText_GetVec3      (ShaderText* shader_text);
tchar*      ShaderText_GetStr       (ShaderText* shader_text);