#pragma once

typedef union data32 data32;

union data32
{
    int32 m_int32;
    float m_float;
    tptr m_pointer; 
};

data32  Data32(const tchar* type, int32 i, float f, tptr p);
tptr    Data32_Cast(const tchar* type, data32 data);

#define Data32(type, x)         Data32(#type, (int32)x, (float)(uint32)x, (tptr)(uint32)x)
#define Data32_Cast(type, data) *((type*)Data32_Cast(#type, data))
