#include "CoreMini.h"


#include "Actor.h"
#include "Queue.h"
#include "Sence.h"
#include "Render.h"


typedef struct RenderContent RenderContent;
typedef struct RenderData RenderData;

void RenderManager_Initialize_Plat();
void RenderManager_RenderEachRenderData_Plat(RenderData* render_data, tptr);
void RenderManager_RenderToScreen_Plat();

struct RenderContent
{
    Queue(RenderData*)* m_render_data_queue;
};


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

static void RenderManager_RenderEachActor(Actor* actor, tptr ptr)
{
    RenderContent* render_content = Actor_Cast(actor, RenderContent);
    if( render_content )
    {
        Queue_ForEach(render_content->m_render_data_queue, RenderManager_RenderEachRenderData_Plat, NULL);
    }
}

void RenderManager_RenderSenceActor(Sence* sence)
{
    Queue_ForEach(Sence_GetActorQueue(sence), RenderManager_RenderEachActor, NULL);
}

void RenderManager_RenderToScreen()
{
    RenderManager_RenderToScreen_Plat();
}

