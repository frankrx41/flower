#include "CoreMini.h"

#include <stdlib.h>
#include <string.h>

tptr Engine_Memory_Alloc_Plat(tsize size)
{
    return
    malloc(size);
}

tptr Engine_Memory_Copy_Plat(tptr dst, const tptr src, tsize size)
{
    return
    memcpy(dst, src, size);
}

tptr Engine_Memory_Set_Plat(tptr dst, int32 val, tsize size)
{
    return
    memset(dst, val, size);
}

error Engine_Memory_Free_Plat(void * memory)
{
    free(memory);
    return 0;
}

