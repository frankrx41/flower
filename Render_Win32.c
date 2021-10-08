#include "CoreMini.h"

#include "MemoryManager.h"
#include "RenderManager.h"

#define PLATFORM_WIN32      1
#include "Platform.h"
#include <stdio.h>
#include <windows.h>

typedef struct RenderManagerPlatformData RenderManagerPlatformData;
typedef struct PixData PixData;


#define LOCAL_NAME  "GPYM"


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

void RenderManager_Initialize_Plat(RenderManager* render_manager)
{
    RenderManagerPlatformData* data = MemNew(LOCAL_NAME, RenderManagerPlatformData);
    RenderManager_SetPlatformData(render_manager, data);
    data->m_width       = 80;
    data->m_height      = 25;
    data->m_buffer[0]   = MemNewSize(LOCAL_NAME, data->m_width*data->m_height*sizeof(PixData) );
    data->m_buffer[1]   = MemNewSize(LOCAL_NAME, data->m_width*data->m_height*sizeof(PixData) );

    MemZero(data->m_buffer[0]);
    MemZero(data->m_buffer[1]);

    data->m_front_buffer = data->m_buffer[0];
    data->m_back_buffer = data->m_buffer[1];

    data->m_std_output  = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cc_info;
    cc_info.bVisible    = 0;
    cc_info.dwSize      = 100;
    SetConsoleCursorInfo(data->m_std_output, &cc_info);
}

static void Render_PrintCharAtXY_Plat(RenderManager* render_manager, uint32 x, uint32 y, tchar ch)
{
    RenderManagerPlatformData* data = RenderManager_GetPlatformData(render_manager);
    COORD coord_screen = { x, y };
    SetConsoleCursorPosition(data->m_std_output, coord_screen);
    printf("%c", ch);
}

void RenderManager_RenderToScreen_Plat(RenderManager* render_manager)
{
    RenderManagerPlatformData* data = RenderManager_GetPlatformData(render_manager);

    uint32 length = data->m_width * data->m_height;

    for( uint32 y=0; y<data->m_height; y++ )
    {
        for( uint32 x=0; x<data->m_width; x++ )
        {
            uint32 i = x + y*data->m_width;
            if( data->m_front_buffer[i].m_tchar != data->m_back_buffer[i].m_tchar )
            {
                Render_PrintCharAtXY_Plat(render_manager, x, y, data->m_back_buffer[i].m_tchar);
                // Log(0, "%c", data->m_back_buffer[i].m_tchar);
            }
        }
    }
}

void RenderManager_SwapBuffer_Plat(RenderManager* render_manager)
{
    RenderManagerPlatformData* data = RenderManager_GetPlatformData(render_manager);
    PixData* back_buffer = data->m_back_buffer;
    data->m_back_buffer = data->m_front_buffer;
    data->m_front_buffer = back_buffer;

    if( data->m_back_buffer == data->m_buffer[0] )
    {
        MemZero(data->m_buffer[0]);
    }
    else
    {
        MemZero(data->m_buffer[1]);
    }
}

void Render_InBackBuffer_Plat(RenderManager* render_manager, int32 x, int32 y, const tchar* str)
{
    RenderManagerPlatformData* data = RenderManager_GetPlatformData(render_manager);

    uint32 index = data->m_width * y + x;
    for( uint32 i=0; str[i] != NULL && index+i < data->m_width*data->m_height; i++ )
    {
        data->m_back_buffer[index+i].m_tchar = str[i];
    }
}

