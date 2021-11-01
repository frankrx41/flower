#include "CoreMini.h"


#include "MemoryManager.h"
#include "RenderComponent.h"
#include "ShaderText.h"

#include "Actor.h"
#include "Scene.h"
#include "Queue.h"


struct RenderComponent
{
    Queue(ShaderText*)* m_shader_text_queue;
};

////////////////////////////////////////////////////////////////////////////////
RenderComponent* Component_Render_Create(const strcrc* local_name, Actor* actor)
{
    RenderComponent* render_component = MemNew(local_name, RenderComponent);
    render_component->m_shader_text_queue = Queue_Create(local_name, ShaderText*);

    Scene_ActorQueue_Renderable_Add(Actor_GetExistScene(actor), actor);

    Assert(Scene_GetViewport(Actor_GetExistScene(actor)) != NULL, "");

    return render_component;
}

void Component_Render_Destroy(RenderComponent* render_component)
{
    Assert(render_component != NULL, "");
    Component_Render_ShaderText_ClearAll(render_component);
    Queue_Destroy(render_component->m_shader_text_queue, ShaderText_Destory);
    MemDel(render_component);
}

void* Component_Render_ShaderText_GetQueue(RenderComponent* render_component)
{
    Assert(render_component != NULL, "");
    return render_component->m_shader_text_queue;
}

void Component_Render_ShaderText_Add(RenderComponent* render_component, ShaderText* shader_text)
{
    Assert(render_component != NULL, "");
    Queue_Push(ShaderText*, NULL, render_component->m_shader_text_queue, shader_text);
}

void Component_Render_ShaderText_ClearAll(RenderComponent* render_component)
{
    Assert(render_component != NULL, "");
    Queue_Clear(render_component->m_shader_text_queue, ShaderText_Destory);
}

