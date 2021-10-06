#include "CoreMini.h"

#include "ActorComponent.h"
#include "LocationComponent.h"
#include "RenderComponent.h"
#include "String.h"
#include "MemoryManager.h"
#include "Queue.h"
#include "Storage.h"
#include "Actor.h"
#include "Component.h"
#include "RenderManager.h"
#include "Sence.h"
#include "Data32.h"
#include "Engine.h"
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

    Queue_Push(Data*, queue, &x);
    Queue_Push(Data*, queue, &y);
    Queue_Push(Data*, queue, &z);

    Queue_ForEach(queue, CallBack_Queue_Test_Print_Data1, (tptr)2);

    Queue_RemoveFindFirst(Data*)(queue, (CB_FindData)CallBack_Queue_Test_Find, (tptr)2);

    Queue_ForEach(queue, Queue_Test_Print_Data2, (tptr)7);

    Queue_RemoveFindFirst(Data*)(queue, (CB_FindData)CallBack_Queue_Test_Find, (tptr)1);

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
    Sence* sence = Sence_Create(__FUNCTION__);
    Actor* actor = Sence_Actor_Create(__FUNCTION__, sence);

    Actor_Component_New(actor, Component_Render);
    Actor_Component_Render_ShaderText_Add(actor, Vec3(0, 10, 0), "hello world" );
    Actor_Component_Render_ShaderText_Add(actor, Vec3(1, 2, 0), "goodbye world" );

    RenderManager_RenderSence(RenderManager_GetInstance(), sence);

    RenderManager_RenderToScreen(RenderManager_GetInstance());

    Actor_Component_Del(actor, Component_Render);

    Sence_Actor_Destroy(sence, NULL, actor);
    Sence_Destroy(sence);
}

void Actor_Test1()
{
    Sence* sence = Sence_Create(__FUNCTION__);
    Actor* actor = Sence_Actor_Create(__FUNCTION__, sence);

    Actor_Component_New(actor, Component_Location);
    Actor_Component_Render_ShaderText_Add(actor, Vec3(2, 2, 0), "hello world" );

    RenderManager_RenderSence(RenderManager_GetInstance(), sence);

    RenderManager_RenderToScreen(RenderManager_GetInstance());

    Actor_Component_Del(actor, Component_Location);

    Sence_Actor_Destroy(sence, NULL, actor);
    Sence_Destroy(sence);
}

void Actor_Test0()
{
    Sence* sence = Sence_Create(__FUNCTION__);
    Actor* actor = Sence_Actor_Create(__FUNCTION__, sence);

    Actor_Component_New(actor, Component_Render);
    Actor_Component_Render_ShaderText_Add(actor, Vec3(0, 10, 0), "hello world" );

    RenderManager_RenderSence(RenderManager_GetInstance(), sence);

    RenderManager_RenderToScreen(RenderManager_GetInstance());

    Actor_Component_Del(actor, Component_Render);

    Sence_Actor_Destroy(sence, NULL, actor);
    Sence_Destroy(sence);
}

////////////////////////////////////////////////////////////////////////////////
void Data32_Test0()
{
    data32 d1 = Data32(int32, 32);
    data32 d2 = Data32(float, 3.2f);
    data32 d3 = Data32(tptr, &d1);
    data32 d4 = Data32(uint32, 123);

    int32 i = Data32_Cast(int32, d1);
    float f = Data32_Cast(float, d2);
    tptr p = Data32_Cast(tptr, d3);
    int32 u = Data32_Cast(uint32, d4);

    Assert(i == 32, "");
    Assert(f == 3.2f, "");
    Assert(p == &d1, "");
    Assert(u == 123, "");
}

////////////////////////////////////////////////////////////////////////////////
void Engine_Test0()
{
    Sence* sence = Sence_Create(__FUNCTION__);
    Actor* actor = Sence_Actor_Create(__FUNCTION__, sence);

    Actor_Component_New(actor, Component_Render);
    Actor_Component_Render_ShaderText_Add(actor, Vec3(0, 10, 0), "hello world" );

    Engine_Sence_SetCurrentSence(sence);

    Engine_MainLoop();


    Actor_Component_Del(actor, Component_Render);

    Sence_Actor_Destroy(sence, NULL, actor);
    Sence_Destroy(sence);
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

    Engine_Test0();

    Engine_Profile_Memory();
}
