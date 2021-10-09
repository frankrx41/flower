#include "CoreMini.h"

#include "Data32.h"


#undef Data32_Cast
#undef Data32

const data32 data32_null = {0};

data32 Data32(const tchar* type, ...)
{
    data32 data32;

    va_list ap;
    va_start(ap, type);

    switch( type[0] )
    {
    case 'i':   data32.m_int32  = va_arg(ap, int32);            break;
    case 'f':   data32.m_float  = (float)va_arg(ap, double);    break;  // float is promoted to double
    case 'u':   data32.m_uint32 = va_arg(ap, uint32);           break;
    default:    data32.m_tptr   = va_arg(ap, tptr);             break;;
    }

    va_end(ap);
    return data32;
}
