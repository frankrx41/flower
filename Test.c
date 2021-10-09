#include "CoreMini.h"

#include "Engine.h"

#include "MemoryManager.h"
#include "RenderManager.h"
#include "SenceManager.h"
#include "EventManager.h"

#include "Component.h"

#include "ActorComponent.h"
#include "ActionComponent.h"
#include "LocationComponent.h"
#include "RenderComponent.h"
#include "StorageComponent.h"

#include "String.h"
#include "Queue.h"
#include "Storage.h"
#include "Actor.h"
#include "Sence.h"
#include "Data32.h"
#include "Event.h"
#include "Vec.h"


static void Storage_Test0()
{
    Storage* storage = Storage_Create(__FUNCTION__);
    
    Storage_StoreData32(storage, Str_CalcCrc("a",0), Data32(int32, 123));

    bool is_exist_a = Storage_IsExistVariable(storage, Str_CalcCrc("a", 0));
    bool is_exist_b = Storage_IsExistVariable(storage, Str_CalcCrc("b", 0));

    Assert(is_exist_a == true, "");
    Assert(is_exist_b == false, "");

    int32 data1 = Storage_ReadData32(storage, Str_CalcCrc("a",0)).m_int32;
    int32 data2 = Storage_ReadData32(storage, Str_CalcCrc("b",0)).m_int32;

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

static void CallBack_Queue_Test_Print_Data1(Data* a, ptr32 ptr)
{
    Assert((int32)ptr == 2, "");
    static int32 call_count = 0;
    static int32 array[3] = {1,2,3};
    Assert(a->m_a == array[call_count], "");
    call_count++;
}

static void Queue_Test_Print_Data2(Data* a, ptr32 ptr)
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

    Queue_ForEach(queue, CallBack_Queue_Test_Print_Data1, (ptr32)2);

    Queue_RemoveFindFirst(Data*)(queue, (CB_FindData)CallBack_Queue_Test_Find, (ptr32)2);

    Queue_ForEach(queue, Queue_Test_Print_Data2, (ptr32)7);

    Queue_RemoveFindFirst(Data*)(queue, (CB_FindData)CallBack_Queue_Test_Find, (ptr32)1);

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
static void Memory_Test0()
{
    ptr32 ptr = MemNewSize(__FUNCTION__, 256);
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
    String* a = String_New(__FUNCTION__, NULL);

    String_Copy(a, "say hi\n", 0);

    Log(1, String_CStr(a));

    String_Copy(a, "bye\n", 0);

    Log(1, String_CStr(a));

    String_Del(a);
};

static void String_Test0()
{

    String* string1 = String_New(__FUNCTION__, "hello world");
    String* string2 = String_New(__FUNCTION__, "goodbye world");
    
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
    Sence* sence = SenceManager_Sence_Create(__FUNCTION__);
    Actor* actor = Sence_Actor_Create(__FUNCTION__, sence, NULL, NULL);

    Actor_Component_New(actor, Component_Render);
    Actor_Component_Render_ShaderText_Add(actor, Vec3(0, 10, 0), "hello world" );
    Actor_Component_Render_ShaderText_Add(actor, Vec3(1, 2, 0), "goodbye world" );

    RenderManager_RenderSence(RenderManager_GetInstance(), sence);

    RenderManager_Render_ToScreen(RenderManager_GetInstance());

    Sence_Actor_Destroy(sence, NULL, actor);
    SenceManager_Sence_Destroy(sence);
}

void Actor_Test1()
{
    Sence* sence = SenceManager_Sence_Create(__FUNCTION__);
    Actor* actor = Sence_Actor_Create(__FUNCTION__, sence, NULL, NULL);

    Actor_Component_New(actor, Component_Location);
    Actor_Component_New(actor, Component_Render);
    Actor_Component_Render_ShaderText_Add(actor, Vec3(2, 2, 0), "hello world" );

    RenderManager_RenderSence(RenderManager_GetInstance(), sence);

    RenderManager_Render_ToScreen(RenderManager_GetInstance());

    Actor_Component_Del(actor, Component_Location);

    Sence_Actor_Destroy(sence, NULL, actor);
    SenceManager_Sence_Destroy(sence);
}

void Actor_Test0()
{
    Sence* sence = SenceManager_Sence_Create(__FUNCTION__);
    Actor* actor = Sence_Actor_Create(__FUNCTION__, sence, NULL, NULL);

    Actor_Component_New(actor, Component_Render);
    Actor_Component_Render_ShaderText_Add(actor, Vec3(0, 10, 0), "hello world" );

    RenderManager_RenderSence(RenderManager_GetInstance(), sence);

    RenderManager_Render_ToScreen(RenderManager_GetInstance());

    Actor_Component_Del(actor, Component_Render);

    Sence_Actor_Destroy(sence, NULL, actor);
    SenceManager_Sence_Destroy(sence);
}

////////////////////////////////////////////////////////////////////////////////
void Data32_Test0()
{
    const data32 d1 = Data32(int32, 32);
    const data32 d2 = Data32(float, 3.2f);
    const data32 d3 = Data32(ptr32, &d1);
    const data32 d4 = Data32(uint32, 123);

    const int32 i = d1.m_int32;
    const float f = d2.m_float;
    const ptr32 p = d3.m_ptr32;
    const uint32 u = d4.m_uint32;

    Assert(i == 32, "");
    Assert(f == 3.2f, "");
    Assert(p == &d1, "");
    Assert(u == 123, "");
}

////////////////////////////////////////////////////////////////////////////////
void CallBack_ActorOnEvent3(Actor* actor, const EventInfo* event_info)
{
    crc32 crc_delta_second = Str_CalcCrc("delta_seconds" ,0);
    crc32 crc_tick_count = Str_CalcCrc("tick_count" ,0);
    int32 tick_count = Actor_Component_Storage_ReadData32(actor, crc_tick_count).m_int32;

    static int32 update_count = 0;

    float delta_second = Actor_Component_Storage_ReadData32(actor, crc_delta_second).m_float;
    const float update_need_seconds = 1.f;

    if( delta_second < update_need_seconds )
    {
        delta_second += event_info->m_delta_seconds;
        tick_count++;
    }
    else
    {
        update_count++;
        Actor_Component_Render_ShaderText_ClearAll(actor);

        String* string = String_New(Actor_GetLocalName(actor), NULL);
        String_Format(string, "%d %.2f", tick_count, 1.f/event_info->m_delta_seconds);
        Actor_Component_Render_ShaderText_Add(actor, Vec3(0,0,0), String_CStr(string));
        String_Del(string);

        delta_second -= update_need_seconds;
        tick_count = 0;

        if( update_count > 5 )
        {
            Engine_SetExit(true);
        }
    }

    Actor_Component_Storage_StoreData32(actor, crc_tick_count, Data32(int32, tick_count));
    Actor_Component_Storage_StoreData32(actor, crc_delta_second, Data32(float, delta_second));
}

void CallBack_Actor_Create3(Actor* actor, ptr32 ptr)
{
    Actor_Component_New(actor, Component_Render);
    Actor_Component_New(actor, Component_Action);
    Actor_Component_New(actor, Component_Storage);

    Actor_Component_Render_ShaderText_Add(actor, Vec3(1, 1, 0), ptr );
    Actor_Component_Action_EventRespond_Add(actor, Event_Sence_Tick, CallBack_ActorOnEvent3);

};
void Engine_Test3()
{
    Sence* sence = SenceManager_Sence_Create(__FUNCTION__);
    Actor* actor = Sence_Actor_Create(__FUNCTION__, sence, CallBack_Actor_Create3, __FUNCTION__);

    SenceManager_Sence_SetCurrent(sence);

    Engine_SetExit(false);
    Engine_MainLoop();


    Sence_Actor_Destroy(sence, NULL, actor);
    SenceManager_Sence_Destroy(sence);
}

void CallBack_ActorOnEvent2(Actor* actor, const EventInfo* event_info)
{
    crc32 crc_delta_second = Str_CalcCrc("delta_seconds" ,0);
    float delta_second = Actor_Component_Storage_ReadData32(actor, crc_delta_second).m_float;
    const float update_need_seconds = 0.5f;

    if( delta_second < update_need_seconds )
    {
        delta_second += event_info->m_delta_seconds;
    }
    else
    {
        delta_second -= update_need_seconds;

        Actor_Component_Render_ShaderText_ClearAll(actor);

        crc32 crc_update_tick = Str_CalcCrc("update_tick" ,0);
        int32 update_tick = Actor_Component_Storage_ReadData32(actor, crc_update_tick).m_int32;
        update_tick += 1;

        if( update_tick >= 5 )
        {
            Engine_SetExit(true);
        }

        String* string = String_New(Actor_GetLocalName(actor), NULL);
        String_Format(string, "%d", update_tick);
        Actor_Component_Render_ShaderText_Add(actor, Vec3(0,0,0), String_CStr(string));
        String_Del(string);

        Actor_Component_Storage_StoreData32(actor, crc_update_tick, Data32(int32, update_tick));

    }
    Actor_Component_Storage_StoreData32(actor, crc_delta_second, Data32(float, delta_second));
}

void CallBack_Actor_Create2(Actor* actor, ptr32 ptr)
{
    Actor_Component_New(actor, Component_Render);
    Actor_Component_New(actor, Component_Action);
    Actor_Component_New(actor, Component_Storage);

    Actor_Component_Render_ShaderText_Add(actor, Vec3(1, 1, 0), ptr );
    Actor_Component_Action_EventRespond_Add(actor, Event_Sence_Tick, CallBack_ActorOnEvent2);

};
void Engine_Test2()
{
    Sence* sence = SenceManager_Sence_Create(__FUNCTION__);
    Actor* actor = Sence_Actor_Create(__FUNCTION__, sence, CallBack_Actor_Create2, __FUNCTION__);

    SenceManager_Sence_SetCurrent(sence);

    Engine_SetExit(false);
    Engine_MainLoop();


    Sence_Actor_Destroy(sence, NULL, actor);
    SenceManager_Sence_Destroy(sence);
}

void CallBack_ActorOnEvent1(Actor* actor, const EventInfo* event_struct)
{
    float seconds = Actor_Component_Storage_ReadData32(actor, Str_CalcCrc("seconds", 0)).m_float;
    seconds += event_struct->m_delta_seconds;
    Actor_Component_Storage_StoreData32(actor, Str_CalcCrc("seconds", 0), Data32(float, seconds));

    if( seconds > 2 )
    {
        Sence* sence = Actor_Component_Storage_ReadData32(actor, Str_CalcCrc("sence", 0)).m_ptr32;
        if( sence )
        {
            SenceManager_Sence_SetCurrent(sence);
        }
        else
        {
            Engine_SetExit(true);
        }

    }
}
void Engine_Test1()
{
    Sence* sence1 = SenceManager_Sence_Create(__FUNCTION__);
    Actor* actor1 = Sence_Actor_Create(__FUNCTION__, sence1, NULL, NULL);

    Actor_Component_New(actor1, Component_Render);
    Actor_Component_Render_ShaderText_Add(actor1, Vec3(1, 1, 0), "hello world" );

    Actor_Component_New(actor1, Component_Action);
    Actor_Component_Action_EventRespond_Add(actor1, Event_Sence_Tick, CallBack_ActorOnEvent1);

    Actor_Component_New(actor1, Component_Storage);
    Actor_Component_Storage_StoreData32(actor1, Str_CalcCrc("seconds", 0), Data32(float, 0));

    Sence* sence2 = SenceManager_Sence_Create(__FUNCTION__);
    Actor* actor2 = Sence_Actor_Create(__FUNCTION__, sence2, NULL, NULL);
    Actor_Component_Storage_StoreData32(actor1, Str_CalcCrc("sence", 0), Data32(ptr32, sence2));

    Actor_Component_New(actor2, Component_Render);
    Actor_Component_Render_ShaderText_Add(actor2, Vec3(1, 1, 0), "goodbye world" );

    Actor_Component_New(actor2, Component_Action);
    Actor_Component_Action_EventRespond_Add(actor2, Event_Sence_Tick, CallBack_ActorOnEvent1);

    Actor_Component_New(actor2, Component_Storage);
    Actor_Component_Storage_StoreData32(actor2, Str_CalcCrc("seconds", 0), Data32(float, 0));
    Actor_Component_Storage_StoreData32(actor2, Str_CalcCrc("sence", 0), Data32(ptr32, NULL));



    SenceManager_Sence_SetCurrent(sence1);

    Engine_SetExit(false);
    Engine_MainLoop();


    // Actor_Component_Del(actor1, Component_Render);
    // Actor_Component_Del(actor1, Component_Action);
    // Actor_Component_Del(actor1, Component_Storage);
    // Actor_Component_Del(actor2, Component_Render);

    Sence_Actor_Destroy(sence1, NULL, actor1);
    // Sence_Actor_Destroy(sence2, NULL, actor2);
    SenceManager_Sence_Destroy(sence1);
    SenceManager_Sence_Destroy(sence2);
}

void CallBack_ActorOnEvent0(Actor* actor, const EventInfo* event_struct)
{
    // StorageComponent* storage_component = Actor_Component_Cast(actor, Component_Storage);
    // float x = Actor_Component_Storage_ReadData32(actor, Str_CalcCrc("X", 0)).m_float;
    // float y = Actor_Component_Storage_ReadData32(actor, Str_CalcCrc("Y", 0)).m_float;

    Actor_Component_Location_Move(actor, Vec3(0.1f, 0.05f, 0));

    vec3 vec = Actor_Component_Location_Get(actor);
    if( vec.m_y > 7 )
    {
        Engine_SetExit(true);
    }
}

void Engine_Test0()
{
    Sence* sence = SenceManager_Sence_Create(__FUNCTION__);
    Actor* actor = Sence_Actor_Create(__FUNCTION__, sence, NULL, NULL);

    Actor_Component_New(actor, Component_Render);
    Actor_Component_Render_ShaderText_Add(actor, Vec3(1, 1, 0), "hello world" );

    Actor_Component_New(actor, Component_Action);
    Actor_Component_New(actor, Component_Location);

    Actor_Component_Location_Set(actor, Vec3(0,0,0));
    Actor_Component_Action_EventRespond_Add(actor, Event_Sence_Tick, CallBack_ActorOnEvent0);

    SenceManager_Sence_SetCurrent(sence);

    Engine_SetExit(false);
    Engine_MainLoop();


    Actor_Component_Del(actor, Component_Render);
    Actor_Component_Del(actor, Component_Action);
    Actor_Component_Del(actor, Component_Location);

    Sence_Actor_Destroy(sence, NULL, actor);
    SenceManager_Sence_Destroy(sence);
}

////////////////////////////////////////////////////////////////////////////////
void Engine_Debug_UnitTesting()
{
    Engine_Profile_Memory();

    Data32_Test0();

    Storage_Test0();

    Queue_Test0();

    Log_Test0();

    Memory_Test0();

    String_Test0();
    String_Test1();
    String_Test2();

    Actor_Test2();
    Actor_Test1();
    Actor_Test0();

    Engine_Test3();
    Engine_Test2();
    Engine_Test1();
    Engine_Test0();

    Engine_Profile_Memory();
}
