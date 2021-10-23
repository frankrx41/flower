#include "CoreMini.h"

#if PLATFORM_WIN32

#include "MemoryManager.h"
#include "RenderManager.h"

#include "ShaderText.h"

#include "Vec.h"
#include "tData.h"

#include "Platform.h"
#include "Viewport.h"


typedef struct RenderManagerPlatformData RenderManagerPlatformData;

struct RenderManagerPlatformData
{
    uint32      m_width;
    uint32      m_height;

    Viewport*   m_buffer[2];
    Viewport*   m_back_buffer;
    Viewport*   m_front_buffer;

    HANDLE      m_std_output;
};


RenderManagerPlatformData* RenderManager_PlatformData_Create_Plat(RenderManager* render_manager, const strcrc* local_name)
{
    RenderManagerPlatformData* render_manager_platform_data = MemNew(local_name, RenderManagerPlatformData);
    render_manager_platform_data->m_width       = 80;
    render_manager_platform_data->m_height      = 25;
    render_manager_platform_data->m_buffer[0]   = Viewport_Create(local_name, 80, 25, &vec2_null, &vec2_null);
    render_manager_platform_data->m_buffer[1]   = Viewport_Create(local_name, 80, 25, &vec2_null, &vec2_null);

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
    Viewport_Destroy(render_manager_platform_data->m_buffer[0]);
    Viewport_Destroy(render_manager_platform_data->m_buffer[1]);

    MemDel(render_manager_platform_data);
}

void Render_PrintCharAtXY_Platform(RenderManager* render_manager, RenderManagerPlatformData* render_manager_platform_data, uint32 x, uint32 y, tchar ch)
{
    const COORD coord_screen = { x, y };
    SetConsoleCursorPosition(render_manager_platform_data->m_std_output, coord_screen);
    printf("%c", ch == 0 ? ' ' : ch);
}

void RenderManager_ToScreen_Plat(RenderManager* render_manager, RenderManagerPlatformData* render_manager_platform_data)
{
    Viewport_RenderTo_Screen(render_manager, render_manager_platform_data, render_manager_platform_data->m_back_buffer, render_manager_platform_data->m_front_buffer);
}

void RenderManager_SwapBuffer_Plat(RenderManager* render_manager, RenderManagerPlatformData* render_manager_platform_data)
{
    Viewport* back_buffer = render_manager_platform_data->m_back_buffer;
    render_manager_platform_data->m_back_buffer = render_manager_platform_data->m_front_buffer;
    render_manager_platform_data->m_front_buffer = back_buffer;

    if( render_manager_platform_data->m_back_buffer == render_manager_platform_data->m_buffer[0] )
    {
        Viewport_Clean(render_manager_platform_data->m_buffer[0]);
    }
    else
    {
        Viewport_Clean(render_manager_platform_data->m_buffer[1]);
    }
}

void RenderManager_Render_ToBackBuffer_Plat(RenderManager* render_manager, RenderManagerPlatformData* render_manager_platform_data, vec2 offset_vec, ShaderText* shader_text)
{
    Viewport_Render_ShaderText(render_manager_platform_data->m_back_buffer, shader_text);
}

#endif
