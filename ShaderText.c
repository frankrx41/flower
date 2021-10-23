#include "CoreMini.h"

#include "MemoryManager.h"
#include "ShaderText.h"

#include "String.h"
#include "Vec.h"


struct ShaderText
{
    bool    m_is_disable;
    bool    m_is_absolute;
    vec2    m_offset;
    vec3    m_location;
    uint32  m_info;
    String* m_string;
};


ShaderText* ShaderText_Create(const strcrc* local_name, bool is_absolute, const vec3* location, const vec2* offset, const tchar* str)
{
    ShaderText* shader_text = MemNew(local_name, ShaderText);
    shader_text->m_is_absolute  = is_absolute;
    shader_text->m_is_disable   = false;
    shader_text->m_location     = location ? *location : vec3_zero;
    shader_text->m_offset       = offset ? *offset : vec2_zero;
    shader_text->m_string       = String_New(local_name, str, 0, false);
    shader_text->m_info         = 0;

    return shader_text;
}

void ShaderText_Destory(ShaderText* shader_text)
{
    String_Del(shader_text->m_string);
    MemDel(shader_text);
}

void ShaderText_Disable(ShaderText* shader_text, bool is_disable)
{
    shader_text->m_is_disable = is_disable;
}

bool ShaderText_Is_Absolute(ShaderText* shader_text)
{
    return shader_text->m_is_absolute;
}

bool ShaderText_IsDisable(ShaderText* shader_text)
{
    return shader_text->m_is_disable;
}

vec3* ShaderText_Location_Get(ShaderText* shader_text)
{
    return &shader_text->m_location;
}

tchar* ShaderText_Str_Get(ShaderText* shader_text)
{
    return String_CStr(shader_text->m_string);
}

// void ShaderText_Offset_Set(ShaderText* shader_text, vec2 vec)
// {
//     shader_text->m_offset = vec;
// }

vec2* ShaderText_Offset_Get(ShaderText* shader_text)
{
    return &shader_text->m_offset;
}
