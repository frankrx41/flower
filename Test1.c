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


////////////////////////////////////////////////////////////////////////////////
void CallBack_ActorOnEvent5(Actor* actor, const EventInfo* event_info)
{
    vec3 vec3_temp;
    switch (event_info->m_event)
    {
    case Event_Control_Cancel: SceneManager_Scene_ExitCurrent();   return;
    case Event_Control_MoveUp: vec3_temp = Vec3(0, -0.1f, 0); Actor_Component_Physics_Location_Move(actor, &vec3_temp); break;
    case Event_Control_MoveDown: vec3_temp = Vec3(0, +0.1f, 0); Actor_Component_Physics_Location_Move(actor, &vec3_temp); break;
    default:
        Assert(false, "");
    }

    vec3 location = *Actor_Component_Physics_Location_Get(actor);
    Actor_Component_Render_ShaderText_Clear(actor);
    String* string = String_New(Actor_LocalName_Str_Get(actor), NULL, 0, false);
    String_Format(string, "(%.2f) %s", location.m_y, event_info->m_event == Event_Control_MoveUp ? "Up" : "Down");

    location = Vec3(2, 2, 0);
    Actor_Component_Render_ShaderText_Add(actor, &location, NULL, String_CStr(string));
    String_Del(string);
}

void CallBack_Actor_Create5(Actor* actor, const void* ptr)
{
    Actor_Component_New(actor, Component_Render);
    Actor_Component_New(actor, Component_Control);
    Actor_Component_New(actor, Component_Physics);

    vec3 vec3_temp;
    vec3_temp = Vec3(2, 2, 0);
    Actor_Component_Render_ShaderText_Add(actor, &vec3_temp, NULL, "Press Esc to exit");
    vec3_temp = Vec3(6, 0, 0);
    Actor_Component_Physics_Location_Set(actor, &vec3_temp);
    Actor_Component_Control_ControlEventRespond_Add(actor, Event_Control_Cancel, NULL, CallBack_ActorOnEvent5);
    Actor_Component_Control_ControlEventRespond_Add(actor, Event_Control_MoveUp, NULL, CallBack_ActorOnEvent5);
    Actor_Component_Control_ControlEventRespond_Add(actor, Event_Control_MoveDown, NULL, CallBack_ActorOnEvent5);
};

void Engine_Test5()
{
    strcrc local_name = StrCrc(__FUNCTION__, 0);

    Scene* scene = SceneManager_Scene_Create(&local_name, NULL, NULL, NULL);
    Actor* actor = Scene_Actor_Create(&local_name, scene, CallBack_Actor_Create5, __FUNCTION__);

    vec2 vec2_temp = Vec2(0, 10);
    Scene_Viewport_Create(scene, 80, 25, NULL, &vec2_temp);

    InputManager_Input_ControlEvent_Add(InputManager_GetInstance(), KeyId_Escape, KeyState_Down, Event_Control_Cancel);
    InputManager_Input_ControlEvent_Add(InputManager_GetInstance(), KeyId_Up, KeyState_Down, Event_Control_MoveUp);
    InputManager_Input_ControlEvent_Add(InputManager_GetInstance(), KeyId_Down, KeyState_Down, Event_Control_MoveDown);


    SceneManager_Scene_Foreground_Queue_Add(scene);
}

////////////////////////////////////////////////////////////////////////////////
void CallBack_ActorOnEvent4(Actor* actor, const EventInfo* event_info)
{
    static int32 update_count = 0;
    update_count++;
    if (update_count > 100)
    {
        SceneManager_Scene_ExitCurrent();
        return;
    }

    Actor_Component_Render_ShaderText_Clear(actor);

    const vec3 displacement = *Actor_Component_Physics_Location_Get(actor);
    String* string = String_New(Actor_LocalName_Str_Get(actor), NULL, 0, false);
    String_Format(string, "(%.2f, %.2f, %.2f) %.2f", displacement.m_x, displacement.m_y, displacement.m_z, 0.f);

    vec3 location = Vec3(0, 0, 0);
    Actor_Component_Render_ShaderText_Add(actor, &location, NULL, String_CStr(string));
    String_Del(string);
}

void CallBack_Actor_Create4(Actor* actor, const void* ptr)
{
    Actor_Component_New(actor, Component_Render);
    Actor_Component_New(actor, Component_Control);
    Actor_Component_New(actor, Component_Physics);

    vec3 vec3_temp;
    vec3_temp = Vec3(1.f, 1.f, 0.f);
    Actor_Component_Physics_Location_Set(actor, &vec3_temp);
    vec3_temp = Vec3(1.f, 0, 2.f);
    Actor_Component_Physics_Velocity_Set(actor, &vec3_temp);

    Actor_Component_Physics_SetEnableSimulate(actor, true);
    Actor_Component_Control_SceneEventRespond_Add(actor, Event_Scene_Tick, NULL, CallBack_ActorOnEvent4);

};
void Engine_Test4()
{
    strcrc local_name = StrCrc(__FUNCTION__, 0);

    Scene* scene = SceneManager_Scene_Create(&local_name, NULL, NULL, NULL);
    Scene_Viewport_Create(scene, 80, 25, NULL, NULL);
    Actor* actor = Scene_Actor_Create(&local_name, scene, CallBack_Actor_Create4, __FUNCTION__);

    SceneManager_Scene_Foreground_Queue_Add(scene);
}

////////////////////////////////////////////////////////////////////////////////
void CallBack_ActorOnEvent3(Actor* actor, const EventInfo* event_info)
{
    crc32 crc_tick_count = Str_CalcCrc("tick_count", 0);
    int32 tick_count = Actor_Component_Storage_Data_Read(actor, crc_tick_count).m_int32;

    static int32 update_count = 0;
    update_count++;
    if (update_count > 2)
    {
        SceneManager_Scene_ExitCurrent();
    }

    Actor_Component_Render_ShaderText_Clear(actor);

    String* string = String_New(Actor_LocalName_Str_Get(actor), NULL, 0, false);
    String_Format(string, "%d %.2f", tick_count, 1.f / event_info->m_delta_seconds);

    vec3 location = Vec3(0, 0, 0);
    Actor_Component_Render_ShaderText_Add(actor, &location, NULL, String_CStr(string));
    String_Del(string);

    tick_count = 0;

    Actor_Component_Storage_Data_Store(actor, crc_tick_count, tData(int32, tick_count));
}

bool CallBack_ActorOnEventCondition3(Actor* actor, const EventInfo* event_info)
{
    crc32 crc_delta_second = Str_CalcCrc("delta_seconds", 0);
    float delta_second = Actor_Component_Storage_Data_Read(actor, crc_delta_second).m_float;
    const float update_need_seconds = 1.f;
    crc32 crc_tick_count = Str_CalcCrc("tick_count", 0);
    int32 tick_count = Actor_Component_Storage_Data_Read(actor, crc_tick_count).m_int32;

    bool need_update = false;

    if (delta_second < update_need_seconds)
    {
        delta_second += event_info->m_delta_seconds;
        tick_count++;
    }
    else
    {
        delta_second -= update_need_seconds;
        need_update = true;
    }

    Actor_Component_Storage_Data_Store(actor, crc_delta_second, tData(float, delta_second));
    Actor_Component_Storage_Data_Store(actor, crc_tick_count, tData(int32, tick_count));

    return need_update;
}

void CallBack_Actor_Create3(Actor* actor, const void* ptr)
{
    Actor_Component_New(actor, Component_Render);
    Actor_Component_New(actor, Component_Control);
    Actor_Component_New(actor, Component_Storage);

    vec3 location = Vec3(1, 1, 0);
    Actor_Component_Render_ShaderText_Add(actor, &location, NULL, ptr);
    Actor_Component_Control_SceneEventRespond_Add(actor, Event_Scene_Tick, CallBack_ActorOnEventCondition3, CallBack_ActorOnEvent3);

};
void Engine_Test3()
{
    strcrc local_name = StrCrc(__FUNCTION__, 0);

    Scene* scene = SceneManager_Scene_Create(&local_name, NULL, NULL, NULL);
    Scene_Viewport_Create(scene, 80, 25, NULL, NULL);
    Actor* actor = Scene_Actor_Create(&local_name, scene, CallBack_Actor_Create3, __FUNCTION__);

    SceneManager_Scene_Foreground_Queue_Add(scene);
}

////////////////////////////////////////////////////////////////////////////////
void CallBack_ActorOnEvent2(Actor* actor, const EventInfo* event_info)
{
    const crc32 crc_delta_second = Str_CalcCrc("delta_seconds", 0);
    float delta_second = Actor_Component_Storage_Data_Read(actor, crc_delta_second).m_float;
    const float update_need_seconds = 0.5f;

    if (delta_second < update_need_seconds)
    {
        delta_second += event_info->m_delta_seconds;
    }
    else
    {
        delta_second -= update_need_seconds;

        Actor_Component_Render_ShaderText_Clear(actor);

        const crc32 crc_update_tick = Str_CalcCrc("update_tick", 0);
        int32 update_tick = Actor_Component_Storage_Data_Read(actor, crc_update_tick).m_int32;
        update_tick += 1;

        if (update_tick >= 2)
        {
            SceneManager_Scene_ExitCurrent();
        }

        String* string = String_New(Actor_LocalName_Str_Get(actor), NULL, 0, false);
        String_Format(string, "%d", update_tick);

        vec3 location = Vec3(0, 0, 0);
        Actor_Component_Render_ShaderText_Add(actor, &location, NULL, String_CStr(string));
        String_Del(string);

        Actor_Component_Storage_Data_Store(actor, crc_update_tick, tData(int32, update_tick));

    }
    Actor_Component_Storage_Data_Store(actor, crc_delta_second, tData(float, delta_second));
}

void CallBack_Actor_Create2(Actor* actor, const void* ptr)
{
    Actor_Component_New(actor, Component_Render);
    Actor_Component_New(actor, Component_Control);
    Actor_Component_New(actor, Component_Storage);

    vec3 location = Vec3(1, 1, 0);
    Actor_Component_Render_ShaderText_Add(actor, &location, NULL, ptr);
    Actor_Component_Control_SceneEventRespond_Add(actor, Event_Scene_Tick, NULL, CallBack_ActorOnEvent2);

};

void Engine_Test2()
{
    strcrc local_name = StrCrc(__FUNCTION__, 0);

    Scene* scene = SceneManager_Scene_Create(&local_name, NULL, NULL, NULL);
    Scene_Viewport_Create(scene, 80, 25, NULL, NULL);
    Actor* actor = Scene_Actor_Create(&local_name, scene, CallBack_Actor_Create2, __FUNCTION__);

    SceneManager_Scene_Foreground_Queue_Add(scene);
}

////////////////////////////////////////////////////////////////////////////////
void CallBack_ActorOnEvent1(Actor* actor, const EventInfo* event_info)
{
    float seconds = Actor_Component_Storage_Data_Read(actor, Str_CalcCrc("seconds", 0)).m_float;
    seconds += event_info->m_delta_seconds;
    Actor_Component_Storage_Data_Store(actor, Str_CalcCrc("seconds", 0), tData(float, seconds));

    if (seconds > 2)
    {
        Scene* scene = Actor_Component_Storage_Data_Read(actor, Str_CalcCrc("scene", 0)).m_tptr;
        if (scene)
        {
            SceneManager_Scene_Foreground_Queue_Add(scene);
            Scene_Pause_All(event_info->m_scene);
        }
        else
        {
            SceneManager_Scene_ExitCurrent();
        }

    }
}

void Engine_Test1()
{
    strcrc local_name = StrCrc(__FUNCTION__, 0);
    vec3 location;

    Scene* scene1 = SceneManager_Scene_Create(&local_name, NULL, NULL, NULL);
    Scene_Viewport_Create(scene1, 80, 25, NULL, NULL);
    Actor* actor1 = Scene_Actor_Create(&local_name, scene1, NULL, NULL);

    Actor_Component_New(actor1, Component_Render);
    location = Vec3(1, 1, 0);
    Actor_Component_Render_ShaderText_Add(actor1, &location, NULL, "hello world");

    Actor_Component_New(actor1, Component_Control);
    Actor_Component_Control_SceneEventRespond_Add(actor1, Event_Scene_Tick, NULL, CallBack_ActorOnEvent1);

    Actor_Component_New(actor1, Component_Storage);
    Actor_Component_Storage_Data_Store(actor1, Str_CalcCrc("seconds", 0), tData(float, 0));

    Scene* scene2 = SceneManager_Scene_Create(&local_name, NULL, NULL, NULL);
    Scene_Viewport_Create(scene2, 80, 25, NULL, NULL);
    Actor* actor2 = Scene_Actor_Create(&local_name, scene2, NULL, NULL);
    Actor_Component_Storage_Data_Store(actor1, Str_CalcCrc("scene", 0), tData(void*, scene2));

    Actor_Component_New(actor2, Component_Render);
    location = Vec3(1, 1, 0);
    Actor_Component_Render_ShaderText_Add(actor2, &location, NULL, "goodbye world");

    Actor_Component_New(actor2, Component_Control);
    Actor_Component_Control_SceneEventRespond_Add(actor2, Event_Scene_Tick, NULL, CallBack_ActorOnEvent1);

    Actor_Component_New(actor2, Component_Storage);
    Actor_Component_Storage_Data_Store(actor2, Str_CalcCrc("seconds", 0), tData(float, 0));
    Actor_Component_Storage_Data_Store(actor2, Str_CalcCrc("scene", 0), tData(void*, NULL));



    SceneManager_Scene_Foreground_Queue_Add(scene1);
}

////////////////////////////////////////////////////////////////////////////////
void CallBack_ActorOnEvent0(Actor* actor, const EventInfo* event_info)
{
    vec3 vec3_temp = Vec3(0.1f, 0.05f, 0);
    Actor_Component_Physics_Location_Move(actor, &vec3_temp);

    vec3 vec = *Actor_Component_Physics_Location_Get(actor);
    if (vec.m_y > 2)
    {
        SceneManager_Scene_ExitCurrent();
    }
}

void Engine_Test0()
{
    strcrc local_name = StrCrc(__FUNCTION__, 0);

    Scene* scene = SceneManager_Scene_Create(&local_name, NULL, NULL, NULL);
    Scene_Viewport_Create(scene, 80, 25, NULL, NULL);
    Actor* actor = Scene_Actor_Create(&local_name, scene, NULL, NULL);

    Actor_Component_New(actor, Component_Render);
    vec3 location = Vec3(1, 1, 0);
    Actor_Component_Render_ShaderText_Add(actor, &location, NULL, "hello world");

    Actor_Component_New(actor, Component_Control);
    Actor_Component_New(actor, Component_Physics);

    vec3 vec3_temp = Vec3(0, 0, 0);
    Actor_Component_Physics_Location_Set(actor, &vec3_temp);
    Actor_Component_Control_SceneEventRespond_Add(actor, Event_Scene_Tick, NULL, CallBack_ActorOnEvent0);

    SceneManager_Scene_Foreground_Queue_Add(scene);

    Memory_Stat();
}

////////////////////////////////////////////////////////////////////////////////
typedef void (*CB_Test_Void)();
static void Test(uint32 index, CB_Test_Void cb_test_void)
{
    Log(0, "Test %d\n", index);

    SceneManager_Command_Add((CB_Command_Void)cb_test_void);
}

void Engine_Debug_UnitTesting1()
{
    CB_Test_Void cb_test_void_array[] =
    {
        // Engine_Test5,
        Engine_Test4,
        Engine_Test3,
        Engine_Test2,
        Engine_Test1,
        Engine_Test0,
    };

    Log(0, "%s Start\n", __FUNCTION__);
    Memory_Stat();

    for (uint32 i = 0, max_i = ARRAY_SIZE(cb_test_void_array); i < max_i; i++)
    {
        Test(i, cb_test_void_array[i]);
    }
}
