#include "CoreMini.h"

#include "Data32.h"

#undef Data32_Cast
#undef Data32

tptr Data32_Cast(const tchar* type, data32 data)
{
    switch( type[0] )
    {
    case 'i':   return (tptr)&data.m_int32;
    case 'f':   return (tptr)&data.m_float;
    default:    return (tptr)&data.m_pointer;
    }
}

// We cannot convert from 'float' to 'tptr', so we covert to 'uint32' first.
data32 Data32(const tchar* type, int32 i, float f, tptr p)
{
    data32 data32;
    switch( type[0] )
    {
    case 'i':   data32.m_int32 = i; break;
    case 'f':   data32.m_float = f; break;
    default:    data32.m_pointer = p;
    }
    return data32;
}
