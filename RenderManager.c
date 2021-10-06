#include "CoreMini.h"


#include "Actor.h"
#include "Queue.h"
#include "Sence.h"
#include "RenderManager.h"


void RenderManager_Initialize_Plat();
void RenderManager_RenderToScreen_Plat();
void RenderManager_SwapBuffer_Plat();
void CallBack_Actor_RenderEachActor(Actor* actor, tptr ptr);

struct RenderManager
{
    bool            m_is_initialized;
    tptr            m_platform_data;
};

RenderManager* RenderManager_GetInstance()
{
    static RenderManager render_manager;
    return &render_manager;
}

void RenderManager_Initialize()
{
    RenderManager* render_manager = RenderManager_GetInstance();
    RenderManager_Initialize_Plat();
    render_manager->m_is_initialized = true;
}

void RenderManager_SetPlatformData(tptr ptr)
{
    RenderManager_GetInstance()->m_platform_data = ptr;
}

tptr RenderManager_GetPlatformData()
{
    return RenderManager_GetInstance()->m_platform_data;
}

void RenderManager_RenderSence(Sence* sence)
{
    Queue_ForEach(Sence_GetActorQueue(sence), CallBack_Actor_RenderEachActor, NULL);
}

void RenderManager_RenderToScreen()
{
    RenderManager_RenderToScreen_Plat();
    RenderManager_SwapBuffer_Plat();
}

