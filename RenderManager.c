#include "CoreMini.h"

#include "MemoryManager.h"
#include "RenderManager.h"
#include "SceneManager.h"

#include "Queue.h"
#include "Scene.h"


tptr    RenderManager_PlatformData_Create_Plat  (RenderManager* render_manager, const tchar* local_name);
void    RenderManager_PlatformData_Destroy_Plat (RenderManager* render_manager, tptr platform_data);
void    RenderManager_RenderToScreen_Plat       (RenderManager* render_manager, tptr platform_data);
void    RenderManager_SwapBuffer_Plat           (RenderManager* render_manager, tptr platform_data);
void    RenderManager_Render_InBackBuffer_Plat  (RenderManager* render_manager, tptr platform_data, int32 x, int32 y, const tchar* str);

void    CallBack_Actor_RenderEachActor          (Actor* actor, RenderManager* render_manager);

struct RenderManager
{
    bool    m_is_initialized;
    tptr    m_platform_data;
};



RenderManager* RenderManager_Create(const tchar* local_name)
{
    RenderManager* render_manager = MemNew(local_name, RenderManager);
    render_manager->m_platform_data = RenderManager_PlatformData_Create_Plat(render_manager, local_name);
    render_manager->m_is_initialized = true;
    return render_manager;
}

void RenderManager_Destroy(RenderManager* render_manager)
{
    RenderManager_PlatformData_Destroy_Plat(render_manager, render_manager->m_platform_data);
    render_manager->m_platform_data = NULL;
    MemDel(render_manager);
}

void RenderManager_Render_BufferToScreen(RenderManager* render_manager)
{
    RenderManager_RenderToScreen_Plat(render_manager, render_manager->m_platform_data);
    RenderManager_SwapBuffer_Plat(render_manager, render_manager->m_platform_data);
}

void RenderManager_Render_InBackBuffer(RenderManager* render_manager, int32 x, int32 y, const tchar* str)
{
    RenderManager_Render_InBackBuffer_Plat(render_manager, render_manager->m_platform_data, x, y, str);
}

static void CallBack_Render_Scene(Scene* scene, RenderManager* render_manager)
{
    if( Scene_IsPause(scene) )
    {
        return;
    }
    Queue_ForEach(Scene_GetActorQueue(scene), CallBack_Actor_RenderEachActor, render_manager);
}

void RenderManager_RenderAllScene(RenderManager* render_manager, SceneManager* scene_manager)
{
    Queue_ForEach(SceneManager_GetSceneQueue(scene_manager), CallBack_Render_Scene, render_manager);
}
