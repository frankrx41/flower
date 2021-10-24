#include "CoreMini.h"

#include "Engine.h"

#include "Event.h"
#include "KeyId.h"
#include "Task.h"

#include "MemoryManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "EventManager.h"
#include "InputManager.h"
#include "TaskManager.h"

#include "Component.h"

#include "ActorComponent.h"
#include "ControlComponent.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "StorageComponent.h"

#include "String.h"
#include "Queue.h"
#include "Storage.h"
#include "Actor.h"
#include "Scene.h"
#include "tData.h"
#include "Vec.h"

static void Actor_OnAction0(Actor* actor, const EventInfo* event_info)
{
    if( event_info->m_event == Event_Control_Cancel )
    {
        SceneManager_Scene_ExitCurrent();
        return;
    }

    if( event_info->m_event == Event_Control_Pause )
    {
        return;
    }

    vec3 vec3_temp;

    vec3_temp = Vec3(0, -0.1f, 0);
    switch( event_info->m_event )
    {
    case Event_Control_MoveUp:      vec3_temp = Vec3(0, -1, 0); break;
    case Event_Control_MoveDown:    vec3_temp = Vec3(0, +1, 0); break;
    case Event_Control_MoveLeft:    vec3_temp = Vec3(-1, 0, 0); break;
    case Event_Control_MoveRight:   vec3_temp = Vec3(+1, 0, 0); break;
    default:    Assert(false, "");
    }

    Actor_Component_Physics_Location_Move(actor, &vec3_temp);
}

static void CallBack_Actor_Create0(Actor* actor, const void* ptr)
{
    Actor_Component_New(actor, Component_Control);
    Actor_Component_Control_ControlEventRespond_Add(actor, Event_Control_MoveUp, NULL, Actor_OnAction0 );
    Actor_Component_Control_ControlEventRespond_Add(actor, Event_Control_MoveDown, NULL, Actor_OnAction0 );
    Actor_Component_Control_ControlEventRespond_Add(actor, Event_Control_MoveLeft, NULL, Actor_OnAction0 );
    Actor_Component_Control_ControlEventRespond_Add(actor, Event_Control_MoveRight, NULL, Actor_OnAction0 );
    Actor_Component_Control_ControlEventRespond_Add(actor, Event_Control_Cancel, NULL, Actor_OnAction0 );
    Actor_Component_Control_ControlEventRespond_Add(actor, Event_Control_Pause, NULL, Actor_OnAction0 );

    Actor_Component_New(actor, Component_Render);
    Actor_Component_Render_ShaderText_Add(actor, &vec3_zero, &vec2_zero, "[]");

    Actor_Component_New(actor, Component_Physics);
    Actor_Component_Physics_Location_Set(actor, &vec3_zero);
}


static void Scene_Create0(void)
{
    strcrc local_name = StrCrc(__FUNCTION__, 0);

    Scene * scene = SceneManager_Scene_Create(&local_name, NULL, NULL, NULL);
    vec2 vec2_scale = Vec2( 2.f, 1.f );
    vec2 vec2_offset = Vec2( 4.f, 2.f );

    Scene_Viewport_Create(scene, 20, 10, &vec2_scale, &vec2_offset);
    SceneManager_Scene_Foreground_Queue_Add(scene);

    Actor * actor = Scene_Actor_Create(&local_name, scene, CallBack_Actor_Create0, NULL, NULL);

    InputManager_Input_ControlEvent_Add(InputManager_GetInstance(), KeyId_Escape, KeyState_Down, Event_Control_Cancel);
    InputManager_Input_ControlEvent_Add(InputManager_GetInstance(), KeyId_Up, KeyState_Down, Event_Control_MoveUp);
    InputManager_Input_ControlEvent_Add(InputManager_GetInstance(), KeyId_Down, KeyState_Down, Event_Control_MoveDown);
    InputManager_Input_ControlEvent_Add(InputManager_GetInstance(), KeyId_Left, KeyState_Down, Event_Control_MoveLeft);
    InputManager_Input_ControlEvent_Add(InputManager_GetInstance(), KeyId_Right, KeyState_Down, Event_Control_MoveRight);
    InputManager_Input_ControlEvent_Add(InputManager_GetInstance(), KeyId_Space, KeyState_Down, Event_Control_Pause);
}

void Engine_Debug_UnitTesting2()
{
    SceneManager_Command_Add(Scene_Create0);
}