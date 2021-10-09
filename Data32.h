#pragma once

typedef union data32 data32;

extern const data32 data32_null;

union data32
{
    int32   m_int32;
    float   m_float;
    uint32  m_uint32;
    tptr    m_tptr; 
};

COMPILE_TIME_ASSERT(sizeof(data32) >= sizeof(float), "");

data32  Data32(const tchar* type, ...);

#define Float(x)    ((float)(x))
#define Int32(x)    ((int32)(x))
#define tPtr(x)     ((tptr)(x))
#define tChar(x)    ((tchar)(x))
#define Uint32(x)   ((uint32)(x))

#define Data32(type, x)         (*(data32*)((type*)0), Data32(#type, x))
