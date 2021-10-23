#include "CoreMini.h"

#include "MemoryManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "TaskManager.h"

#include "Viewport.h"
#include "Queue.h"
#include "Scene.h"
#include "String.h"
#include "Vec.h"


void*   RenderManager_PlatformData_Create_Plat  (RenderManager* render_manager, const strcrc* local_name);
void    RenderManager_PlatformData_Destroy_Plat (RenderManager* render_manager, void* platform_data);

void    CallBack_Actor_RenderTo_Scene_Viewport  (Actor* actor, const Scene* scene);

struct RenderManager
{
    bool        m_is_initialized;

    Viewport*   m_buffer[2];
    Viewport*   m_back_buffer;
    Viewport*   m_front_buffer;

    void*       m_platform_data;
};

RenderManager* RenderManager_Create(const strcrc* local_name)
{
    RenderManager* render_manager   = MemNew(local_name, RenderManager);
    render_manager->m_buffer[0]     = Viewport_Create(local_name, 80, 25, &vec2_null, &vec2_null);
    render_manager->m_buffer[1]     = Viewport_Create(local_name, 80, 25, &vec2_null, &vec2_null);

    render_manager->m_front_buffer  = render_manager->m_buffer[0];
    render_manager->m_back_buffer   = render_manager->m_buffer[1];

    render_manager->m_platform_data = RenderManager_PlatformData_Create_Plat(render_manager, local_name);

    render_manager->m_is_initialized = true;

    return render_manager;
}

void RenderManager_Destroy(RenderManager* render_manager)
{
    RenderManager_PlatformData_Destroy_Plat(render_manager, render_manager->m_platform_data);
    Viewport_Destroy(render_manager->m_buffer[0]);
    Viewport_Destroy(render_manager->m_buffer[1]);
    MemDel(render_manager);
}

static void RenderManager_SwapBuffer(RenderManager* render_manager)
{
    Viewport* back_buffer = render_manager->m_back_buffer;
    render_manager->m_back_buffer = render_manager->m_front_buffer;
    render_manager->m_front_buffer = back_buffer;

    if( render_manager->m_back_buffer == render_manager->m_buffer[0] )
    {
        Viewport_Clean(render_manager->m_buffer[0]);
    }
    else
    {
        Viewport_Clean(render_manager->m_buffer[1]);
    }
}

void RenderManager_Render_ToScreen(RenderManager* render_manager)
{
    Viewport_RenderTo_Screen(render_manager, render_manager->m_platform_data, render_manager->m_back_buffer, render_manager->m_front_buffer);
    RenderManager_SwapBuffer(render_manager);
}

void RenderManager_Render_ToBackBuffer(RenderManager* render_manager, ShaderText* shader_text)
{
    Viewport_Render_ShaderText(render_manager->m_back_buffer, shader_text);
}

static void CallBack_Render_Scene(Scene* scene, const RenderManager* render_manager)
{
    Viewport* viewport = Scene_Viewport_Get(scene);
    if( Scene_Is_Hide(scene) || viewport == NULL)
    {
        return;
    }
    Queue_ForEach(Scene_ActorQueue_Renderable_Get(scene), CallBack_Actor_RenderTo_Scene_Viewport, scene);

    Viewport_RenderTo_Viewport(render_manager->m_back_buffer, viewport);
    Viewport_Clean(viewport);
}

static void CallBack_RenderManager_Render_ToScreen_Task(Task* task, RenderManager* render_manager)
{
    RenderManager_Render_ToScreen(render_manager);
}

void RenderManager_RenderAllScene(RenderManager* render_manager, SceneManager* scene_manager)
{
    Queue_ForEach(SceneManager_SceneQueue_Foreground_Get(scene_manager), CallBack_Render_Scene, render_manager);
    
    strcrc local_name = StrCrc("RenderManager_Render_All_Task", 0);
    
    TaskManager_Task_Render_Add(&local_name, CallBack_RenderManager_Render_ToScreen_Task, NULL, RenderManager_GetInstance());
}

