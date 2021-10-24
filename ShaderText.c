#include "CoreMini.h"

#include "MemoryManager.h"
#include "ShaderText.h"

#include "String.h"
#include "Vec.h"


struct ShaderText
{
    bool        m_is_disable;
    bool        m_is_absolute;
    vec3        m_position;
    const void* m_info;
    String*     m_string;
};


ShaderText* ShaderText_Create(const strcrc* local_name, bool is_absolute, const vec3* position, const void* info, const tchar* str)
{
    ShaderText* shader_text = MemNew(local_name, ShaderText);
    shader_text->m_is_absolute  = is_absolute;
    shader_text->m_is_disable   = false;
    shader_text->m_position     = position ? *position : vec3_zero;
    shader_text->m_string       = String_New(local_name, str, 0, false);
    shader_text->m_info         = info;

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

bool ShaderText_Is_Absolute(const ShaderText* shader_text)
{
    return shader_text->m_is_absolute;
}

bool ShaderText_Is_Disable(const ShaderText* shader_text)
{
    return shader_text->m_is_disable;
}

const vec3* ShaderText_Position_Get(const ShaderText* shader_text)
{
    return &shader_text->m_position;
}

const tchar* ShaderText_Str_Get(const ShaderText* shader_text)
{
    return String_CStr(shader_text->m_string);
}

const void* ShaderText_Info_Get(const ShaderText* shader_text)
{
    return shader_text->m_info;
}