#include "CoreMini.h"

#include "tData.h"


#undef tData_Cast
#undef tData

const tdata tdata_null = {0};

tdata tData(const tchar* type, ...)
{
    tdata data;

    va_list ap;
    va_start(ap, type);

    switch( type[0] )
    {
    case 'i':   data.m_int32    = va_arg(ap, int32);            break;
    case 'f':   data.m_float    = (float)va_arg(ap, double);    break;  // float is promoted to double
    case 'u':   data.m_uint32   = va_arg(ap, uint32);           break;
    case 'v':   data.m_tptr     = va_arg(ap, void*);            break;
    default:
    {
        Assert(type[0] == 't', "");
        switch( type[1] )
        {
        case 's':   data.m_tsize    = va_arg(ap, tsize);    break;
        default:    Assert(false, "Unknow type!");
        }
    }
        
    }

    va_end(ap);
    return data;
}
