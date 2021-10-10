#include "CoreMini.h"
#if PLATFORM_WIN32

#include "String.h"

#include <Windows.h>

#define LOCAL_NAME  "Debug"

void Engine_Debug_Break()
{
    __debugbreak();
}

void Engine_Debug_Log(int32 type, const tchar* format, ...)
{
    tchar buffer[4996];
    va_list ap;
    va_start(ap, format);
    Str_FormatArgs(buffer, 4996, format, ap);
    va_end(ap);

    OutputDebugStringA(buffer);
}

#endif
