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
    HANDLE      m_std_output;
};


RenderManagerPlatformData* RenderManager_PlatformData_Create(RenderManager* render_manager, const strcrc* local_name)
{
    RenderManagerPlatformData* render_manager_platform_data = MemNew(local_name, RenderManagerPlatformData);
    render_manager_platform_data->m_std_output  = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cc_info;
    cc_info.bVisible    = 0;
    cc_info.dwSize      = 100;
    SetConsoleCursorInfo(render_manager_platform_data->m_std_output, &cc_info);

    return render_manager_platform_data;
}

void RenderManager_PlatformData_Destroy(RenderManager* render_manager, RenderManagerPlatformData* render_manager_platform_data)
{
    MemDel(render_manager_platform_data);
}

void Render_PrintCharAtXY_Platform(RenderManager* render_manager, RenderManagerPlatformData* render_manager_platform_data, uint32 x, uint32 y, uint32 info, tchar ch)
{
    const COORD coord_screen = { x, y };
    SetConsoleCursorPosition(render_manager_platform_data->m_std_output, coord_screen);
    if( info == 0 )
    {
        info = 0x07;
    }
    SetConsoleTextAttribute(render_manager_platform_data->m_std_output, info);
    printf("%c", ch == 0 ? ' ' : ch);
}


#endif
