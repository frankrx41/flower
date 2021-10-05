#include "CoreMini.h"


#include "Actor.h"
#include "Component.h"
#include "Memory.h"
#include "Queue.h"
#include "Render.h"
#include "String.h"

#define LOCAL_NAME  "RenderComponent"

void Component_Render_ClearAllRenderData_Plat(RenderComponent* render_component);
void RenderManager_RenderEachRenderData_Plat(RenderData* render_data, tptr);


////////////////////////////////////////////////////////////////////////////////
void Component_Render_ClearAllRenderData_Plat(RenderComponent* render_component)
{
    // TODO:
    // Assert(false, "Waiting impl");
}

void RenderManager_RenderEachActor_Plat(Actor* actor, tptr ptr)
{
    RenderComponent* render_component = Actor_Component_Cast(actor, Render);
    if( render_component )
    {
        Queue_ForEach(Component_Render_GetRenderDataQueue(render_component), RenderManager_RenderEachRenderData_Plat, NULL);
    }
}

