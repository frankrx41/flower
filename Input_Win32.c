#include "CoreMini.h"
#if PLATFORM_WIN32

#include "KeyId.h"

#include <Windows.h>

static int32 KeyID_To_WinVK(KeyId key_id)
{
    switch(key_id)
    {
    case KeyId_Up:      return VK_UP;
    case KeyId_Down:    return VK_DOWN;
    case KeyId_Left:    return VK_LEFT;
    case KeyId_Right:   return VK_RIGHT;
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
        return GetAsyncKeyState(KeyID_To_WinVK(key_id)) & 0x8000;
    }
}

#endif
