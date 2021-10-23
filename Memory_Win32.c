#include "CoreMini.h"
#if PLATFORM_WIN32

#include "Platform.h"


void* Memory_Alloc_Platform(tsize size)
{
    return
    malloc(size);
}

void* Memory_Copy_Platform(void* out, const void* src, tsize size)
{
    return
    memcpy(out, src, size);
}

void* Memory_Set_Platform(void* dst, int32 val, tsize size)
{
    return
    memset(dst, val, size);
}

void Memory_Free_Platform(void* memory)
{
    free(memory);
}

#endif
