#include "CoreMini.h"

int32 main()
{
    Log(2, "error %s", "hello");
    return 0;
};

// Log
#if 0
int32 main()
{
    Log(2, "error %s", "hello");
    return 0;
};
#endif

// Memory test
#if 0
#include "Memory.h"

int32 main()
{
    Assert(true, "");
    tptr ptr = MemNewSize("a", 256);
    MemDel("a", ptr);
    return 0;
};
#endif

// String test
#if 0
#include "String.h"
#include <stdio.h>

int32 main()
{

    String* string = String_New("hello world", 0);
    printf(String_CStr(string));
    printf("\n");
    String_Del(string);

    for( int32 i=0; i<10; i++)
    {
        printf("%x\t", String_Crc("hello world"));
        printf("%x\n", String_Crc("hello world2"));
    }
    return 0;
}

#endif
