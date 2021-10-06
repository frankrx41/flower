#pragma once

typedef union data32 data32;

union data32
{
    int32   m_int32;
    float   m_float;
    uint32  m_uint32;
    tptr    m_pointer; 
};

StaticAssert(sizeof(data32) == sizeof(float), "");

data32  Data32(const tchar* type, ...);
tptr    Data32_Cast(const tchar* type, data32 data);

extern const data32 data32_null;

#define Float(x)    ((float)(x))
#define Int32(x)    ((int32)(x))
#define Tptr(x)     ((tptr)(x))
#define Uint32(x)   ((uint32)(x))

#define Data32(type, x)         Data32(#type, x)
#define Data32_Cast(type, data) *((type*)Data32_Cast(#type, data))
