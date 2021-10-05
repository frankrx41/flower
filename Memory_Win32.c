#include "CoreMini.h"

#include <stdlib.h>
#include <string.h>

tptr Memory_Alloc_Plat(tsize size)
{
    return
    malloc(size);
}

tptr Memory_Copy_Plat(tptr dst, const tptr src, tsize size)
{
    return
    memcpy(dst, src, size);
}

tptr Memory_Set_Plat(tptr dst, int32 val, tsize size)
{
    return
    memset(dst, val, size);
}

void Memory_Free_Plat(void * memory)
{
    free(memory);
}

