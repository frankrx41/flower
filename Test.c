#include "CoreMini.h"

#include "String.h"
#include "Memory.h"
#include "Queue.h"
#include "Storage.h"

#include <stdio.h>



void Storage_Test0()
{
    Storage* storage = Storage_Create(__FUNCTION__);
    Storage_StoreInt32(storage, Str_CalcCrc("a",0), 123);

    printf("%d\n", Storage_IsExistVariable(storage, Str_CalcCrc("a", 0)));
    printf("%d\n", Storage_IsExistVariable(storage, Str_CalcCrc("b", 0)));

    int32 data1 = Storage_ReadInt32(storage, Str_CalcCrc("a",0));
    int32 data2 = Storage_ReadInt32(storage, Str_CalcCrc("b",0));
    printf("%d\n", data1);
    printf("%d\n", data2);
}



typedef struct Data Data;
struct Data
{
    int32 m_a;
};

void Print_Data(Data* a, tptr ptr)
{
    printf("%d\n", a->m_a);
}


bool QueueFind(Data* a, int32 v)
{
    if(a->m_a == v)
    {
        return true;
    }
    return false;
}

void Queue_Test0()
{

    Queue(Data*)* queue = Queue_Create("test", Data*);

    Data x = {1};
    Data y = {2};
    Data z = {3};

    Queue_Push(Data*, queue, &x);
    Queue_Push(Data*, queue, &y);
    Queue_Push(Data*, queue, &z);

    Queue_ForEach(queue, Print_Data, NULL);

    Queue_RemoveFrist(Data*)(queue, (FindDataFunc)QueueFind, (tptr)2);

    printf("\n");
    Queue_ForEach(queue, Print_Data, NULL);


    Queue_Destroy(queue);

}

void Log_Test0()
{
    Log(0, "info %s", "hello");
    Log(1, "warn %s", "hello");
    Log(2, "error %s", "hello");
};

void Memory_Test0()
{
    tptr ptr = MemNewSize("a", 256);
    MemDel(ptr);
};


void String_Test2()
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


void String_Test1()
{
    String* a = String_New(NULL);

    String_Copy(a, "say hi\n", 0);

    Log(1, String_CStr(a));

    String_Copy(a, "bye\n", 0);

    Log(1, String_CStr(a));

    String_Del(a);
};

void String_Test0()
{

    String* string1 = String_New("hello world");
    String* string2 = String_New("goodbye world");
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

