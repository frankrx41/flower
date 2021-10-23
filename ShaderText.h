#pragma once

typedef struct ShaderText ShaderText;
typedef struct vec3 vec3;
typedef struct vec2 vec2;

ShaderText* ShaderText_Create       (const strcrc* local_name, bool is_absolute, const vec3* location, const vec2* offset, const tchar* str);
void        ShaderText_Destory      (ShaderText* shader_text);
void        ShaderText_Disable      (ShaderText* shader_text, bool is_disable);
void        ShaderText_Move         (ShaderText* shader_text, int32 offset_x, int32 offset_y);
void        ShaderText_UpdateStr    (ShaderText* shader_text, const tchar* str);
void        ShaderText_UpdateInfo   (ShaderText* shader_text, uint32 info);

bool        ShaderText_Is_Absolute  (ShaderText* shader_text);
bool        ShaderText_IsDisable    (ShaderText* shader_text);
vec3*       ShaderText_Location_Get (ShaderText* shader_text);
tchar*      ShaderText_Str_Get      (ShaderText* shader_text);
void        ShaderText_Offset_Set   (ShaderText* shader_text, vec2* vec);
vec2*       ShaderText_Offset_Get   (ShaderText* shader_text);
