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

    KeyId_Escape,
    KeyId_Space,

    KeyId_F1,
    KeyId_F2,
    KeyId_F3,
    KeyId_F4,
    KeyId_F5,
    KeyId_F6,
    KeyId_F7,
    KeyId_F8,
    KeyId_F9,
    KeyId_F10,
    KeyId_F11,
    KeyId_F12,
    KeyId_F13,
    KeyId_F14,
    KeyId_F15,
    KeyId_F16,
    KeyId_F17,
    KeyId_F18,
    KeyId_F19,
    KeyId_F20,

    KeyId_0,
    KeyId_1,
    KeyId_2,
    KeyId_3,
    KeyId_4,
    KeyId_5,
    KeyId_6,
    KeyId_7,
    KeyId_8,
    KeyId_9,

    KeyId_A,
    KeyId_B,
    KeyId_C,
    KeyId_D,
    KeyId_E,
    KeyId_F,
    KeyId_G,
    KeyId_H,
    KeyId_I,
    KeyId_J,
    KeyId_K,
    KeyId_L,
    KeyId_M,
    KeyId_N,
    KeyId_O,
    KeyId_P,
    KeyId_Q,
    KeyId_R,
    KeyId_S,
    KeyId_T,
    KeyId_U,
    KeyId_V,
    KeyId_W,
    KeyId_X,
    KeyId_Y,
    KeyId_Z,

    KeyId_BackSpace,
    KeyId_Tab,
    KeyId_CapsLock,
    KeyId_LeftShift,
    KeyId_RightShift,
    KeyId_LeftCtrl,
    KeyId_RightCtrl,
    KeyId_LeftAlt,
    KeyId_RightAlt,
    KeyId_Enter,


    KeyId_Max
};

#define KEYID_IS_DEFINED 1
