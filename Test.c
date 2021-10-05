#include "CoreMini.h"

#include "String.h"
#include "Memory.h"
#include "Queue.h"
#include "Storage.h"
#include "Actor.h"
#include "Component.h"
#include "Render.h"
#include "Sence.h"

#include <stdio.h>


static void Storage_Test0()
{
    Storage* storage = Storage_Create(__FUNCTION__);
    
    Storage_StoreInt32(storage, Str_CalcCrc("a",0), 123);


    printf("%d\n", Storage_IsExistVariable(storage, Str_CalcCrc("a", 0)));
    printf("%d\n", Storage_IsExistVariable(storage, Str_CalcCrc("b", 0)));

    int32 data1 = Storage_ReadInt32(storage, Str_CalcCrc("a",0));
    int32 data2 = Storage_ReadInt32(storage, Str_CalcCrc("b",0));
    printf("%d\n", data1);
    printf("%d\n", data2);


    Storage_DeleteVariable(storage, Str_CalcCrc("a",0));

    Storage_Destroy(storage);
}

////////////////////////////////////////////////////////////////////////////////
typedef struct Data Data;
struct Data
{
    int32 m_a;
};

static void Queue_Test_Print_Data(Data* a, tptr ptr)
{
    printf("%d: %d\n", (int32)ptr, a->m_a);
}

static bool Queue_Test_Find(Data* a, int32 v)
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

    Queue_ForEach(queue, Queue_Test_Print_Data, (tptr)2);

    Queue_RemoveFrist(Data*)(queue, (FindDataFunc)Queue_Test_Find, (tptr)2);

    printf("\n");
    Queue_ForEach(queue, Queue_Test_Print_Data, (tptr)7);

    Queue_RemoveFrist(Data*)(queue, (FindDataFunc)Queue_Test_Find, (tptr)1);

    Queue_Destroy(queue);
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
    printf(String_CStr(string1));
    printf("\n");

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
void Actor_Test0()
{
    Sence* sence = Sence_Create(__FUNCTION__);
    Actor* actor = Sence_Actor_Create(__FUNCTION__, sence);

    Actor_Component_Add(actor, Render);
    Actor_RenderComponent_AddRenderData2D(actor, 0, 10, "hello world" );

    RenderManager_RenderSenceActor(sence);

    RenderManager_RenderToScreen();

    Actor_Component_Del(actor, Render);


    Actor_Destroy(actor);
    Sence_Destroy(sence);
}

////////////////////////////////////////////////////////////////////////////////
void Engine_Test0()
{
    // Engine_Initialize();
    //
    // Engine_MainLoop();
    //
    // Engine_SetSence();

}

////////////////////////////////////////////////////////////////////////////////
void Engine_Debug_UnitTesting()
{
    Engine_Profile_Memory();

    Storage_Test0();

    Queue_Test0();

    Log_Test0();

    Memory_Test0();

    String_Test0();
    String_Test1();
    String_Test2();

    Engine_Profile_Memory();
    Actor_Test0();
    Engine_Profile_Memory();

    Engine_Test0();

    Engine_Profile_Memory();
}