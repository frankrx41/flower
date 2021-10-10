#include "CoreMini.h"

#include "Event.h"
#include "KeyId.h"

#include "EventManager.h"

#include "Component.h"
#include "ActionComponent.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "StorageComponent.h"
#include "RenderManager.h"
#include "ActorComponent.h"

#include "ShaderText.h"

#include "tData.h"
#include "Actor.h"
#include "Queue.h"
#include "Vec.h"
#include "Scene.h"

// Location
vec3 Actor_Component_Physics_GetLocation(Actor* actor)
{
    Assert(actor != NULL, "");
    PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
    Assert(physics_component != NULL, "");

    return
    Component_Physics_GetLocation(physics_component);
}

void Actor_Component_Physics_SetLocation(Actor* actor, vec3 vec)
{
    Assert(actor != NULL, "");
    PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
    Assert(physics_component != NULL, "");
    Component_Physics_SetLocation(physics_component, vec);
}

void Actor_Component_Physics_MoveLocation(Actor* actor, vec3 offset_vec)
{
    Assert(actor != NULL, "");
    PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
    Assert(physics_component != NULL, "");
    Component_Physics_MoveLocation(physics_component, offset_vec);
}

vec3 Actor_Component_Physics_GetVelocity(Actor* actor)
{
    Assert(actor != NULL, "");
    PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
    Assert(physics_component != NULL, "");

    return
    Component_Physics_GetVelocity(physics_component);
}

void Actor_Component_Physics_SetVelocity(Actor* actor, vec3 velocity)
{
    Assert(actor != NULL, "");
    PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
    Assert(physics_component != NULL, "");
    Component_Physics_SetVelocity(physics_component, velocity);
}

vec3 Actor_Component_Physics_GetAcceleration(Actor* actor)
{
    Assert(actor != NULL, "");
    PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
    Assert(physics_component != NULL, "");

    return
    Component_Physics_GetAcceleration(physics_component);
}

void Actor_Component_Physics_SetAcceleration(Actor* actor, vec3 acceleration)
{
    Assert(actor != NULL, "");
    PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
    Assert(physics_component != NULL, "");
    Component_Physics_SetAcceleration(physics_component, acceleration);
}

void CallBack_Event_Scene_Physics_Update(Actor* actor, const EventInfo* event_info)
{
    PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
    Assert(physics_component != NULL, "");
    Component_Physics_Simulate(physics_component, event_info->m_delta_seconds);
}

void Actor_Component_Physics_SetEnableSimulate(Actor* actor, bool is_enable_simulater)
{
    Assert(actor != NULL, "");
    PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
    Assert(physics_component != NULL, "");

    Component_Physics_SetEnableSimulate(physics_component, is_enable_simulater);

    ActionComponent* action_component = Actor_Component_Cast(actor, Component_Action);
    Assert(action_component != NULL, "You need ActionComponent to do physics simulate");
    if( is_enable_simulater )
    {
        Actor_Component_Action_EventRespond_Add(actor, Event_Scene_Physics_Update, NULL, CallBack_Event_Scene_Physics_Update);
    }
    else
    {
        Actor_Component_Action_EventRespond_Del(actor, Event_Scene_Physics_Update);
    }
}


// Component_Render
ShaderText* Actor_Component_Render_ShaderText_Add(Actor* actor, vec3 vec, const tchar* str)
{
    Assert(actor != NULL, "");
    RenderComponent* render_component = Actor_Component_Cast(actor, Component_Render);
    Assert(render_component != NULL, "");
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
    Assert(render_component != NULL, "");
    if( render_component )
    {
        Component_Render_ShaderText_ClearAll(render_component);
    }
}


// Component_Action
void Actor_Component_Action_EventRespond_Add(Actor* actor, Event event, CB_RespondCondition_Bool_Actor_EventInfo cb_respond_condition_void_actor_eventinfo, CB_RespondAction_Void_Actor_EventInfo cb_respond_action_void_actor_eventinfo)
{
    Assert(IN_RANGE(event, Event_Scene_Min, Event_Scene_Max) || IN_RANGE(event, Event_Actor_Min, Event_Actor_Max), "");
    Assert(actor != NULL, "");

    ActionComponent* action_component = Actor_Component_Cast(actor, Component_Action);
    Assert(action_component != NULL, "");
    if( action_component )
    {
        Component_Action_EventRespond_Add(action_component, event, cb_respond_condition_void_actor_eventinfo, cb_respond_action_void_actor_eventinfo);
        Scene_Actor_AddEventGroup(Actor_GetScene(actor), actor, event);
    }
}

void Actor_Component_Action_EventRespond_Del(Actor* actor, Event event)
{
    Assert(actor != NULL, "");
    ActionComponent* action_component = Actor_Component_Cast(actor, Component_Action);
    Assert(action_component != NULL, "");
    if( action_component )
    {
        Component_Action_EventRespond_Del(action_component, event);
    }
}

void Actor_Component_Action_EventRespond_Clear(Actor* actor)
{
    Assert(actor != NULL, "");
    ActionComponent* action_component = Actor_Component_Cast(actor, Component_Action);
    Assert(action_component != NULL, "");
    if( action_component )
    {
        Component_Action_EventRespond_Clear(action_component);
    }
}

// Component_Storage
bool Actor_Component_Storage_IsExistVariable(Actor* actor, crc32 variable)
{
    Assert(actor != NULL, "");
    StorageComponent* storage_component = Actor_Component_Cast(actor, Component_Storage);
    Assert(storage_component != NULL, "");
    if( storage_component )
    {
        return
        Component_Storage_IsExistVariable(storage_component, variable);
    }
    return false;
}

void Actor_Component_Storage_StoreData(Actor* actor, crc32 variable, tdata data)
{
    Assert(actor != NULL, "");
    StorageComponent* storage_component = Actor_Component_Cast(actor, Component_Storage);
    Assert(storage_component != NULL, "");
    if( storage_component )
    {
        Component_Storage_StoreData(storage_component, variable, data);
    }
}

tdata Actor_Component_Storage_ReadData(Actor* actor, crc32 variable)
{
    Assert(actor != NULL, "");
    StorageComponent* storage_component = Actor_Component_Cast(actor, Component_Storage);
    Assert(storage_component != NULL, "");
    if( storage_component )
    {
        return
        Component_Storage_ReadData(storage_component, variable);
    }
    return tdata_null;
}

void Actor_Component_Storage_DeleteVariable(Actor* actor, crc32 variable)
{
    Assert(actor != NULL, "");
    StorageComponent* storage_component = Actor_Component_Cast(actor, Component_Storage);
    Assert(storage_component != NULL, "");
    if( storage_component )
    {
        Component_Storage_DeleteVariable(storage_component, variable);
    }
}


// CallBack
////////////////////////////////////////////////////////////////////////////////
typedef struct RenderManager RenderManager;

typedef struct RenderManagerWithVec RenderManagerWithVec;
struct RenderManagerWithVec
{
    RenderManager*  m_render_manager;
    vec3            m_vec3;
};

void CallBack_Render_ShaderText_Plat(ShaderText* shader_text, RenderManagerWithVec* render_manager_with_vec)
{
    if( ShaderText_IsDisable(shader_text) )
    {
        return;
    }

    vec3 vec = Vec3_Add(ShaderText_GetVec3(shader_text), render_manager_with_vec->m_vec3);

    RenderManager_Render_InBackBuffer(render_manager_with_vec->m_render_manager, (int32)vec.m_x, (int32)vec.m_y, ShaderText_GetStr(shader_text));
}

void CallBack_Actor_RenderEachActor(Actor* actor, RenderManager* render_manager)
{
    if( Actor_IsPause(actor) )
    {
        return;
    }
    RenderComponent* render_component = Actor_Component_Cast(actor, Component_Render);
    if( render_component )
    {
        vec3 vec = vec3_null;
        if( Actor_Component_Cast(actor, Component_Physics) )
        {
            vec = Actor_Component_Physics_GetLocation(actor);
        }
        RenderManagerWithVec render_manager_with_vec;
        render_manager_with_vec.m_vec3              = vec;
        render_manager_with_vec.m_render_manager    = render_manager;
        Queue_ForEach(Component_Render_ShaderText_GetQueue(render_component), (CB_ProcessData_Void_tPtr_tPtr)CallBack_Render_ShaderText_Plat, &render_manager_with_vec);
    }
}
