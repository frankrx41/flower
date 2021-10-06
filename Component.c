#include "CoreMini.h"

#include "Component.h"
#include "Actor.h"
#include "MemoryManager.h"
#include "Queue.h"
#include "Render.h"


struct RenderComponent
{
    Queue(RenderDataText*)* m_render_data_text_queue;
};

tptr    Component_Render_RenderDataText_GetQueue    (RenderComponent* render_component);
void    Component_Render_RenderDataText_Add         (RenderComponent* render_component, RenderDataText* render_data);
void    Component_Render_RenderDataText_ClearAll    (RenderComponent* render_component);


void RenderManager_RenderEachRenderDataText_Plat(RenderDataText* render_data_text, tptr);

////////////////////////////////////////////////////////////////////////////////
RenderComponent* Component_Render_Create(const tchar* local_name)
{
    RenderComponent* render_component = MemNew(local_name, RenderComponent);
    render_component->m_render_data_text_queue = Queue_Create(local_name, RenderDataText*);
    return render_component;
}

void Component_Render_Destroy(RenderComponent* render_component)
{
    Assert(render_component != NULL, "");
    Component_Render_RenderDataText_ClearAll(render_component);
    Queue_Destroy(render_component->m_render_data_text_queue);
    MemDel(render_component);
}

tptr Component_Render_RenderDataText_GetQueue(RenderComponent* render_component)
{
    Assert(render_component != NULL, "");
    return render_component->m_render_data_text_queue;
}

void Component_Render_RenderDataText_Add(RenderComponent* render_component, RenderDataText* render_data)
{
    Assert(render_component != NULL, "");
    Queue_Push(RenderDataText*, render_component->m_render_data_text_queue, render_data);
}

static void RenderComponent_DeleteEach(RenderDataText* render_data_text, tptr ptr)
{
    RenderDataText_Destory(render_data_text);
}

void Component_Render_RenderDataText_ClearAll(RenderComponent* render_component)
{
    Assert(render_component != NULL, "");
    Queue_ForEach(render_component->m_render_data_text_queue, RenderComponent_DeleteEach, NULL);
}

////////////////////////////////////////////////////////////////////////////////
RenderDataText* Actor_Component_Render_RenderDataText_Add(Actor* actor, uint32 x, uint32 y, const tchar* str)
{
    Assert(actor != NULL, "");
    RenderComponent* render_component = Actor_Component_Cast(actor, Component_Render);
    if( render_component )
    {
        RenderDataText* render_data = RenderDataText_Create(Actor_GetLocalName(actor), x, y, str);
        Component_Render_RenderDataText_Add(render_component, render_data);
        return render_data;
    }
    return NULL;
}

void Actor_Component_Render_RenderDataText_ClearAll(Actor* actor)
{
    RenderComponent* render_component = Actor_Component_Cast(actor, Component_Render);
    if( render_component )
    {
        Component_Render_RenderDataText_ClearAll(render_component);
    }
}

////////////////////////////////////////////////////////////////////////////////
void RenderManager_RenderEachActor(Actor* actor, tptr ptr)
{
    RenderComponent* render_component = Actor_Component_Cast(actor, Component_Render);
    if( render_component )
    {
        Queue_ForEach(Component_Render_RenderDataText_GetQueue(render_component), RenderManager_RenderEachRenderDataText_Plat, NULL);
    }
}