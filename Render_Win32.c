#include "CoreMini.h"

#if PLATFORM_WIN32

#include "MemoryManager.h"
#include "RenderManager.h"

#include "ShaderText.h"

#include "Vec.h"
#include "tData.h"

#include "Platform.h"


typedef struct RenderManagerPlatformData RenderManagerPlatformData;
typedef struct PixData PixData;

struct PixData
{
    tchar   m_tchar;
    uint32  m_info;
};

struct RenderManagerPlatformData
{
    uint32      m_width;
    uint32      m_height;

    PixData*    m_buffer[2];
    PixData*    m_back_buffer;
    PixData*    m_front_buffer;

    HANDLE      m_std_output;
};


RenderManagerPlatformData* RenderManager_PlatformData_Create_Plat(RenderManager* render_manager, const strcrc* local_name)
{
    RenderManagerPlatformData* render_manager_platform_data = MemNew(local_name, RenderManagerPlatformData);
    render_manager_platform_data->m_width       = 80;
    render_manager_platform_data->m_height      = 25;
    render_manager_platform_data->m_buffer[0]   = MemNewSize(local_name, render_manager_platform_data->m_width*render_manager_platform_data->m_height*sizeof(PixData) );
    render_manager_platform_data->m_buffer[1]   = MemNewSize(local_name, render_manager_platform_data->m_width*render_manager_platform_data->m_height*sizeof(PixData) );

    MemZero(render_manager_platform_data->m_buffer[0]);
    MemZero(render_manager_platform_data->m_buffer[1]);

    render_manager_platform_data->m_front_buffer= render_manager_platform_data->m_buffer[0];
    render_manager_platform_data->m_back_buffer = render_manager_platform_data->m_buffer[1];

    render_manager_platform_data->m_std_output  = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cc_info;
    cc_info.bVisible    = 0;
    cc_info.dwSize      = 100;
    SetConsoleCursorInfo(render_manager_platform_data->m_std_output, &cc_info);

    return render_manager_platform_data;
}

void RenderManager_PlatformData_Destroy_Plat(RenderManager* render_manager, RenderManagerPlatformData* render_manager_platform_data)
{
    MemDel(render_manager_platform_data->m_buffer[0]);
    MemDel(render_manager_platform_data->m_buffer[1]);
    MemDel(render_manager_platform_data);
}

static void Render_PrintCharAtXY_Win32(RenderManagerPlatformData* render_manager_platform_data, uint32 x, uint32 y, tchar ch)
{
    const COORD coord_screen = { x, y };
    SetConsoleCursorPosition(render_manager_platform_data->m_std_output, coord_screen);
    printf("%c", ch == 0 ? ' ' : ch);
}

void RenderManager_ToScreen_Plat(RenderManager* render_manager, RenderManagerPlatformData* render_manager_platform_data)
{
    uint32 length = render_manager_platform_data->m_width * render_manager_platform_data->m_height;

    for( uint32 y=0; y<render_manager_platform_data->m_height; y++ )
    {
        for( uint32 x=0; x<render_manager_platform_data->m_width; x++ )
        {
            uint32 i = x + y*render_manager_platform_data->m_width;
            if( render_manager_platform_data->m_front_buffer[i].m_tchar != render_manager_platform_data->m_back_buffer[i].m_tchar )
            {
                Render_PrintCharAtXY_Win32(render_manager_platform_data, x, y, render_manager_platform_data->m_back_buffer[i].m_tchar);
            }
        }
    }
}

void RenderManager_SwapBuffer_Plat(RenderManager* render_manager, RenderManagerPlatformData* render_manager_platform_data)
{
    PixData* back_buffer = render_manager_platform_data->m_back_buffer;
    render_manager_platform_data->m_back_buffer = render_manager_platform_data->m_front_buffer;
    render_manager_platform_data->m_front_buffer = back_buffer;

    if( render_manager_platform_data->m_back_buffer == render_manager_platform_data->m_buffer[0] )
    {
        MemZero(render_manager_platform_data->m_buffer[0]);
    }
    else
    {
        MemZero(render_manager_platform_data->m_buffer[1]);
    }
}

void RenderManager_Render_ToBackBuffer_Plat(RenderManager* render_manager, RenderManagerPlatformData* render_manager_platform_data, vec2 offset_vec, ShaderText* shader_text)
{
    const vec3 location = ShaderText_GetVec3(shader_text);
    const float x = location.m_x + offset_vec.m_x;
    const float y = location.m_y + offset_vec.m_y;
    const tchar* str = ShaderText_GetStr(shader_text);

    const uint32 index = render_manager_platform_data->m_width * uInt32(y) + uInt32(x);

    for( uint32 i=0; str[i] != NULL; i++ )
    {
        PixData* pix_data = &render_manager_platform_data->m_back_buffer[index+i];
        if(Memory_IsInBounds(render_manager_platform_data->m_back_buffer, pix_data))
        {
            pix_data->m_tchar = str[i];
        }
        else
        {
            Assert(false, "Render out of range!");
        }
    }
}

#endif
