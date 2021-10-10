#include "CoreMini.h"

#include "Event.h"
#include "KeyId.h"

#include "EventManager.h"

#include "Component.h"
#include "ActionComponent.h"
#include "LocationComponent.h"
#include "RenderComponent.h"
#include "StorageComponent.h"
#include "RenderManager.h"

#include "ShaderText.h"

#include "tData.h"
#include "Actor.h"
#include "Queue.h"
#include "Vec.h"
#include "Scene.h"

// Location
void Actor_Component_Location_Set(Actor* actor, vec3 vec)
{
    Assert(actor != NULL, "");
    LocationComponent* location_component = Actor_Component_Cast(actor, Component_Location);
    Assert(location_component != NULL, "");
    Component_Location_Set(location_component, vec);
}

void Actor_Component_Location_Move(Actor* actor, vec3 offset_vec)
{
    Assert(actor != NULL, "");
    LocationComponent* location_component = Actor_Component_Cast(actor, Component_Location);
    Assert(location_component != NULL, "");
    Component_Location_Move(location_component, offset_vec);
}

vec3 Actor_Component_Location_Get(Actor* actor)
{
    Assert(actor != NULL, "");
    LocationComponent* location_component = Actor_Component_Cast(actor, Component_Location);
    Assert(location_component != NULL, "");
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
void Actor_Component_Action_EventRespond_Add(Actor* actor, Event event, CB_EventRespond_Void_Actor_EventInfo cb_event_respond)
{
    Assert(actor != NULL, "");
    ActionComponent* action_component = Actor_Component_Cast(actor, Component_Action);
    Assert(action_component != NULL, "");
    if( action_component )
    {
        Component_Action_EventRespond_Add(action_component, event, cb_event_respond);
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

void Actor_Component_Storage_StoreData32(Actor* actor, crc32 variable, tdata data)
{
    Assert(actor != NULL, "");
    StorageComponent* storage_component = Actor_Component_Cast(actor, Component_Storage);
    Assert(storage_component != NULL, "");
    if( storage_component )
    {
        Component_Storage_StoreData32(storage_component, variable, data);
    }
}

tdata Actor_Component_Storage_ReadData32(Actor* actor, crc32 variable)
{
    Assert(actor != NULL, "");
    StorageComponent* storage_component = Actor_Component_Cast(actor, Component_Storage);
    Assert(storage_component != NULL, "");
    if( storage_component )
    {
        return
        Component_Storage_ReadData32(storage_component, variable);
    }
    return data32_null;
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
    RenderComponent* render_component = Actor_Component_Cast(actor, Component_Render);
    if( render_component )
    {
        vec3 vec = vec3_null;
        if( Actor_Component_Cast(actor, Component_Location) )
        {
            vec = Actor_Component_Location_Get(actor);
        }
        RenderManagerWithVec render_manager_with_vec;
        render_manager_with_vec.m_vec3              = vec;
        render_manager_with_vec.m_render_manager    = render_manager;
        Queue_ForEach(Component_Render_ShaderText_GetQueue(render_component), (CB_ProcessData_Void_tPtr_tPtr)CallBack_Render_ShaderText_Plat, &render_manager_with_vec);
    }
}

void CallBack_Actor_ProcessEachActorEvent(Actor* actor, const EventInfo* event_struct)
{
    ActionComponent* action_component = Actor_Component_Cast(actor, Component_Action);

    const CB_EventRespond_Void_Actor_EventInfo cb_actor_respond = Component_Action_EventRespond_Get(action_component, event_struct->m_event);

    Assert(cb_actor_respond != NULL, "");

    cb_actor_respond(actor, event_struct);
}
