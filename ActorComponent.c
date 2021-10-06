#include "CoreMini.h"

#include "Actor.h"
#include "Component.h"
#include "ActionComponent.h"
#include "LocationComponent.h"
#include "Queue.h"
#include "RenderComponent.h"
#include "Vec.h"
#include "ShaderText.h"

void CallBack_Render_ShaderText_Plat(ShaderText* shader_text, vec3 vec);


// Location
void Actor_Component_Location_Set(Actor* actor, vec3 vec)
{
    LocationComponent* location_component = Actor_Component_Cast(actor, Component_Location);
    Component_Location_Set(location_component, vec);
}

void Actor_Component_Location_Move(Actor* actor, vec3 offset_vec)
{
    LocationComponent* location_component = Actor_Component_Cast(actor, Component_Location);
    Component_Location_Move(location_component, offset_vec);
}

vec3 Actor_Component_Location_Get(Actor* actor)
{
    LocationComponent* location_component = Actor_Component_Cast(actor, Component_Location);
    if( location_component )
    {
        return
        Component_Location_Get(location_component);
    }
    return vec3_null;
}


// Component_Render
ShaderText* Actor_Component_Render_ShaderText_Add(Actor* actor, vec3 vec, const tchar* str)
{
    Assert(actor != NULL, "");
    RenderComponent* render_component = Actor_Component_Cast(actor, Component_Render);
    if( render_component )
    {
        ShaderText* shader_text = ShaderText_Create(Actor_GetLocalName(actor), vec, str);
        Component_Render_ShaderText_Add(render_component, shader_text);
        return shader_text;
    }
    return NULL;
}

void Actor_Component_Render_ShaderText_Del(Actor* actor, ShaderText* shader_text)
{
    // TODO:
    Assert(false, "");
}

void Actor_Component_Render_ShaderText_ClearAll(Actor* actor)
{
    Assert(actor != NULL, "");
    RenderComponent* render_component = Actor_Component_Cast(actor, Component_Render);
    if( render_component )
    {
        Component_Render_ShaderText_ClearAll(render_component);
    }
}


// Component_Action
void Actor_Component_Action_EventRespond_Add(Actor* actor, Event event, CB_EventRespond cb_event_respond)
{
    Assert(actor != NULL, "");
    ActionComponent* action_component = Actor_Component_Cast(actor, Component_Action);
    if( action_component )
    {
        Component_Action_EventRespond_Add(action_component, event, cb_event_respond);
    }
}

void Actor_Component_Action_EventRespond_Del(Actor* actor, Event event)
{
    Assert(actor != NULL, "");
    ActionComponent* action_component = Actor_Component_Cast(actor, Component_Action);
    if( action_component )
    {
        Component_Action_EventRespond_Del(action_component, event);
    }
}

void Actor_Component_Action_EventRespond_Clear(Actor* actor)
{
    Assert(actor != NULL, "");
    ActionComponent* action_component = Actor_Component_Cast(actor, Component_Action);
    if( action_component )
    {
        Component_Action_EventRespond_Clear(action_component);
    }
}


// CallBack
void CallBack_Actor_RenderEachActor(Actor* actor, tptr ptr)
{
    RenderComponent* render_component = Actor_Component_Cast(actor, Component_Render);
    if( render_component )
    {
        vec3 vec = Actor_Component_Location_Get(actor);
        Queue_ForEach(Component_Render_ShaderText_GetQueue(render_component), (CB_ProcessData)CallBack_Render_ShaderText_Plat, &vec);
    }
}

void CallBack_Actor_ProcessEachActorEvent(Actor* actor, Event event)
{
    ActionComponent* action_component = Actor_Component_Cast(actor, Component_Action);
    CB_EventRespond cb_actor_respond = Component_Action_EventRespond_Get(action_component, event);
    cb_actor_respond(actor, NULL);
}
