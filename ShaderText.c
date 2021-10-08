#include "CoreMini.h"

#include "MemoryManager.h"
#include "ShaderText.h"

#include "String.h"
#include "Vec.h"


struct ShaderText
{
    bool    m_is_disable;
    vec3    m_vec3;
    uint32  m_info;
    String* m_string;
};


ShaderText* ShaderText_Create(const tchar* local_name, vec3 vec, const tchar* str)
{
    ShaderText* shader_text = MemNew(local_name, ShaderText);
    shader_text->m_vec3    = vec;
    shader_text->m_string  = String_New(local_name, str);
    shader_text->m_is_disable = false;
    shader_text->m_info    = 0;

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

bool ShaderText_IsDisable(ShaderText* shader_text)
{
    return shader_text->m_is_disable;
}

vec3 ShaderText_GetVec3(ShaderText* shader_text)
{
    return shader_text->m_vec3;
}

tchar* ShaderText_GetStr(ShaderText* shader_text)
{
    return String_CStr(shader_text->m_string);
}
