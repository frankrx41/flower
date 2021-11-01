#include "CoreMini.h"


#include "MemoryManager.h"
#include "RenderComponent.h"
#include "Component.h"
#include "ShaderText.h"

#include "Actor.h"
#include "Scene.h"
#include "Queue.h"

typedef struct RenderComponentData RenderComponentData;

struct RenderComponentData
{
    Queue(ShaderText*)* m_shader_text_queue;
};

////////////////////////////////////////////////////////////////////////////////
void* ComponentData_Render_Create(const strcrc* local_name, Actor* actor)
{
    RenderComponentData* render_component_data = MemNew(local_name, RenderComponentData);
    render_component_data->m_shader_text_queue = Queue_Create(local_name, ShaderText*);

    Scene_ActorQueue_Renderable_Add(Actor_GetExistScene(actor), actor);

    Assert(Scene_GetViewport(Actor_GetExistScene(actor)) != NULL, "");

    return render_component_data;
}

void ComponentData_Render_Destroy(Component* component)
{
    Assert(component != NULL, "");
    Assert(Component_GetEnum(component) == Component_Render, "");

    Component_Render_ShaderText_ClearAll(component);
    RenderComponentData* render_component_data = Component_GetData(component);
    Queue_Destroy(render_component_data->m_shader_text_queue, ShaderText_Destory);
    MemDel(render_component_data);
}

void* Component_Render_ShaderText_GetQueue(Component* component)
{
    Assert(component != NULL, "");
    Assert(Component_GetEnum(component) == Component_Render, "");

    RenderComponentData* render_component_data = Component_GetData(component);
    return render_component_data->m_shader_text_queue;
}

void Component_Render_ShaderText_Add(Component* component, ShaderText* shader_text)
{
    Assert(component != NULL, "");
    Assert(Component_GetEnum(component) == Component_Render, "");

    RenderComponentData* render_component_data = Component_GetData(component);
    Queue_Push(ShaderText*, NULL, render_component_data->m_shader_text_queue, shader_text);
}

void Component_Render_ShaderText_ClearAll(Component* component)
{
    Assert(component != NULL, "");
    Assert(Component_GetEnum(component) == Component_Render, "");

    RenderComponentData* render_component_data = Component_GetData(component);
    Queue_Clear(render_component_data->m_shader_text_queue, ShaderText_Destory);
}

