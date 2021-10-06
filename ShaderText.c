#include "CoreMini.h"

#include "Actor.h"
#include "MemoryManager.h"
#include "String.h"
#include "Vec.h"


struct ShaderText
{
    bool    m_disable;
    vec3    m_vec3;
    uint32  m_info;
    String* m_string;
};


ShaderText* ShaderText_Create(const tchar* local_name, vec3 vec, const tchar* str)
{
    ShaderText* render_data_text = MemNew(local_name, ShaderText);
    render_data_text->m_vec3    = vec;
    render_data_text->m_string  = String_New(local_name, str);
    render_data_text->m_disable = false;
    render_data_text->m_info    = 0;

    return render_data_text;
}

void ShaderText_Destory(ShaderText* render_data_text)
{
    String_Del(render_data_text->m_string);
    MemDel(render_data_text);
}
