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
#include "ActionComponent.h"
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
    if( event_info->m_event == Event_Actor_Action_Cancel )
    {
        SceneManager_Scene_Destroy(Actor_GetScene(actor));
    }
}

static void Actor_Create0(Actor* actor, tptr ptr)
{
    Actor_Component_New(actor, Component_Action);
    Actor_Component_Action_EventRespond_Add(actor, Event_Actor_Action_MoveUp, NULL, Actor_OnAction0 );
    Actor_Component_Action_EventRespond_Add(actor, Event_Actor_Action_MoveDown, NULL, Actor_OnAction0 );
    Actor_Component_Action_EventRespond_Add(actor, Event_Actor_Action_Cancel, NULL, Actor_OnAction0 );
}

static void Scene_Create0(void)
{
    
    Scene * scene = SceneManager_Scene_Create(__FUNCTION__);

    Actor * actor = Scene_Actor_Create(__FUNCTION__, scene, Actor_Create0, NULL);
}


static void Scene_Create_Task(Task* task, tptr ptr)
{
    Scene_Create0();
}

void Engine_Debug_UnitTesting1()
{
    // TaskManager_Task_Add(TaskManager_GetInstance(), __FUNCTION__, 0, true, Scene_Create_Task, NULL);
    SceneManager_Command_Add(Scene_Create0);

    InputManager_InputActionEvent_Add(InputManager_GetInstance(), KeyId_Escape, KeyState_Down, Event_Actor_Action_Cancel);
    InputManager_InputActionEvent_Add(InputManager_GetInstance(), KeyId_Up, KeyState_Down, Event_Actor_Action_MoveUp);
    InputManager_InputActionEvent_Add(InputManager_GetInstance(), KeyId_Down, KeyState_Down, Event_Actor_Action_MoveDown);

    Engine_MainLoop();
}