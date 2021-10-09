#pragma once

typedef union data32 data32;

extern const data32 data32_null;

union data32
{
    int32   m_int32;
    float   m_float;
    uint32  m_uint32;
    ptr32   m_ptr32; 
};

COMPILE_TIME_ASSERT(sizeof(data32) >= sizeof(float), "");

data32  Data32(const tchar* type, ...);

#define Float(x)    ((float)(x))
#define Int32(x)    ((int32)(x))
#define Ptr32(x)    ((ptr32)(x))
#define Uint32(x)   ((uint32)(x))

#define Data32(type, x)         Data32(#type, x)
