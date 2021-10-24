#pragma once

typedef struct ShaderText ShaderText;
typedef struct vec3 vec3;


extern ShaderText*  ShaderText_Create       (const strcrc* local_name, bool is_absolute, const vec3* position, const void* info, const tchar* str);
extern void         ShaderText_Destory      (ShaderText* shader_text);
extern void         ShaderText_Disable      (ShaderText* shader_text, bool is_disable);
extern void         ShaderText_Move         (ShaderText* shader_text, int32 offset_x, int32 offset_y);
extern void         ShaderText_UpdateStr    (ShaderText* shader_text, const tchar* str);
extern void         ShaderText_UpdateInfo   (ShaderText* shader_text, uint32 info);

extern bool         ShaderText_Is_Absolute  (const ShaderText* shader_text);
extern bool         ShaderText_Is_Disable   (const ShaderText* shader_text);
extern const vec3*  ShaderText_Position_Get (const ShaderText* shader_text);
extern const tchar* ShaderText_Str_Get      (const ShaderText* shader_text);
extern const void*  ShaderText_Info_Get     (const ShaderText* shader_text);
