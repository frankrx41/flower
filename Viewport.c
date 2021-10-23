#include "CoreMini.h"

#include "Viewport.h"

#include "MemoryManager.h"
#include "Vec.h"

typedef struct PixData PixData;

struct PixData
{
    tchar   m_tchar;
    uint32  m_info;
};

struct Viewport
{
    vec2    m_scale;
    vec2    m_offset;
    float   m_width;
    float   m_height;
    void*   m_data;
};


Viewport* Viewport_Create(const strcrc* local_name, float width, float height, const vec2* scale, const vec2* offset)
{
    Viewport* viewport  = MemNew(local_name, Viewport);
    viewport->m_scale   = scale ? *scale : vec2_null;
    viewport->m_offset  = offset ? *offset : vec2_null;
    viewport->m_width   = width;
    viewport->m_height  = height;
    viewport->m_data    = MemNewSize(local_name, width*height*sizeof(PixData));
    Viewport_Clean(viewport);

    return viewport;
}

void Viewport_Destroy(Viewport* viewport)
{
    MemDel(viewport->m_data);
    MemDel(viewport);
}

void Viewport_Clean(Viewport* viewport)
{
    MemZero(viewport->m_data);
}

void Viewport_RenderTo_Screen(const Viewport* back_viewport, const Viewport* front_viewport)
{
    // for( uint32 y=0; y<render_manager_platform_data->m_height; y++ )
    // {
    //     for( uint32 x=0; x<render_manager_platform_data->m_width; x++ )
    //     {
    //         uint32 i = x + y*render_manager_platform_data->m_width;
    //         if( render_manager_platform_data->m_front_buffer[i].m_tchar != render_manager_platform_data->m_back_buffer[i].m_tchar )
    //         {
    //             Render_PrintCharAtXY_Win32(render_manager_platform_data, x, y, render_manager_platform_data->m_back_buffer[i].m_tchar);
    //         }
    //     }
    // }

    // uint32 length = render_manager_platform_data->m_width * render_manager_platform_data->m_height;

}

void Viewport_RenderTo_Viewport(const Viewport* viewport, Viewport* out_viewport)
{

}


void Viewport_Render_ShaderText(Viewport* viewport, ShaderText* shader_text)
{
    // const vec3 location = ShaderText_GetVec3(shader_text);
    // const float x = location.m_x + offset_vec.m_x;
    // const float y = location.m_y + offset_vec.m_y;
    // const tchar* str = ShaderText_GetStr(shader_text);
    //
    // const uint32 index = render_manager_platform_data->m_width * uInt32(y) + uInt32(x);
    //
    // for( uint32 i=0; str[i] != NULL; i++ )
    // {
    //     PixData* pix_data = &render_manager_platform_data->m_back_buffer[index+i];
    //     if(Memory_IsInBounds(render_manager_platform_data->m_back_buffer, pix_data))
    //     {
    //         pix_data->m_tchar = str[i];
    //     }
    //     else
    //     {
    //         Assert(false, "Render out of range!");
    //     }
    // }
}

