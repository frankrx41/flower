#include "CoreMini.h"

#include "Component.h"

#include "Actor.h"
#include "Memory.h"
#include "Queue.h"
#include "Render.h"

struct RenderComponent
{
    Queue(RenderData2D*)*     m_render_data_2d_queue;
};


void Component_Render_ClearAllRenderData2D_Plat(RenderComponent* render_component);

////////////////////////////////////////////////////////////////////////////////
RenderComponent* Component_Render_Create(const tchar* local_name)
{
    RenderComponent* render_component = MemNew(local_name, RenderComponent);
    render_component->m_render_data_2d_queue = Queue_Create(local_name, RenderData2D*);
    return render_component;
}

void Component_Render_Destroy(RenderComponent* render_component)
{
    Assert(render_component != NULL, "");
    Component_Render_ClearAllRenderData2D_Plat(render_component);
    Queue_Destroy(render_component->m_render_data_2d_queue);
    MemDel(render_component);
}

tptr Component_Render_GetRenderData2DQueue(RenderComponent* render_component)
{
    Assert(render_component != NULL, "");
    return render_component->m_render_data_2d_queue;
}

void Component_Render_AddRenderData2D(RenderComponent* render_component, RenderData2D* render_data)
{
    Assert(render_component != NULL, "");
    Queue_Push(RenderData2D*, render_component->m_render_data_2d_queue, render_data);
}

void Component_Render_ClearAllRenderData2D(RenderComponent* render_component)
{
    Assert(render_component != NULL, "");
    Component_Render_ClearAllRenderData2D_Plat(render_component);
}

////////////////////////////////////////////////////////////////////////////////
void Actor_RenderComponent_AddRenderData2D(Actor* actor, uint32 x, uint32 y, const tchar* str)
{
    Assert(actor != NULL, "");
    RenderComponent* render_component = Actor_Component_Cast(actor, Render);
    if( render_component )
    {
        RenderData2D* render_data = RenderData2D_Create(x, y, str);
        Component_Render_AddRenderData2D(render_component, render_data);
    }
}

void Actor_RenderComponent_ClearAllRenderData2D(Actor* actor)
{
    RenderComponent* render_component = Actor_Component_Cast(actor, Render);
    if( render_component )
    {
        Component_Render_ClearAllRenderData2D_Plat(render_component);
    }
}
