#include "CoreMini.h"

#include "String.h"
#include "Memory.h"
#include "Queue.h"

#include <stdio.h>

typedef struct Data Data;
struct Data
{
    int32 m_a;
};

void Print_Data(Data* a)
{
    printf("%d\n", a->m_a);
}


bool QueueFind(Data* a)
{
    if(a->m_a == 2)
    {
        return true;
    }
    return false;
}

void Queue_Test0()
{

    Queue(Data)* queue = Queue_Create("test", Data*);

    Data x = {1};
    Data y = {2};
    Data z = {3};

    Queue_Push((Data*), "test", queue, &x);
    Queue_Push((Data*), "test", queue, &y);
    Queue_Push((Data*), "test", queue, &z);

    Queue_ForEach(queue, Print_Data);

    Queue_RemoveFrist(Data*)(queue, QueueFind);

    printf("\n");
    Queue_ForEach(queue, Print_Data);


    Queue_Destroy(queue);

}

void String_Test1()
{
    Log(2, "error %s\n", "hello");

    String* a = String_New(NULL);

    String_Copy(a, "say hi\n", 0);

    Log(1, String_CStr(a));

    String_Copy(a, "bye\n", 0);

    Log(1, String_CStr(a));

    String_Del(a);
};

void Log_Test0()
{
    Log(2, "error %s", "hello");
};

void Memory_Test0()
{
    Assert(true, "");
    tptr ptr = MemNewSize("a", 256);
    MemDel(ptr);
};

void String_Test0()
{

    String* string = String_New("hello world");
    printf(String_CStr(string));
    printf("\n");
    String_Del(string);

    for( int32 i=0; i<10; i++)
    {
        printf("%x\t", Str_CalcCrc("hello world", 0));
        printf("%x\n", Str_CalcCrc("hello world2", 0));
    }
}

