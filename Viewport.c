#include "CoreMini.h"

#include "MemoryManager.h"
#include "RenderManager.h"

#include "Viewport.h"
#include "ShaderText.h"
#include "tData.h"
#include "Vec.h"

typedef struct PixData PixData;

struct PixData
{
    tchar   m_tchar;
    uint32  m_info;
};

struct Viewport
{
    vec2        m_scale;
    vec2        m_offset;
    float       m_width;
    float       m_height;
    PixData*    m_data;
};


Viewport* Viewport_Create(const strcrc* local_name, float width, float height, const vec2* scale, const vec2* offset)
{
    Viewport* viewport  = MemNew(local_name, Viewport);
    viewport->m_scale   = scale ? *scale : vec2_null;
    viewport->m_offset  = offset ? *offset : vec2_null;
    viewport->m_width   = width;
    viewport->m_height  = height;
    viewport->m_data    = MemNewSize(local_name, tSize(width*height*sizeof(PixData)));
    Viewport_Clean(viewport);

    return viewport;
}

void Viewport_Destroy(Viewport* viewport)
{
    MemDel(viewport->m_data);
    MemDel(viewport);
}

void Viewport_Offset_Set(Viewport* viewport, vec2* offset)
{
    viewport->m_offset = *offset;
}

void Viewport_Scale_Set(Viewport* viewport, vec2* scale)
{
    viewport->m_scale = *scale;
}

void Viewport_Clean(Viewport* viewport)
{
    MemZero(viewport->m_data);
}

void Viewport_RenderTo_Screen(RenderManager* render_manager, void* platform_data, const Viewport* back_viewport, const Viewport* front_viewport)
{
    for( uint32 y=0; y<front_viewport->m_height; y++ )
    {
        for( uint32 x=0; x<front_viewport->m_width; x++ )
        {
            uint32 i = x + y*uInt32(front_viewport->m_width);
            if( front_viewport->m_data[i].m_tchar != back_viewport->m_data[i].m_tchar )
            {
                extern void Render_PrintCharAtXY_Platform(RenderManager*, void*, uint32, uint32, tchar);
                Render_PrintCharAtXY_Platform(render_manager, platform_data, x, y, back_viewport->m_data[i].m_tchar);
            }
        }
    }
}

void Viewport_RenderTo_Viewport(const Viewport* viewport, Viewport* out_viewport)
{
    Assert(out_viewport != NULL, "");
    for( int32 y=out_viewport->m_offset.m_y; y<viewport->m_height; y++ )
    {
        for( int32 x=out_viewport->m_offset.m_x; x<viewport->m_width; x++ )
        {
            int32 i1 = x + y*Int32(out_viewport->m_width);
            // int32 i2 = (out_viewport->m_offset.m_y-y) * (Int32(viewport->m_width)+out_viewport->m_offset.m_x) + x;
            int32 i2 = i1;
            if( out_viewport->m_data[i2].m_tchar != 0 )
            {
                if( viewport->m_data[i1].m_tchar != out_viewport->m_data[i2].m_tchar )
                {
                    viewport->m_data[i1] = out_viewport->m_data[i2];
                }
            }
        }
    }
}

void Viewport_Render_ShaderText(Viewport* viewport, ShaderText* shader_text)
{
    const vec3* location = ShaderText_Location_Get(shader_text);
    const float x = location->m_x + viewport->m_offset.m_x;
    const float y = location->m_y + viewport->m_offset.m_y;
    const tchar* str = ShaderText_Str_Get(shader_text);
    
    const uint32 index = uInt32(viewport->m_width) * uInt32(y) + uInt32(x);
    
    for( uint32 i=0; str[i] != NULL; i++ )
    {
        PixData* pix_data = &viewport->m_data[index+i];
        if(Memory_IsInBounds(viewport->m_data, pix_data))
        {
            pix_data->m_tchar = str[i];
        }
        else
        {
            Assert(false, "Render out of range!");
        }
    }
}

