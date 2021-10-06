#include "CoreMini.h"

#include "MemoryManager.h"
#include "RenderManager.h"
#include "String.h"
#include "Vec.h"

#define PLATFORM_WIN32      1
#include "Platform.h"
#include <stdio.h>
#include <windows.h>

#include "RenderComponent.h"
#include "ShaderText.h"


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

    data->m_std_output  = GetStdHandle(STD_OUTPUT_HANDLE);

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

static void Render_InBackBuff_Plat(int32 x, int32 y, const tchar* str)
{
    RenderManagerPlatformData* data = RenderManager_GetPlatformData();

    uint32 index = data->m_width * y + x;
    for( uint32 i=0; str[i] != NULL; i++ )
    {
        data->m_back_buffer[index+i].m_tchar = str[i];
    }
}

void CallBack_Render_ShaderText_Plat(ShaderText* shader_text, vec3* actor_vec)
{
    if( ShaderText_IsDisable(shader_text) )
    {
        return;
    }

    vec3 vec = Vec3_Add(ShaderText_GetVec3(shader_text), *actor_vec);

    Render_InBackBuff_Plat((int32)vec.m_x, (int32)vec.m_y, ShaderText_GetStr(shader_text));
}
