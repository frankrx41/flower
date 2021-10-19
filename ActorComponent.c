#include "CoreMini.h"

#include "Event.h"
#include "KeyId.h"

#include "EventManager.h"
#include "TaskManager.h"

#include "Component.h"
#include "ControlComponent.h"
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
vec3 Actor_Component_Physics_Location_Get(Actor* actor)
{
    Assert(actor != NULL, "");
    PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
    Assert(physics_component != NULL, "");

    return
    Component_Physics_GetLocation(physics_component);
}

void Actor_Component_Physics_Location_Set(Actor* actor, vec3 vec)
{
    Assert(actor != NULL, "");
    PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
    Assert(physics_component != NULL, "");
    Component_Physics_SetLocation(physics_component, vec);
}

void Actor_Component_Physics_Location_Move(Actor* actor, vec3 offset_vec)
{
    Assert(actor != NULL, "");
    PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
    Assert(physics_component != NULL, "");
    Component_Physics_MoveLocation(physics_component, offset_vec);
}

vec3 Actor_Component_Physics_Velocity_Get(Actor* actor)
{
    Assert(actor != NULL, "");
    PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
    Assert(physics_component != NULL, "");

    return
    Component_Physics_GetVelocity(physics_component);
}

void Actor_Component_Physics_Velocity_Set(Actor* actor, vec3 velocity)
{
    Assert(actor != NULL, "");
    PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
    Assert(physics_component != NULL, "");
    Component_Physics_SetVelocity(physics_component, velocity);
}

vec3 Actor_Component_Physics_Acceleration_Get(Actor* actor)
{
    Assert(actor != NULL, "");
    PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
    Assert(physics_component != NULL, "");

    return
    Component_Physics_GetAcceleration(physics_component);
}

void Actor_Component_Physics_Acceleration_Set(Actor* actor, vec3 acceleration)
{
    Assert(actor != NULL, "");
    PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
    Assert(physics_component != NULL, "");
    Component_Physics_SetAcceleration(physics_component, acceleration);
}

void CallBack_Actor_Component_Physics_Simulate(Actor* actor, float* delta_seconds)
{
    PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
    Assert(physics_component != NULL, "");
    Component_Physics_Simulate(physics_component, *delta_seconds);
}

void Actor_Component_Physics_SetEnableSimulate(Actor* actor, bool is_enable_simulate)
{
    Assert(actor != NULL, "");
    PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
    Assert(physics_component != NULL, "");

    Component_Physics_SetEnableSimulate(physics_component, is_enable_simulate);

    if( is_enable_simulate )
    {
        Scene_PhysicsGroup_Actor_Add(Actor_OwnerScene_Get(actor), actor);
    }
    else
    {
        Scene_PhysicsGroup_Actor_Remove(Actor_OwnerScene_Get(actor), actor);
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
        ShaderText* shader_text = ShaderText_Create(Actor_LocalName_Str_Get(actor), false, vec, str);
        Component_Render_ShaderText_Add(render_component, shader_text);
        return shader_text;
    }
    return NULL;
}

void Actor_Component_Render_ShaderText_Del(Actor* actor, ShaderText* shader_text)
{
    TODO("Actor_Component_Render_ShaderText_Del")
    Assert(false, "");
}

void Actor_Component_Render_ShaderText_Clear(Actor* actor)
{
    Assert(actor != NULL, "");
    RenderComponent* render_component = Actor_Component_Cast(actor, Component_Render);
    Assert(render_component != NULL, "");
    if( render_component )
    {
        Component_Render_ShaderText_ClearAll(render_component);
    }
}


// Component_Control
void Actor_Component_Control_EventRespond_Add(Actor* actor, Event event, CB_RespondCondition_Bool_Actor_EventInfo cb_respond_condition_void_actor_eventinfo, CB_RespondAction_Void_Actor_EventInfo cb_respond_action_void_actor_eventinfo)
{
    Assert(IS_IN_RANGE(event, Event_Scene_Min, Event_Scene_Max) || IS_IN_RANGE(event, Event_Control_Min, Event_Control_Max), "");
    Assert(actor != NULL, "");

    ControlComponent* control_component = Actor_Component_Cast(actor, Component_Control);
    Assert(control_component != NULL, "");
    if( control_component )
    {
        Component_Control_EventRespond_Add(control_component, event, cb_respond_condition_void_actor_eventinfo, cb_respond_action_void_actor_eventinfo);
        if(IS_IN_RANGE(event, Event_Scene_Min, Event_Scene_Max))
        {
            Scene_SceneEventGroup_Actor_Add(Actor_OwnerScene_Get(actor), actor, event);
        }
        else if(IS_IN_RANGE(event, Event_Actor_Min, Event_Actor_Max))
        {
            // We add to queue when Actor_Component is been create, not need this any more
            // Scene_ControlEventGroup_Actor_Add(Actor_OwnerScene_Get(actor), actor, event);
        }
    }
}

void Actor_Component_Control_EventRespond_Del(Actor* actor, Event event)
{
    Assert(actor != NULL, "");
    ControlComponent* control_component = Actor_Component_Cast(actor, Component_Control);
    Assert(control_component != NULL, "");
    if( control_component )
    {
        Component_Control_EventRespond_Del(control_component, event);
    }
}

void Actor_Component_Control_EventRespond_Clear(Actor* actor)
{
    Assert(actor != NULL, "");
    ControlComponent* control_component = Actor_Component_Cast(actor, Component_Control);
    Assert(control_component != NULL, "");
    if( control_component )
    {
        Component_Control_EventRespond_Clear(control_component);
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
        Component_Storage_Is_ExistVariable(storage_component, variable);
    }
    return false;
}

void Actor_Component_Storage_Data_Store(Actor* actor, crc32 variable, tdata data)
{
    Assert(actor != NULL, "");
    StorageComponent* storage_component = Actor_Component_Cast(actor, Component_Storage);
    Assert(storage_component != NULL, "");
    if( storage_component )
    {
        Component_Storage_Data_Store(storage_component, variable, data);
    }
}

tdata Actor_Component_Storage_Data_Read(Actor* actor, crc32 variable)
{
    Assert(actor != NULL, "");
    StorageComponent* storage_component = Actor_Component_Cast(actor, Component_Storage);
    Assert(storage_component != NULL, "");
    if( storage_component )
    {
        return
        Component_Storage_Data_Read(storage_component, variable);
    }
    return tdata_null;
}

void Actor_Component_Storage_Variable_Delete(Actor* actor, crc32 variable)
{
    Assert(actor != NULL, "");
    StorageComponent* storage_component = Actor_Component_Cast(actor, Component_Storage);
    Assert(storage_component != NULL, "");
    if( storage_component )
    {
        Component_Storage_Variable_Delete(storage_component, variable);
    }
}


// CallBack
////////////////////////////////////////////////////////////////////////////////
typedef struct RenderManager RenderManager;
void CallBack_RenderManager_Render_ToBackBuffer_Task(Task* task, ShaderText* shader_text)
{
    RenderManager_Render_ToBackBuffer(RenderManager_GetInstance(), ShaderText_Offset_Get(shader_text), shader_text);
}

void CallBack_Render_ActorShaderText_Plat(ShaderText* shader_text, Actor* actor)
{
    if( ShaderText_IsDisable(shader_text) )
    {
        return;
    }

    vec3 vec = ShaderText_GetVec3(shader_text);
    if( !ShaderText_Is_Absolute(shader_text) )
    {
        if (Actor_Component_Cast(actor, Component_Physics))
        {
            vec = Vec3_Add(vec, Actor_Component_Physics_Location_Get(actor));
        }
    }


    ShaderText* shader_text_copy = ShaderText_Create("RenderManager_ShaderText", true,vec, ShaderText_GetStr(shader_text));
    ShaderText_Offset_Set(shader_text_copy, Scene_Render_Offset_Get(Actor_OwnerScene_Get(actor)));
    TaskManager_Task_Render_Add("RenderManager_ShaderText_Task", CallBack_RenderManager_Render_ToBackBuffer_Task, ShaderText_Destory, shader_text_copy);
}

void CallBack_Actor_RenderEachActor(Actor* actor, RenderManager* render_manager)
{
    if( Actor_Is_Hide(actor) )
    {
        return;
    }
    RenderComponent* render_component = Actor_Component_Cast(actor, Component_Render);
    Assert(render_component != false, "");

    Queue_ForEach(Component_Render_ShaderText_GetQueue(render_component), CallBack_Render_ActorShaderText_Plat, actor);
}
