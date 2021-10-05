#include "CoreMini.h"


#include "Actor.h"
#include "Component.h"
#include "Memory.h"
#include "Queue.h"
#include "Render.h"
#include "String.h"

#define LOCAL_NAME  "RenderComponent"

struct RenderData
{
    uint32  m_x;
    uint32  m_y;
    String* m_string;
    // TODO: Add color
    // int32   m_info;
};


struct RenderComponent
{
    Queue(RenderData*)*     m_render_data_queue;
};

void Component_Render_ClearRenderData_Plat(RenderComponent* render_component);
void RenderManager_RenderEachRenderData_Plat(RenderData* render_data, tptr);

RenderComponent* Component_Render_Create(const tchar* local_name)
{
    RenderComponent* render_component = MemNew(local_name, RenderComponent);
    render_component->m_render_data_queue = Queue_Create(local_name, RenderData*);
    return render_component;
}

void Component_Render_Destroy(RenderComponent* render_component)
{
    Component_Render_ClearRenderData_Plat(render_component);
    Queue_Destroy(render_component->m_render_data_queue);
    MemDel(render_component);
}


void Component_Render_AddRenderData(RenderComponent* render_component, RenderData* render_data)
{
    Queue_Push(RenderData*, render_component->m_render_data_queue, render_data);
}

void Component_Render_ClearRenderData_Plat(RenderComponent* render_component)
{
    Assert(false, "Waiting impl");
}

void RenderManager_RenderEachActor_Plat(Actor* actor, tptr ptr)
{
    RenderComponent* render_component = Actor_Component_Cast(actor, Render);
    if( render_component )
    {
        Queue_ForEach(render_component->m_render_data_queue, RenderManager_RenderEachRenderData_Plat, NULL);
    }
}
