#include "CoreMini.h"

#include "Engine.h"

#include "Event.h"
#include "KeyId.h"

#include "MemoryManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "EventManager.h"
#include "InputManager.h"

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


static void Storage_Test0()
{
    Storage* storage = Storage_Create(__FUNCTION__);

    Storage_StoreData(storage, Str_CalcCrc("a",0), tData(int32, 123));

    const bool is_exist_a = Storage_IsExistVariable(storage, Str_CalcCrc("a", 0));
    const bool is_exist_b = Storage_IsExistVariable(storage, Str_CalcCrc("b", 0));

    Assert(is_exist_a == true, "");
    Assert(is_exist_b == false, "");

    const int32 data1 = Storage_ReadData(storage, Str_CalcCrc("a",0)).m_int32;
    const int32 data2 = Storage_ReadData(storage, Str_CalcCrc("b",0)).m_int32;

    Assert(data1 == 123, "");
    Assert(data2 == 0, "");

    Storage_DeleteVariable(storage, Str_CalcCrc("a",0));

    Storage_Destroy(storage);
}

////////////////////////////////////////////////////////////////////////////////
typedef struct Data Data;
struct Data
{
    int32 m_a;
};

static void CallBack_Queue_Test_Print_Data1(Data* a, tptr ptr)
{
    Assert((int32)ptr == 2, "");
    static int32 call_count = 0;
    static int32 array[3] = {1,2,3};
    Assert(a->m_a == array[call_count], "");
    call_count++;
}

static void Queue_Test_Print_Data2(Data* a, tptr ptr)
{
    Assert((int32)ptr == 7, "");
    static int32 call_count = 0;
    static int32 array[3] = {1,3};
    Assert(a->m_a == array[call_count], "");
    call_count++;
}

static bool CallBack_Queue_Test_Find(Data* a, int32 v)
{
    return a->m_a == v;
}

static void Queue_Test0()
{
    Queue(Data*)* queue = Queue_Create(__FUNCTION__, Data*);

    Data x = {1};
    Data y = {2};
    Data z = {3};

    Queue_Push(Data*, __FUNCTION__, queue, &x);
    Queue_Push(Data*, __FUNCTION__, queue, &y);
    Queue_Push(Data*, __FUNCTION__, queue, &z);

    Queue_ForEach(queue, CallBack_Queue_Test_Print_Data1, (tptr)2);

    Queue_RemoveFindFirst(Data*)(queue, (CB_FindData_Bool_tPtr_tPtr)CallBack_Queue_Test_Find, (tptr)2);

    Queue_ForEach(queue, Queue_Test_Print_Data2, (tptr)7);

    Queue_RemoveFindFirst(Data*)(queue, (CB_FindData_Bool_tPtr_tPtr)CallBack_Queue_Test_Find, (tptr)1);

    Queue_Destroy(queue, NULL);
}

////////////////////////////////////////////////////////////////////////////////
static void Log_Test0()
{
    Log(0, "info %s", "hello");
    Log(1, "warn %s", "hello");
    Log(2, "error %s", "hello");
};

////////////////////////////////////////////////////////////////////////////////
static void Memory_Test1()
{
    tchar* ptr = MemNewSize(__FUNCTION__, 256);
    tptr ptr2 = ptr + 1;
    tptr ptr3 = ptr - 1;
    tptr ptr4 = ptr + 255;
    tptr ptr5 = ptr + 256;
    tptr ptr6 = ptr + 111;
    Assert(Memory_IsInBounds(ptr, ptr2), "");
    Assert(Memory_IsInBounds(ptr, ptr3) == false, "");
    Assert(Memory_IsInBounds(ptr, ptr4), "");
    Assert(Memory_IsInBounds(ptr, ptr5) == false, "");
    Assert(Memory_IsInBounds(ptr, ptr6), "");
    MemDel(ptr);
};

static void Memory_Test0()
{
    tptr ptr = MemNewSize(__FUNCTION__, 256);
    MemDel(ptr);
};

////////////////////////////////////////////////////////////////////////////////
static void String_Test2()
{
    {
        bool is_empty = Str_IsEmpty(NULL);
        Assert(is_empty == true, "");
    }

    {
        bool is_empty = Str_IsEmpty("");
        Assert(is_empty == true, "");
    }

    {
        bool is_empty = Str_IsEmpty("haha");
        Assert(is_empty == false, "");
    }
}

static void String_Test1()
{
    String* a = String_New(__FUNCTION__, NULL, false);

    String_Copy(a, "say hi\n", 0);

    Log(1, String_CStr(a));

    String_Copy(a, "bye\n", 0);

    Log(1, String_CStr(a));

    String_Del(a);
};

static void String_Test0()
{

    String* string1 = String_New(__FUNCTION__, "hello world", true);
    String* string2 = String_New(__FUNCTION__, "goodbye world", true);
    
    Assert(Str_IsSame(String_CStr(string1), "hello world"), "");
    Assert(!Str_IsSame(String_CStr(string1), "goodbye world"), "");

    crc32 str1_crc = Str_CalcCrc("hello world", 0);
    crc32 str2_crc = Str_CalcCrc("goodbye world", 0);

    Assert(str1_crc == String_GetCrc(string1), "");
    Assert(str2_crc == String_GetCrc(string2), "");
    Assert(str1_crc != String_GetCrc(string2), "");
    Assert(str2_crc != String_GetCrc(string1), "");

    String_Del(string1);
    String_Del(string2);


    for( int32 i=0; i<10; i++)
    {
        Assert(str1_crc == Str_CalcCrc("hello world", 0), "");
        Assert(str2_crc == Str_CalcCrc("goodbye world", 0), "");
    }
}


////////////////////////////////////////////////////////////////////////////////
void Actor_Test2()
{
    Scene* scene = SceneManager_Scene_Create(__FUNCTION__);
    Actor* actor = Scene_Actor_Create(__FUNCTION__, scene, NULL, NULL);

    Actor_Component_New(actor, Component_Render);
    Actor_Component_Render_ShaderText_Add(actor, Vec3(0, 10, 0), "hello world" );
    Actor_Component_Render_ShaderText_Add(actor, Vec3(1, 2, 0), "goodbye world" );

    RenderManager_RenderAllScene(RenderManager_GetInstance(), SceneManager_GetInstance());

    RenderManager_Render_ToScreen(RenderManager_GetInstance());

    Scene_Actor_Destroy(scene, NULL, actor);
    SceneManager_Scene_Destroy(scene);
}

void Actor_Test1()
{
    Scene* scene = SceneManager_Scene_Create(__FUNCTION__);
    Actor* actor = Scene_Actor_Create(__FUNCTION__, scene, NULL, NULL);

    Actor_Component_New(actor, Component_Physics);
    Actor_Component_New(actor, Component_Render);
    Actor_Component_Render_ShaderText_Add(actor, Vec3(2, 2, 0), "hello world" );

    RenderManager_RenderAllScene(RenderManager_GetInstance(), SceneManager_GetInstance());

    RenderManager_Render_ToScreen(RenderManager_GetInstance());

    Actor_Component_Del(actor, Component_Physics);

    Scene_Actor_Destroy(scene, NULL, actor);
    SceneManager_Scene_Destroy(scene);
}

void Actor_Test0()
{
    Scene* scene = SceneManager_Scene_Create(__FUNCTION__);
    Actor* actor = Scene_Actor_Create(__FUNCTION__, scene, NULL, NULL);

    Actor_Component_New(actor, Component_Render);
    Actor_Component_Render_ShaderText_Add(actor, Vec3(0, 10, 0), "hello world" );

    RenderManager_RenderAllScene(RenderManager_GetInstance(), SceneManager_GetInstance());

    RenderManager_Render_ToScreen(RenderManager_GetInstance());

    Actor_Component_Del(actor, Component_Render);

    Scene_Actor_Destroy(scene, NULL, actor);
    SceneManager_Scene_Destroy(scene);
}

////////////////////////////////////////////////////////////////////////////////
void tData_Test0()
{
    const tdata d1 = tData(int32, 32);
    const tdata d2 = tData(float, 3.2f);
    const tdata d3 = tData(tptr, &d1);
    const tdata d4 = tData(uint32, 123);

    const int32 i = d1.m_int32;
    const float f = d2.m_float;
    const tptr p = d3.m_tptr;
    const uint32 u = d4.m_uint32;

    Assert(i == 32, "");
    Assert(f == 3.2f, "");
    Assert(p == &d1, "");
    Assert(u == 123, "");
}

////////////////////////////////////////////////////////////////////////////////
static bool is_scene_destroy_callback = false;
void CallBack_SceneDesrtoy(Scene* scene)
{
    is_scene_destroy_callback = true;
}

void Scene_Test0()
{
    Scene* scene = SceneManager_Scene_Create(__FUNCTION__);
    Scene_Destroy_CB_Set(scene, CallBack_SceneDesrtoy);

    SceneManager_Scene_Destroy(scene);
    Assert(is_scene_destroy_callback == true, "");
}

////////////////////////////////////////////////////////////////////////////////
void CallBack_ActorOnEvent5(Actor* actor, const EventInfo* event_info)
{
    switch(event_info->m_event)
    {
    case Event_Actor_Action_Cancel: Engine_SetExit(true);   break;
    case Event_Actor_Action_MoveUp: Actor_Component_Physics_MoveLocation(actor, Vec3(0, -0.1f, 0)); break;
    case Event_Actor_Action_MoveDown: Actor_Component_Physics_MoveLocation(actor, Vec3(0, +0.1f, 0)); break;
    default:
        Assert(false, "");
    }

    vec3 vec = Actor_Component_Physics_GetLocation(actor);
    Actor_Component_Render_ShaderText_ClearAll(actor);
    String* string = String_New(Actor_GetLocalName(actor), NULL, false);
    String_Format(string, "(%.2f) %s", vec.m_y, event_info->m_event == Event_Actor_Action_MoveUp ? "Up" : "Down");
    Actor_Component_Render_ShaderText_Add(actor, Vec3(2,2,0), String_CStr(string));
    String_Del(string);
}

void CallBack_Actor_Create5(Actor* actor, tptr ptr)
{
    Actor_Component_New(actor, Component_Render);
    Actor_Component_New(actor, Component_Action);
    Actor_Component_New(actor, Component_Physics);

    Actor_Component_Render_ShaderText_Add(actor, Vec3(2,2,0), "Press Esc to exit");
    Actor_Component_Physics_SetLocation(actor, Vec3(6, 0, 0));
    Actor_Component_Action_EventRespond_Add(actor, Event_Actor_Action_Cancel, NULL, CallBack_ActorOnEvent5);
    Actor_Component_Action_EventRespond_Add(actor, Event_Actor_Action_MoveUp, NULL, CallBack_ActorOnEvent5);
    Actor_Component_Action_EventRespond_Add(actor, Event_Actor_Action_MoveDown, NULL, CallBack_ActorOnEvent5);
};
void Engine_Test5()
{
    Scene* scene = SceneManager_Scene_Create(__FUNCTION__);
    Actor* actor = Scene_Actor_Create(__FUNCTION__, scene, CallBack_Actor_Create5, __FUNCTION__);

    InputManager_InputActionEvent_Add(InputManager_GetInstance(), KeyId_Escape, KeyState_Down, Event_Actor_Action_Cancel);
    InputManager_InputActionEvent_Add(InputManager_GetInstance(), KeyId_Up, KeyState_Down, Event_Actor_Action_MoveUp);
    InputManager_InputActionEvent_Add(InputManager_GetInstance(), KeyId_Down, KeyState_Down, Event_Actor_Action_MoveDown);

    Scene_Render_Offset_Set(scene, Vec2(0, 10));

    SceneManager_Scene_SetCurrent(scene);

    Engine_SetExit(false);
    Engine_MainLoop();


    Scene_Actor_Destroy(scene, NULL, actor);
    SceneManager_Scene_Destroy(scene);
}

void CallBack_ActorOnEvent4(Actor* actor, const EventInfo* event_info)
{
    static int32 update_count = 0;
    update_count++;
    if( update_count > 300 )
    {
        Engine_SetExit(true);
    }

    Actor_Component_Render_ShaderText_ClearAll(actor);

    vec3 displacement = Actor_Component_Physics_GetLocation(actor);
    String* string = String_New(Actor_GetLocalName(actor), NULL, false);
    String_Format(string, "(%.2f, %.2f, %.2f) %.2f", displacement.m_x, displacement.m_y, displacement.m_z, 0.f);
    Actor_Component_Render_ShaderText_Add(actor, Vec3(0,0,0), String_CStr(string));
    String_Del(string);
}

void CallBack_Actor_Create4(Actor* actor, tptr ptr)
{
    Actor_Component_New(actor, Component_Render);
    Actor_Component_New(actor, Component_Action);
    Actor_Component_New(actor, Component_Physics);

    Actor_Component_Physics_SetLocation(actor, Vec3(1.f, 1.f, 0.f));
    Actor_Component_Physics_SetVelocity(actor, Vec3(1.f, 0, 2.f));

    Actor_Component_Physics_SetEnableSimulate(actor, true);
    Actor_Component_Action_EventRespond_Add(actor, Event_Scene_Tick, NULL, CallBack_ActorOnEvent4);

};
void Engine_Test4()
{
    Scene* scene = SceneManager_Scene_Create(__FUNCTION__);
    Actor* actor = Scene_Actor_Create(__FUNCTION__, scene, CallBack_Actor_Create4, __FUNCTION__);

    SceneManager_Scene_SetCurrent(scene);

    Engine_SetExit(false);
    Engine_MainLoop();


    Scene_Actor_Destroy(scene, NULL, actor);
    SceneManager_Scene_Destroy(scene);
}

void CallBack_ActorOnEvent3(Actor* actor, const EventInfo* event_info)
{
    crc32 crc_tick_count = Str_CalcCrc("tick_count" ,0);
    int32 tick_count = Actor_Component_Storage_ReadData(actor, crc_tick_count).m_int32;

    static int32 update_count = 0;
    update_count++;
    if( update_count > 5 )
    {
        Engine_SetExit(true);
    }

    Actor_Component_Render_ShaderText_ClearAll(actor);

    String* string = String_New(Actor_GetLocalName(actor), NULL, false);
    String_Format(string, "%d %.2f", tick_count, 1.f/event_info->m_delta_seconds);
    Actor_Component_Render_ShaderText_Add(actor, Vec3(0,0,0), String_CStr(string));
    String_Del(string);

    tick_count = 0;

    Actor_Component_Storage_StoreData(actor, crc_tick_count, tData(int32, tick_count));
}

bool CallBack_ActorOnEventCondition3(Actor* actor, const EventInfo* event_info)
{
    crc32 crc_delta_second = Str_CalcCrc("delta_seconds" ,0);
    float delta_second = Actor_Component_Storage_ReadData(actor, crc_delta_second).m_float;
    const float update_need_seconds = 1.f;
    crc32 crc_tick_count = Str_CalcCrc("tick_count" ,0);
    int32 tick_count = Actor_Component_Storage_ReadData(actor, crc_tick_count).m_int32;

    bool need_update = false;

    if( delta_second < update_need_seconds )
    {
        delta_second += event_info->m_delta_seconds;
        tick_count++;
    }
    else
    {
        delta_second -= update_need_seconds;
        need_update = true;
    }

    Actor_Component_Storage_StoreData(actor, crc_delta_second, tData(float, delta_second));
    Actor_Component_Storage_StoreData(actor, crc_tick_count, tData(int32, tick_count));

    return need_update;
}

void CallBack_Actor_Create3(Actor* actor, tptr ptr)
{
    Actor_Component_New(actor, Component_Render);
    Actor_Component_New(actor, Component_Action);
    Actor_Component_New(actor, Component_Storage);

    Actor_Component_Render_ShaderText_Add(actor, Vec3(1, 1, 0), ptr );
    Actor_Component_Action_EventRespond_Add(actor, Event_Scene_Tick, CallBack_ActorOnEventCondition3, CallBack_ActorOnEvent3);

};
void Engine_Test3()
{
    Scene* scene = SceneManager_Scene_Create(__FUNCTION__);
    Actor* actor = Scene_Actor_Create(__FUNCTION__, scene, CallBack_Actor_Create3, __FUNCTION__);

    SceneManager_Scene_SetCurrent(scene);

    Engine_SetExit(false);
    Engine_MainLoop();


    Scene_Actor_Destroy(scene, NULL, actor);
    SceneManager_Scene_Destroy(scene);
}

void CallBack_ActorOnEvent2(Actor* actor, const EventInfo* event_info)
{
    const crc32 crc_delta_second = Str_CalcCrc("delta_seconds" ,0);
    float delta_second = Actor_Component_Storage_ReadData(actor, crc_delta_second).m_float;
    const float update_need_seconds = 0.5f;

    if( delta_second < update_need_seconds )
    {
        delta_second += event_info->m_delta_seconds;
    }
    else
    {
        delta_second -= update_need_seconds;

        Actor_Component_Render_ShaderText_ClearAll(actor);

        const crc32 crc_update_tick = Str_CalcCrc("update_tick" ,0);
        int32 update_tick = Actor_Component_Storage_ReadData(actor, crc_update_tick).m_int32;
        update_tick += 1;

        if( update_tick >= 5 )
        {
            Engine_SetExit(true);
        }

        String* string = String_New(Actor_GetLocalName(actor), NULL, false);
        String_Format(string, "%d", update_tick);
        Actor_Component_Render_ShaderText_Add(actor, Vec3(0,0,0), String_CStr(string));
        String_Del(string);

        Actor_Component_Storage_StoreData(actor, crc_update_tick, tData(int32, update_tick));

    }
    Actor_Component_Storage_StoreData(actor, crc_delta_second, tData(float, delta_second));
}

void CallBack_Actor_Create2(Actor* actor, tptr ptr)
{
    Actor_Component_New(actor, Component_Render);
    Actor_Component_New(actor, Component_Action);
    Actor_Component_New(actor, Component_Storage);

    Actor_Component_Render_ShaderText_Add(actor, Vec3(1, 1, 0), ptr );
    Actor_Component_Action_EventRespond_Add(actor, Event_Scene_Tick, NULL, CallBack_ActorOnEvent2);

};
void Engine_Test2()
{
    Scene* scene = SceneManager_Scene_Create(__FUNCTION__);
    Actor* actor = Scene_Actor_Create(__FUNCTION__, scene, CallBack_Actor_Create2, __FUNCTION__);

    SceneManager_Scene_SetCurrent(scene);

    Engine_SetExit(false);
    Engine_MainLoop();


    Scene_Actor_Destroy(scene, NULL, actor);
    SceneManager_Scene_Destroy(scene);
}

void CallBack_ActorOnEvent1(Actor* actor, const EventInfo* event_info)
{
    float seconds = Actor_Component_Storage_ReadData(actor, Str_CalcCrc("seconds", 0)).m_float;
    seconds += event_info->m_delta_seconds;
    Actor_Component_Storage_StoreData(actor, Str_CalcCrc("seconds", 0), tData(float, seconds));

    if( seconds > 2 )
    {
        Scene* scene = Actor_Component_Storage_ReadData(actor, Str_CalcCrc("scene", 0)).m_tptr;
        if( scene )
        {
            SceneManager_Scene_SetCurrent(scene);
            Scene_SetIsPause(event_info->m_scene, true);
        }
        else
        {
            Engine_SetExit(true);
        }

    }
}
void Engine_Test1()
{
    Scene* scene1 = SceneManager_Scene_Create(__FUNCTION__);
    Actor* actor1 = Scene_Actor_Create(__FUNCTION__, scene1, NULL, NULL);

    Actor_Component_New(actor1, Component_Render);
    Actor_Component_Render_ShaderText_Add(actor1, Vec3(1, 1, 0), "hello world" );

    Actor_Component_New(actor1, Component_Action);
    Actor_Component_Action_EventRespond_Add(actor1, Event_Scene_Tick, NULL, CallBack_ActorOnEvent1);

    Actor_Component_New(actor1, Component_Storage);
    Actor_Component_Storage_StoreData(actor1, Str_CalcCrc("seconds", 0), tData(float, 0));

    Scene* scene2 = SceneManager_Scene_Create(__FUNCTION__);
    Actor* actor2 = Scene_Actor_Create(__FUNCTION__, scene2, NULL, NULL);
    Actor_Component_Storage_StoreData(actor1, Str_CalcCrc("scene", 0), tData(tptr, scene2));

    Actor_Component_New(actor2, Component_Render);
    Actor_Component_Render_ShaderText_Add(actor2, Vec3(1, 1, 0), "goodbye world" );

    Actor_Component_New(actor2, Component_Action);
    Actor_Component_Action_EventRespond_Add(actor2, Event_Scene_Tick, NULL, CallBack_ActorOnEvent1);

    Actor_Component_New(actor2, Component_Storage);
    Actor_Component_Storage_StoreData(actor2, Str_CalcCrc("seconds", 0), tData(float, 0));
    Actor_Component_Storage_StoreData(actor2, Str_CalcCrc("scene", 0), tData(tptr, NULL));



    SceneManager_Scene_SetCurrent(scene1);

    Engine_SetExit(false);
    Engine_MainLoop();

    Scene_Actor_Destroy(scene1, NULL, actor1);
    SceneManager_Scene_Destroy(scene1);
    SceneManager_Scene_Destroy(scene2);
}

void CallBack_ActorOnEvent0(Actor* actor, const EventInfo* event_info)
{
    Actor_Component_Physics_MoveLocation(actor, Vec3(0.1f, 0.05f, 0));

    vec3 vec = Actor_Component_Physics_GetLocation(actor);
    if( vec.m_y > 7 )
    {
        Engine_SetExit(true);
    }
}

void Engine_Test0()
{
    Scene* scene = SceneManager_Scene_Create(__FUNCTION__);
    Actor* actor = Scene_Actor_Create(__FUNCTION__, scene, NULL, NULL);

    Actor_Component_New(actor, Component_Render);
    Actor_Component_Render_ShaderText_Add(actor, Vec3(1, 1, 0), "hello world" );

    Actor_Component_New(actor, Component_Action);
    Actor_Component_New(actor, Component_Physics);

    Actor_Component_Physics_SetLocation(actor, Vec3(0,0,0));
    Actor_Component_Action_EventRespond_Add(actor, Event_Scene_Tick, NULL, CallBack_ActorOnEvent0);

    SceneManager_Scene_SetCurrent(scene);

    Engine_SetExit(false);
    Engine_MainLoop();


    Actor_Component_Del(actor, Component_Render);
    Actor_Component_Del(actor, Component_Action);
    Actor_Component_Del(actor, Component_Physics);

    Scene_Actor_Destroy(scene, NULL, actor);
    SceneManager_Scene_Destroy(scene);
}

////////////////////////////////////////////////////////////////////////////////
typedef void (*CB_Test_Void)();
void Test(uint32 index, CB_Test_Void cb_test_void)
{
    Log(0, "Test %d\n", index);
    cb_test_void();
    Engine_Profile_Memory();
}

void Engine_Debug_UnitTesting()
{
    CB_Test_Void cb_test_void_array[] = 
    {
        tData_Test0,
        Storage_Test0,
        Queue_Test0,
        Log_Test0,
        Memory_Test1,
        Memory_Test0,

        String_Test0,
        String_Test1,
        String_Test2,

        Actor_Test2,
        Actor_Test1,
        Actor_Test0,

        Scene_Test0,

        Engine_Test5,
        Engine_Test4,
        Engine_Test3,
        Engine_Test2,
        Engine_Test1,
        Engine_Test0,
    };

    Log(0, "Test Start\n");
    Engine_Profile_Memory();

    for(uint32 i=0, max_i = ARRAY_SIZE(cb_test_void_array); i<max_i; i++)
    {
        Test(i, cb_test_void_array[i]);
    }

}
