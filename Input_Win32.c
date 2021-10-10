#include "CoreMini.h"
#if PLATFORM_WIN32

#include "KeyId.h"

#include <Windows.h>

static int32 KeyId_To_WinVK(KeyId key_id)
{
    Assert( key_id != KeyId_Null, "");
    Assert( key_id != KeyId_Max, "");

    switch(key_id)
    {
    case KeyId_Up:      return VK_UP;
    case KeyId_Down:    return VK_DOWN;
    case KeyId_Left:    return VK_LEFT;
    case KeyId_Right:   return VK_RIGHT;
    case KeyId_Escape:  return VK_ESCAPE;
    case KeyId_Space:   return VK_SPACE;
    case KeyId_F1:      return VK_F1;
    case KeyId_F2:      return VK_F2;
    case KeyId_F3:      return VK_F3;
    case KeyId_F4:      return VK_F4;
    case KeyId_F5:      return VK_F5;
    case KeyId_F6:      return VK_F6;
    case KeyId_F7:      return VK_F7;
    case KeyId_F8:      return VK_F8;
    case KeyId_F9:      return VK_F9;
    case KeyId_F10:     return VK_F10;
    case KeyId_F11:     return VK_F11;
    case KeyId_F12:     return VK_F12;
    case KeyId_F13:     return VK_F13;
    case KeyId_F14:     return VK_F14;
    case KeyId_F15:     return VK_F15;
    case KeyId_F16:     return VK_F16;
    case KeyId_F17:     return VK_F17;
    case KeyId_F18:     return VK_F18;
    case KeyId_F19:     return VK_F19;
    case KeyId_F20:     return VK_F20;
    case KeyId_0:       return '0';
    case KeyId_1:       return '1';
    case KeyId_2:       return '2';
    case KeyId_3:       return '3';
    case KeyId_4:       return '4';
    case KeyId_5:       return '5';
    case KeyId_6:       return '6';
    case KeyId_7:       return '7';
    case KeyId_8:       return '8';
    case KeyId_9:       return '9';
    case KeyId_A:       return 'A';
    case KeyId_B:       return 'B';
    case KeyId_C:       return 'C';
    case KeyId_D:       return 'D';
    case KeyId_E:       return 'E';
    case KeyId_F:       return 'F';
    case KeyId_G:       return 'G';
    case KeyId_H:       return 'H';
    case KeyId_I:       return 'I';
    case KeyId_J:       return 'J';
    case KeyId_K:       return 'K';
    case KeyId_L:       return 'L';
    case KeyId_M:       return 'M';
    case KeyId_N:       return 'N';
    case KeyId_O:       return 'O';
    case KeyId_P:       return 'P';
    case KeyId_Q:       return 'Q';
    case KeyId_R:       return 'R';
    case KeyId_S:       return 'S';
    case KeyId_T:       return 'T';
    case KeyId_U:       return 'U';
    case KeyId_V:       return 'V';
    case KeyId_W:       return 'W';
    case KeyId_X:       return 'X';
    case KeyId_Y:       return 'Y';
    case KeyId_Z:       return 'Z';
    case KeyId_BackSpace:   return VK_BACK;
    case KeyId_Tab:         return VK_TAB;
    case KeyId_CapsLock:    return VK_CAPITAL;
    case KeyId_LeftShift:   return VK_LSHIFT;
    case KeyId_RightShift:  return VK_RSHIFT;
    case KeyId_LeftCtrl:    return VK_LCONTROL;
    case KeyId_RightCtrl:   return VK_RCONTROL;
    case KeyId_LeftAlt:     return VK_LMENU;
    case KeyId_RightAlt:    return VK_RMENU;
    case KeyId_Enter:       return VK_RETURN;
    default:
        Assert(false, "");
        return 0;
    }
}

bool InputManager_Key_IsPhysicsDown_Plat(KeyId key_id)
{
    if( key_id == KeyId_Null )
    {
        return false;
    }
    else
    {
        int32 key_state = GetKeyState(KeyId_To_WinVK(key_id));
        return key_state & 0x80;
    }
}

#endif
