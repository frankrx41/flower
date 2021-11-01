#include "CoreMini.h"

#include "MemoryManager.h"

#include "tData.h"
#include "Argument.h"

struct Argument
{
    tdata   m_data[];
};

Argument* Argument_New(const strcrc* local_name, uint32 count, ...)
{
    Argument* argument = MemNewSize(local_name, sizeof(tdata)*count);

    va_list ap;
    va_start(ap, count);
    for( uint32 i=0; i<count; i++ )
    {
        const tdata data = va_arg(ap, tdata);
        argument->m_data[i] = data;
    }

    return argument;
}

void Argument_Del(Argument* argument)
{
    MemDel(argument);
}

tdata Argument_Get(const Argument* argument, uint32 index)
{
    Assert(Memory_IsInBounds(argument, &argument->m_data[index]), "");
    return argument->m_data[index];
}
