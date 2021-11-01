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
#include "String.h"
#include "Viewport.h"
//
// // Location
// vec3* Actor_Component_Physics_Location_Get(const Actor* actor)
// {
//     Assert(actor != NULL, "");
//     PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
//     Assert(physics_component != NULL, "");
//
//     return
//     Component_Physics_GetLocation(physics_component);
// }
//
// void Actor_Component_Physics_Location_Set(Actor* actor, const vec3* vec)
// {
//     Assert(actor != NULL, "");
//     PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
//     Assert(physics_component != NULL, "");
//     Component_Physics_SetLocation(physics_component, vec);
// }
//
// void Actor_Component_Physics_Location_Move(Actor* actor, const vec3* offset_vec)
// {
//     Assert(actor != NULL, "");
//     PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
//     Assert(physics_component != NULL, "");
//     Component_Physics_MoveLocation(physics_component, offset_vec);
// }
//
// vec3* Actor_Component_Physics_Velocity_Get(const Actor* actor)
// {
//     Assert(actor != NULL, "");
//     PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
//     Assert(physics_component != NULL, "");
//
//     return
//     Component_Physics_GetVelocity(physics_component);
// }
//
// void Actor_Component_Physics_Velocity_Set(Actor* actor, const vec3* velocity)
// {
//     Assert(actor != NULL, "");
//     PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
//     Assert(physics_component != NULL, "");
//     Component_Physics_SetVelocity(physics_component, velocity);
// }
//
// vec3* Actor_Component_Physics_Acceleration_Get(const Actor* actor)
// {
//     Assert(actor != NULL, "");
//     PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
//     Assert(physics_component != NULL, "");
//
//     return
//     Component_Physics_GetAcceleration(physics_component);
// }
//
// void Actor_Component_Physics_Acceleration_Set(Actor* actor, const vec3* acceleration)
// {
//     Assert(actor != NULL, "");
//     PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
//     Assert(physics_component != NULL, "");
//     Component_Physics_SetAcceleration(physics_component, acceleration);
// }
//
void CallBack_Actor_Component_Physics_Simulate(Actor* actor, const float* delta_seconds)
{
    Component* component = Actor_Component_Cast(actor, Component_Physics);
    Assert(component != NULL, "");
    Component_Physics_Simulate(component, *delta_seconds);
}
//
// void Actor_Component_Physics_SetEnableSimulate(Actor* actor, bool is_enable_simulate)
// {
//     Assert(actor != NULL, "");
//     PhysicsComponent* physics_component = Actor_Component_Cast(actor, Component_Physics);
//     Assert(physics_component != NULL, "");
//
//     Component_Physics_SetEnableSimulate(physics_component, is_enable_simulate);
//
//     if( is_enable_simulate )
//     {
//         Scene_PhysicsGroup_Actor_Add(Actor_GetExistScene(actor), actor);
//     }
//     else
//     {
//         Scene_PhysicsGroup_Actor_Remove(Actor_GetExistScene(actor), actor);
//     }
// }
//
//
// // Component_Render
// ShaderText* Actor_Component_Render_ShaderText_Add(Actor* actor, bool is_absolute, const vec3* position, const void* info, const tchar* str)
// {
//     Assert(actor != NULL, "");
//     RenderComponent* render_component = Actor_Component_Cast(actor, Component_Render);
//     Assert(render_component != NULL, "");
//     if( render_component )
//     {
//         ShaderText* shader_text = ShaderText_Create(Actor_GetLocalName(actor), is_absolute, position, info, str);
//         Component_Render_ShaderText_Add(render_component, shader_text);
//         return shader_text;
//     }
//     return NULL;
// }
//
// void Actor_Component_Render_ShaderText_Del(Actor* actor, ShaderText* shader_text)
// {
//     TODO("Actor_Component_Render_ShaderText_Del")
//     Assert(false, "");
// }
//
// void Actor_Component_Render_ShaderText_Clear(Actor* actor)
// {
//     Assert(actor != NULL, "");
//     RenderComponent* render_component = Actor_Component_Cast(actor, Component_Render);
//     Assert(render_component != NULL, "");
//     if( render_component )
//     {
//         Component_Render_ShaderText_ClearAll(render_component);
//     }
// }
//
//
// // Component_Control
// void Actor_Component_Control_SceneEventRespond_Add(Actor* actor, EventId event, CB_RespondCondition_Bool_Actor_EventInfo cb_respond_condition_void_actor_eventinfo, CB_RespondAction_Void_Actor_EventInfo cb_respond_action_void_actor_eventinfo)
// {
//     Assert(IS_IN_RANGE(event, Event_Scene_Min, Event_Scene_Max), "");
//     Assert(event != Event_Scene_Update_Physics, "Update_Physics is handle by engine");
//
//     ControlComponent* control_component = Actor_Component_Cast(actor, Component_Control);
//     Assert(control_component != NULL, "");
//     if( control_component )
//     {
//         Component_Control_EventRespond_Add(control_component, event, cb_respond_condition_void_actor_eventinfo, cb_respond_action_void_actor_eventinfo);
//         Scene_SceneEventGroup_Actor_Add(Actor_GetExistScene(actor), actor, event);
//     }
// }
//
// void Actor_Component_Control_ControlEventRespond_Add(Actor* actor, EventId event, CB_RespondCondition_Bool_Actor_EventInfo cb_respond_condition_void_actor_eventinfo, CB_RespondAction_Void_Actor_EventInfo cb_respond_action_void_actor_eventinfo)
// {
//     Assert(IS_IN_RANGE(event, Event_Control_Min, Event_Control_Max), "");
//     Assert(actor != NULL, "");
//
//     ControlComponent* control_component = Actor_Component_Cast(actor, Component_Control);
//     Assert(control_component != NULL, "");
//     if( control_component )
//     {
//         Component_Control_EventRespond_Add(control_component, event, cb_respond_condition_void_actor_eventinfo, cb_respond_action_void_actor_eventinfo);
//         if(IS_IN_RANGE(event, Event_Scene_Min, Event_Scene_Max))
//         {
//             Scene_SceneEventGroup_Actor_Add(Actor_GetExistScene(actor), actor, event);
//         }
//     }
// }
//
// void Actor_Component_Control_ControlEventRespond_Del(Actor* actor, EventId event)
// {
//     Assert(actor != NULL, "");
//     ControlComponent* control_component = Actor_Component_Cast(actor, Component_Control);
//     Assert(control_component != NULL, "");
//     if( control_component )
//     {
//         Component_Control_EventRespond_Del(control_component, event);
//     }
// }
//
// void Actor_Component_Control_ControlEventRespond_Clear(Actor* actor)
// {
//     Assert(actor != NULL, "");
//     ControlComponent* control_component = Actor_Component_Cast(actor, Component_Control);
//     Assert(control_component != NULL, "");
//     if( control_component )
//     {
//         Component_Control_EventRespond_Clear(control_component);
//     }
// }
//
// // Component_Storage
// bool Actor_Component_Storage_IsExistVariable(Actor* actor, crc32 variable)
// {
//     Assert(actor != NULL, "");
//     StorageComponent* storage_component = Actor_Component_Cast(actor, Component_Storage);
//     Assert(storage_component != NULL, "");
//     if( storage_component )
//     {
//         return
//         Component_Storage_Is_ExistVariable(storage_component, variable);
//     }
//     return false;
// }
//
// void Actor_Component_Storage_Data_Store(Actor* actor, crc32 variable, tdata data)
// {
//     Assert(actor != NULL, "");
//     StorageComponent* storage_component = Actor_Component_Cast(actor, Component_Storage);
//     Assert(storage_component != NULL, "");
//     if( storage_component )
//     {
//         Component_Storage_Data_Store(storage_component, variable, data);
//     }
// }
//
// tdata Actor_Component_Storage_Data_Read(Actor* actor, crc32 variable)
// {
//     Assert(actor != NULL, "");
//     StorageComponent* storage_component = Actor_Component_Cast(actor, Component_Storage);
//     Assert(storage_component != NULL, "");
//     if( storage_component )
//     {
//         return
//         Component_Storage_Data_Read(storage_component, variable);
//     }
//     return tdata_null;
// }
//
// void Actor_Component_Storage_Variable_Delete(Actor* actor, crc32 variable)
// {
//     Assert(actor != NULL, "");
//     StorageComponent* storage_component = Actor_Component_Cast(actor, Component_Storage);
//     Assert(storage_component != NULL, "");
//     if( storage_component )
//     {
//         Component_Storage_Variable_Delete(storage_component, variable);
//     }
// }
//
//
// // CallBack
// ////////////////////////////////////////////////////////////////////////////////
static void CallBack_Render_ActorShaderText(ShaderText* shader_text, const Actor* actor)
{
    if( ShaderText_Is_Disable(shader_text) )
    {
        return;
    }

    vec3 position = *ShaderText_Position_Get(shader_text);
    if( !ShaderText_Is_Absolute(shader_text) )
    {
        Component* component = Actor_Component_Cast(actor, Component_Physics);
        if( component )
        {
            Vec3_Add(&position, Component_Physics_GetLocation(component), &position);
        }
    }

    strcrc local_name;

    local_name = StrCrc("RenderManager_ShaderText", 0);
    ShaderText* shader_text_copy = ShaderText_Create(&local_name, true, &position, ShaderText_Info_Get(shader_text), ShaderText_Str_Get(shader_text));
    
    Viewport* viewport = Scene_GetViewport(Actor_GetExistScene(actor));
    Viewport_Render_ShaderText(viewport, shader_text_copy);

    ShaderText_Destory(shader_text_copy);
}

void CallBack_Actor_RenderTo_Scene_Viewport(Actor* actor, const Scene* scene)
{
    if( Actor_IsHide(actor) )
    {
        return;
    }
    Component* render_component = Actor_Component_Cast(actor, Component_Render);
    Assert(render_component != false, "");

    Queue_ForEach(Component_Render_ShaderText_GetQueue(render_component), CallBack_Render_ActorShaderText, actor);
}
