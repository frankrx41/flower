#include "CoreMini.h"


#include "Actor.h"
#include "Queue.h"
#include "Sence.h"
#include "RenderManager.h"

#include "MemoryManager.h"


void    RenderManager_Initialize_Plat       (RenderManager* render_manager);
void    RenderManager_RenderToScreen_Plat   (RenderManager* render_manager);
void    RenderManager_SwapBuffer_Plat       (RenderManager* render_manager);
void    CallBack_Actor_RenderEachActor      (Actor* actor, RenderManager* render_manager);

struct RenderManager
{
    bool            m_is_initialized;
    tptr            m_platform_data;
};



RenderManager* RenderManager_Create(const tchar* local_name)
{
    RenderManager* render_manager = MemNew(local_name, RenderManager);
    RenderManager_Initialize_Plat(render_manager);
    render_manager->m_is_initialized = true;
    return render_manager;
}

void RenderManager_SetPlatformData(RenderManager* render_manager, tptr ptr)
{
    render_manager->m_platform_data = ptr;
}

tptr RenderManager_GetPlatformData(RenderManager* render_manager)
{
    return render_manager->m_platform_data;
}

void RenderManager_RenderSence(RenderManager* render_manager, Sence* sence)
{
    Queue_ForEach(Sence_GetActorQueue(sence), CallBack_Actor_RenderEachActor, render_manager);
}

void RenderManager_RenderToScreen(RenderManager* render_manager)
{
    RenderManager_RenderToScreen_Plat(render_manager);
    RenderManager_SwapBuffer_Plat(render_manager);
}

