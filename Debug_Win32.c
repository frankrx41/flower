#define PLATFORM_WIN32      1
#include "CoreMini.h"

#include <Windows.h>

void Engine_Debug_Break()
{
    __debugbreak();
}

error Log(int32 type, const tchar* format, ...)
{

}
