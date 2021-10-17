#include "CoreMini.h"

#include "MemoryManager.h"
#include "RenderManager.h"
#include "SceneManager.h"

#include "Queue.h"
#include "Scene.h"
#include "Vec.h"


tptr    RenderManager_PlatformData_Create_Plat  (RenderManager* render_manager, const tchar* local_name);
void    RenderManager_PlatformData_Destroy_Plat (RenderManager* render_manager, tptr platform_data);
void    RenderManager_ToScreen_Plat             (RenderManager* render_manager, tptr platform_data);
void    RenderManager_SwapBuffer_Plat           (RenderManager* render_manager, tptr platform_data);
void    RenderManager_Render_ToBackBuffer_Plat  (RenderManager* render_manager, tptr platform_data, vec2 offset_vec, ShaderText* shader_text);

void    CallBack_Actor_RenderEachActor          (Actor* actor, RenderManager* render_manager);

struct RenderManager
{
    bool        m_is_initialized;
    RenderMode  m_render_mode;
    tptr        m_platform_data;
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

void RenderManager_Render_ToScreen(RenderManager* render_manager)
{
    RenderManager_ToScreen_Plat(render_manager, render_manager->m_platform_data);
    RenderManager_SwapBuffer_Plat(render_manager, render_manager->m_platform_data);
}

void RenderManager_Render_ToBackBuffer(RenderManager* render_manager, vec2 offset_vec, ShaderText* shader_text)
{
    RenderManager_Render_ToBackBuffer_Plat(render_manager, render_manager->m_platform_data, offset_vec, shader_text);
}

static void CallBack_Render_Scene(Scene* scene, RenderManager* render_manager)
{
    if( Scene_Is_Pause_Get(scene) )
    {
        return;
    }
    Queue_ForEach(Scene_ActorQueue_Renderable_Get(scene), CallBack_Actor_RenderEachActor, render_manager);
}

void RenderManager_RenderAllScene(RenderManager* render_manager, SceneManager* scene_manager)
{
    Queue_ForEach(SceneManager_SceneQueue_Get(scene_manager), CallBack_Render_Scene, render_manager);
}

