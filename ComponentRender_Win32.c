#include "CoreMini.h"


#include "Actor.h"
#include "Component.h"
#include "Queue.h"

#define LOCAL_NAME  "RenderComponent"


void RenderManager_RenderEachRenderData2D_Plat(RenderData2D* render_data_2d, tptr);


////////////////////////////////////////////////////////////////////////////////
void Component_Render_ClearAllRenderData2D_Plat(RenderComponent* render_component)
{
    // TODO:
    // Assert(false, "Waiting impl");
}

void RenderManager_RenderEachActor_Plat(Actor* actor, tptr ptr)
{
    RenderComponent* render_component = Actor_Component_Cast(actor, Render);
    if( render_component )
    {
        Queue_ForEach(Component_Render_GetRenderData2DQueue(render_component), RenderManager_RenderEachRenderData2D_Plat, NULL);
    }
}

