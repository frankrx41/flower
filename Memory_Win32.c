#include "CoreMini.h"

#define PLATFORM_WIN32 1
#include "Platform.h"
#include <stdlib.h>
#include <string.h>


ptr32 Memory_Alloc_Plat(tsize size)
{
    return
    malloc(size);
}

ptr32 Memory_Copy_Plat(ptr32 dst, const ptr32 src, tsize size)
{
    return
    memcpy(dst, src, size);
}

ptr32 Memory_Set_Plat(ptr32 dst, int32 val, tsize size)
{
    return
    memset(dst, val, size);
}

void Memory_Free_Plat(void * memory)
{
    free(memory);
}

