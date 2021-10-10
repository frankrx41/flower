#pragma once

typedef union tdata tdata;

extern const tdata tdata_null;

union tdata
{
    int32   m_int32;
    float   m_float;
    uint32  m_uint32;
    tptr    m_tptr; 
};

tdata  tData(const tchar* type, ...);

#define Float(x)    ((float)(x))
#define Int32(x)    ((int32)(x))
#define tPtr(x)     ((tptr)(x))
#define tChar(x)    ((tchar)(x))
#define uInt32(x)   ((uint32)(x))

#define tData(type, x)          (*(tdata*)((type*)0), tData(#type, x))
