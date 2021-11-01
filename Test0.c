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
#include "Mutex.h"
#include "Scene.h"
#include "tData.h"
#include "Argument.h"
#include "ShaderText.h"
#include "Vec.h"
#include "Stat.h"


static void Storage_Test0()
{
    strcrc local_name = StrCrc(__FUNCTION__, 0);

    Storage* storage = Storage_Create(&local_name);

    Storage_Data_Store(storage, Str_CalcCrc("a",0), tData(int32, 123));

    const bool is_exist_a = Storage_Is_ExistVariable(storage, Str_CalcCrc("a", 0));
    const bool is_exist_b = Storage_Is_ExistVariable(storage, Str_CalcCrc("b", 0));

    Assert(is_exist_a == true, "");
    Assert(is_exist_b == false, "");

    const int32 data1 = Storage_Data_Read(storage, Str_CalcCrc("a",0)).m_int32;
    const int32 data2 = Storage_Data_Read(storage, Str_CalcCrc("b",0)).m_int32;

    Assert(data1 == 123, "");
    Assert(data2 == 0, "");

    Storage_Variable_Delete(storage, Str_CalcCrc("a",0));

    Storage_Destroy(storage);
}

////////////////////////////////////////////////////////////////////////////////
typedef struct Data Data;
struct Data
{
    int32 m_a;
};

static void CallBack_Queue_Test_Print_Data1(Data* a, const void* ptr)
{
    Assert((int32)ptr == 2, "");
    static int32 call_count = 0;
    static int32 array[3] = {1,2,3};
    Assert(a->m_a == array[call_count], "");
    call_count++;
}

static void Queue_Test_Print_Data2(Data* a, const void* ptr)
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
    strcrc local_name = StrCrc(__FUNCTION__, 0);

    Queue(Data*)* queue = Queue_Create(&local_name, Data*);

    Data x = {1};
    Data y = {2};
    Data z = {3};

    Queue_Push(Data*, &local_name, queue, &x);
    Queue_Push(Data*, &local_name, queue, &y);
    Queue_Push(Data*, &local_name, queue, &z);

    Queue_ForEach(queue, CallBack_Queue_Test_Print_Data1, (void*)2);

    Queue_RemoveFindFirst(Data*)(queue, (CB_FindData_Bool_tPtr_tPtr)CallBack_Queue_Test_Find, (void*)2, NULL);

    Queue_ForEach(queue, Queue_Test_Print_Data2, (void*)7);

    Queue_RemoveFindFirst(Data*)(queue, (CB_FindData_Bool_tPtr_tPtr)CallBack_Queue_Test_Find, (void*)1, NULL);

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
    strcrc local_name = StrCrc(__FUNCTION__, 0);

    tchar* ptr = MemNewSize(&local_name, 256);
    void* ptr2 = ptr + 1;
    void* ptr3 = ptr - 1;
    void* ptr4 = ptr + 255;
    void* ptr5 = ptr + 256;
    void* ptr6 = ptr + 111;
    Assert(Memory_IsInBounds(ptr, ptr2), "");
    Assert(Memory_IsInBounds(ptr, ptr3) == false, "");
    Assert(Memory_IsInBounds(ptr, ptr4), "");
    Assert(Memory_IsInBounds(ptr, ptr5) == false, "");
    Assert(Memory_IsInBounds(ptr, ptr6), "");
    MemDel(ptr);
};

static void Memory_Test0()
{
    strcrc local_name = StrCrc(__FUNCTION__, 0);

    void* ptr = MemNewSize(&local_name, 256);
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
    strcrc local_name = StrCrc(__FUNCTION__, 0);

    String* a = String_New(&local_name, NULL, 0, false);

    String_Copy(a, "say hi\n", 0);

    Log(1, String_CStr(a));

    String_Copy(a, "bye\n", 0);

    Log(1, String_CStr(a));

    String_Del(a);
};

static void String_Test0()
{
    strcrc local_name = StrCrc(__FUNCTION__, 0);

    String* string1 = String_New(&local_name, "hello world", 0, true);
    String* string2 = String_New(&local_name, "goodbye world", 0, true);
    
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
    strcrc local_name = StrCrc(__FUNCTION__, 0);
    vec3 location;
    ShaderText* shader_text;
    Scene* scene = SceneManager_Scene_Create(&local_name, NULL, NULL, NULL);
    Actor* actor = Scene_Actor_Create(&local_name, scene, NULL, NULL, NULL);

    Scene_Viewport_Create(scene, 0, 0, NULL, NULL);

    Actor_Component_New(actor, Component_Render);
    Component* render_component = Actor_Component_Cast(actor, Component_Render);
    {
        location = Vec3(1, 1, 0);
        shader_text = ShaderText_Create(Actor_GetLocalName(actor), false, &location, NULL, "* hello world");
        Component_Render_ShaderText_Add(render_component, shader_text);
    }
    {
        location = Vec3(1, 2, 0);
        shader_text = ShaderText_Create(Actor_GetLocalName(actor), false, &location, NULL, "* hello world");
        Component_Render_ShaderText_Add(render_component, shader_text);
    }

    RenderManager_RenderAllScene(RenderManager_GetInstance(), SceneManager_GetInstance());

    RenderManager_Render_ToScreen(RenderManager_GetInstance());

    Scene_Actor_Destroy(scene, NULL, actor);
    SceneManager_Scene_Destroy(scene);
}

void Actor_Test1()
{
    strcrc local_name = StrCrc(__FUNCTION__, 0);

    Scene* scene = SceneManager_Scene_Create(&local_name, NULL, NULL, NULL);
    Actor* actor = Scene_Actor_Create(&local_name, scene, NULL, NULL, NULL);
    Scene_Viewport_Create(scene, 0, 0, NULL, NULL);

    Actor_Component_New(actor, Component_Physics);
    Actor_Component_New(actor, Component_Render);

    vec3 location = Vec3(2, 2, 0);
    ShaderText* shader_text = ShaderText_Create(Actor_GetLocalName(actor), false, & location, NULL, "hello world");
    Component_Render_ShaderText_Add(Actor_Component_Cast(actor, Component_Render), shader_text);

    RenderManager_RenderAllScene(RenderManager_GetInstance(), SceneManager_GetInstance());

    RenderManager_Render_ToScreen(RenderManager_GetInstance());

    Actor_Component_Del(actor, Component_Physics);

    Scene_Actor_Destroy(scene, NULL, actor);
    SceneManager_Scene_Destroy(scene);
}

void Actor_Test0()
{
    strcrc local_name = StrCrc(__FUNCTION__, 0);

    Scene* scene = SceneManager_Scene_Create(&local_name, NULL, NULL, NULL);
    Actor* actor = Scene_Actor_Create(&local_name, scene, NULL, NULL, NULL);
    Scene_Viewport_Create(scene, 0, 0, NULL, NULL);

    Actor_Component_New(actor, Component_Render);
    vec3 location = Vec3(0, 10, 0);
    ShaderText* shader_text = ShaderText_Create(Actor_GetLocalName(actor), false, &location, NULL, "hello world");
    Component_Render_ShaderText_Add(Actor_Component_Cast(actor, Component_Render), shader_text);

    RenderManager_RenderAllScene(RenderManager_GetInstance(), SceneManager_GetInstance());

    RenderManager_Render_ToScreen(RenderManager_GetInstance());

    Actor_Component_Del(actor, Component_Render);

    Scene_Actor_Destroy(scene, NULL, actor);
    SceneManager_Scene_Destroy(scene);
}

////////////////////////////////////////////////////////////////////////////////
void CallBack_TaskRun0(Task* task, void* ptr)
{
    volatile int32 foo = 1;
}

void Task_Test0()
{
    strcrc local_name = StrCrc(__FUNCTION__, 0);

    Task* task = TaskManager_Task_Work_Add(&local_name,0, 4, false, NULL, CallBack_TaskRun0, NULL, NULL);

    while(!Task_IsFinish(task))
    {
        ;
    }
    Task_Destroy(task);
}

////////////////////////////////////////////////////////////////////////////////
void tData_Test0()
{
    const tdata d1 = tData(int32, 32);
    const tdata d2 = tData(float, 3.2f);
    const tdata d3 = tData(void*, &d1);
    const tdata d4 = tData(uint32, 123);

    const int32 i = d1.m_int32;
    const float f = d2.m_float;
    const void* p = d3.m_tptr;
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
    strcrc local_name = StrCrc(__FUNCTION__, 0);

    Scene* scene = SceneManager_Scene_Create(&local_name, NULL, CallBack_SceneDesrtoy, NULL);

    SceneManager_Scene_Destroy(scene);
    Assert(is_scene_destroy_callback == true, "");
}

////////////////////////////////////////////////////////////////////////////////
void Mutex_Test0()
{
    strcrc local_name = StrCrc(__FUNCTION__, 0);

    bool success;
    Mutex* mutex1 = Mutex_Create(&local_name, 1);
    Mutex* mutex2 = Mutex_Create(&local_name, 2);
    // Mutex* mutex0 = Mutex_Create(__FUNCTION__, 0);

    Mutex_Lock(mutex1, 0);
    Mutex_Lock(mutex2, 0);
    Mutex_Lock(mutex2, 0);

    Mutex_UnLock(mutex2);
    Mutex_UnLock(mutex1);

    success = Mutex_TryLock(mutex2, 0);
    Assert(success == true, "");
    success = Mutex_TryLock(mutex1, 0);
    Assert(success == true, "");
    success = Mutex_TryLock(mutex1, 0);
    Assert(success == false, "");

    Mutex_UnLock(mutex1);
    Mutex_Destroy(mutex1);
    Mutex_UnLock(mutex2);
    Mutex_UnLock(mutex2);
    Mutex_Destroy(mutex2);
}

////////////////////////////////////////////////////////////////////////////////
void Stat_Test0()
{
    statcycle stat_data_cycle;

    StatCycle_Reset(&stat_data_cycle);

    StatCycle_Float_Add(&stat_data_cycle, 5);
    StatCycle_Float_Add(&stat_data_cycle, 4);
    StatCycle_Float_Add(&stat_data_cycle, 3);

    Assert(stat_data_cycle.m_data_ave.m_float == 4, "");

    StatCycle_Float_Add(&stat_data_cycle, 88);
    Assert(stat_data_cycle.m_data_ave.m_float == 25, "");
}

////////////////////////////////////////////////////////////////////////////////
void Argument_Test0()
{
    strcrc local_name = StrCrc(__FUNCTION__, 0);
    Argument* argument = ARG_New2(&local_name, float, 1.1f, int32, 111);

    Assert(Argument_Get(argument, 0).m_float == 1.1f, "");
    Assert(Argument_Get(argument, 1).m_int32 == 111, "");

    Argument_Del(argument);
}

////////////////////////////////////////////////////////////////////////////////
typedef void (*CB_Test_Void)();
static void Test(uint32 index, CB_Test_Void cb_test_void)
{
    Log(0, "Test %d\n", index);

    cb_test_void();

    Memory_Stat();
}

void Engine_Debug_UnitTesting0()
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

        Mutex_Test0,

        Stat_Test0,

        Argument_Test0,
    };

    Log(0, "%s Start\n", __FUNCTION__);
    Memory_Stat();

    for(uint32 i=0, max_i = ARRAY_SIZE(cb_test_void_array); i<max_i; i++)
    {
        Test(i, cb_test_void_array[i]);
    }

}
