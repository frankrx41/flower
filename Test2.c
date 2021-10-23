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

static void Scene_Create1(Task* task, tptr ptr)
{

}

static void Actor_OnAction0(Actor* actor, const EventInfo* event_info)
{
    if( event_info->m_event == Event_Control_Cancel )
    {
        SceneManager_Scene_Destroy(Actor_OwnerScene_Get(actor));
    }
}

static void Actor_Create0(Actor* actor, tptr ptr)
{
    Actor_Component_New(actor, Component_Control);
    Actor_Component_Control_ControlEventRespond_Add(actor, Event_Control_MoveUp, NULL, Actor_OnAction0 );
    Actor_Component_Control_ControlEventRespond_Add(actor, Event_Control_MoveDown, NULL, Actor_OnAction0 );
    Actor_Component_Control_ControlEventRespond_Add(actor, Event_Control_Cancel, NULL, Actor_OnAction0 );
}


static void Scene_Create0(void)
{
    strcrc local_name;
    StrCrc(__FUNCTION__, 0, &local_name);

    Scene * scene = SceneManager_Scene_Create(&local_name, NULL);

    Actor * actor = Scene_Actor_Create(&local_name, scene, Actor_Create0, NULL);
}


static void Scene_Create_Task(Task* task, tptr ptr)
{
    Scene_Create0();
}

void Engine_Debug_UnitTesting2()
{
    // TaskManager_Task_Work_Add(TaskManager_GetInstance(), __FUNCTION__, 0, true, Scene_Create_Task, NULL);
    SceneManager_Command_Add(Scene_Create0);

    InputManager_Input_ControlEvent_Add(InputManager_GetInstance(), KeyId_Escape, KeyState_Down, Event_Control_Cancel);
    InputManager_Input_ControlEvent_Add(InputManager_GetInstance(), KeyId_Up, KeyState_Down, Event_Control_MoveUp);
    InputManager_Input_ControlEvent_Add(InputManager_GetInstance(), KeyId_Down, KeyState_Down, Event_Control_MoveDown);


    // Engine_MainLoop();
}