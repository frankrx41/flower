#include "CoreMini.h"
#if PLATFORM_WIN32

#include "String.h"

#include <Windows.h>

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

    switch(type)
    {
    case 0: OutputDebugStringA("[Info]    ");   break;
    case 1: OutputDebugStringA("[Warn]    ");   break;
    case 2: OutputDebugStringA("[Error]   ");   break;
    case 4: OutputDebugStringA("[Profile] ");   break;
    default: Assert(false, "");
    }
    OutputDebugStringA(buffer);
}

#endif
