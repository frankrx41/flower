#define PLATFORM_WIN32      1
#include "CoreMini.h"


#include "Memory.h"
#include "Render.h"

#include <stdio.h>
#include <windows.h>

typedef struct RenderData RenderData;
typedef struct RenderManagerPlatformData RenderManagerPlatformData;
typedef struct PixData PixData;

#define LOCAL_NAME  "Render"

struct RenderData
{
    uint32  m_x;
    uint32  m_y;
    uint32  m_length;
    tchar   m_char[8];
};

struct PixData
{
    tchar   m_tchar;
    WORD    m_info;
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

void RenderManager_Initialize_Plat()
{
    RenderManagerPlatformData* data = MemNew(LOCAL_NAME, RenderManagerPlatformData);
    RenderManager_SetPlatformData(data);
    data->m_width       = 80;
    data->m_height      = 25;
    data->m_buffer[0]   = MemNewSize(LOCAL_NAME, data->m_width*data->m_height*sizeof(PixData) );
    data->m_buffer[1]   = MemNewSize(LOCAL_NAME, data->m_width*data->m_height*sizeof(PixData) );

    data->m_front_buffer = data->m_buffer[0];
    data->m_back_buffer = data->m_buffer[1];

    CONSOLE_CURSOR_INFO cc_info;
    cc_info.bVisible    = 0;
    cc_info.dwSize      = 100;
    SetConsoleCursorInfo(data->m_std_output, &cc_info);
}

static Render_PrintAtXY(uint32 x, uint32 y, const tchar* str)
{
    RenderManagerPlatformData* data = RenderManager_GetPlatformData();
    COORD coord_screen = { x, y };
    SetConsoleCursorPosition(data->m_std_output, coord_screen);
    for( uint32 i=0; str[i] != NULL; i++ )
    {
        printf("%c", str[i]);
    }
}

void RenderManager_RenderToScreen_Plat()
{
    RenderManagerPlatformData* data = RenderManager_GetPlatformData();

    uint32 length = data->m_width * data->m_height;

    for( uint32 y=0; y<data->m_height; y++ )
    {
        for( uint32 x=0; x<data->m_width; x++ )
        {
            uint32 i = x + y*data->m_width;
            if( data->m_front_buffer[i].m_tchar != data->m_back_buffer[i].m_tchar )
            {
                Render_PrintAtXY(x, y, data->m_back_buffer[i].m_tchar);
            }
        }
    }

    PixData* back_buffer = data->m_back_buffer;
    data->m_back_buffer = data->m_front_buffer;
    data->m_front_buffer = back_buffer;
}


void RenderManager_RenderEachRenderData_Plat(RenderData* render_data, tptr ptr)
{
    RenderManagerPlatformData* data = RenderManager_GetPlatformData();

    uint32 index = data->m_width * render_data->m_y + render_data->m_x;
    for( int32 i=0; i<render_data->m_length; i++ )
    {
        data->m_back_buffer[index+i].m_tchar = render_data->m_char[i];
    }
}

