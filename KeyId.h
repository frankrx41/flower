#pragma once

typedef enum KeyId KeyId;

enum KeyId
{
    KeyId_Min,

    KeyId_Null = KeyId_Min,

    KeyId_Up,
    KeyId_Down,
    KeyId_Left,
    KeyId_Right,

    KeyId_Max
};

#define KEYID_IS_DEFINED 1
