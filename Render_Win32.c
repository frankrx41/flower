#include "CoreMini.h"

#include "MemoryManager.h"
#include "RenderManager.h"

#define PLATFORM_WIN32      1
#include "Platform.h"
#include <stdio.h>
#include <windows.h>

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


RenderManagerPlatformData* RenderManager_PlatformData_Create_Plat(RenderManager* render_manager, const tchar* local_name)
{
    RenderManagerPlatformData* render_manager_platform_data = MemNew(local_name, RenderManagerPlatformData);
    render_manager_platform_data->m_width       = 80;
    render_manager_platform_data->m_height      = 25;
    render_manager_platform_data->m_buffer[0]   = MemNewSize(local_name, render_manager_platform_data->m_width*render_manager_platform_data->m_height*sizeof(PixData) );
    render_manager_platform_data->m_buffer[1]   = MemNewSize(local_name, render_manager_platform_data->m_width*render_manager_platform_data->m_height*sizeof(PixData) );

    MemZero(render_manager_platform_data->m_buffer[0]);
    MemZero(render_manager_platform_data->m_buffer[1]);

    render_manager_platform_data->m_front_buffer = render_manager_platform_data->m_buffer[0];
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

static void Render_PrintCharAtXY_Plat(RenderManagerPlatformData* render_manager_platform_data, uint32 x, uint32 y, tchar ch)
{
    COORD coord_screen = { x, y };
    SetConsoleCursorPosition(render_manager_platform_data->m_std_output, coord_screen);
    printf("%c", ch);
}

void RenderManager_RenderToScreen_Plat(RenderManager* render_manager, RenderManagerPlatformData* render_manager_platform_data)
{
    uint32 length = render_manager_platform_data->m_width * render_manager_platform_data->m_height;

    for( uint32 y=0; y<render_manager_platform_data->m_height; y++ )
    {
        for( uint32 x=0; x<render_manager_platform_data->m_width; x++ )
        {
            uint32 i = x + y*render_manager_platform_data->m_width;
            if( render_manager_platform_data->m_front_buffer[i].m_tchar != render_manager_platform_data->m_back_buffer[i].m_tchar )
            {
                Render_PrintCharAtXY_Plat(render_manager_platform_data, x, y, render_manager_platform_data->m_back_buffer[i].m_tchar);
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

void RenderManager_Render_InBackBuffer_Plat(RenderManager* render_manager, RenderManagerPlatformData* render_manager_platform_data, int32 x, int32 y, const tchar* str)
{
    uint32 index = render_manager_platform_data->m_width * y + x;
    for( uint32 i=0; str[i] != NULL && index+i < render_manager_platform_data->m_width*render_manager_platform_data->m_height; i++ )
    {
        render_manager_platform_data->m_back_buffer[index+i].m_tchar = str[i];
    }
}

