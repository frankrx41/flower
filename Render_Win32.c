#define PLATFORM_WIN32      1
#include "CoreMini.h"


#include "Memory.h"
#include "Render.h"
#include "String.h"

#include <stdio.h>
#include <windows.h>


typedef struct RenderManagerPlatformData RenderManagerPlatformData;
typedef struct PixData PixData;


#define LOCAL_NAME  "GPYM"


struct RenderData2D
{
    uint32  m_x;
    uint32  m_y;
    String* m_string;
    // TODO: Add color
    // int32   m_info;
};

struct PixData
{
    tchar   m_tchar;
    // TODO: Add color
    // int32   m_info;
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

static void Render_PrintCharAtXY_Plat(uint32 x, uint32 y, tchar ch)
{
    RenderManagerPlatformData* data = RenderManager_GetPlatformData();
    COORD coord_screen = { x, y };
    SetConsoleCursorPosition(data->m_std_output, coord_screen);
    {
        printf("%c", ch);
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
                Render_PrintCharAtXY_Plat(x, y, data->m_back_buffer[i].m_tchar);
            }
        }
    }
}

void RenderManager_SwapBuffer_Plat()
{
    RenderManagerPlatformData* data = RenderManager_GetPlatformData();
    PixData* back_buffer = data->m_back_buffer;
    data->m_back_buffer = data->m_front_buffer;
    data->m_front_buffer = back_buffer;
}

void RenderManager_RenderEachRenderData2D_Plat(RenderData2D* render_data_2d, tptr ptr)
{
    RenderManagerPlatformData* data = RenderManager_GetPlatformData();

    uint32 index = data->m_width * render_data_2d->m_y + render_data_2d->m_x;
    for( uint32 i=0; i<String_GetLength(render_data_2d->m_string); i++ )
    {
        data->m_back_buffer[index+i].m_tchar = String_CStr(render_data_2d->m_string)[i];
    }
}

RenderData2D* RenderData2D_Create(const tchar* local_name, int32 x, int32 y, const tchar* str)
{
    RenderData2D* render_data_2d = MemNew(local_name, RenderData2D);
    render_data_2d->m_x = x;
    render_data_2d->m_y = y;
    render_data_2d->m_string = String_New(local_name, str);

    return render_data_2d;
}

void RenderData2D_Destory(RenderData2D* render_data_2d)
{
    String_Del(render_data_2d->m_string);
    MemDel(render_data_2d);
}
