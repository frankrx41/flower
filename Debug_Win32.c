#include "CoreMini.h"
#if PLATFORM_WIN32

#include "String.h"

#include "Platform.h"


void Debug_Break()
{
    __debugbreak();
}

void Debug_Log(int32 type, const tchar* format, ...)
{
    tchar buffer[4996];
    va_list ap;
    va_start(ap, format);
    Str_FormatArgs(buffer, 4996, format, ap);
    va_end(ap);

    switch(type)
    {
    case 0: OutputDebugStringA("[Info] ");   break;
    case 1: OutputDebugStringA("[Warn] ");   break;
    case 2: OutputDebugStringA("[Errr] ");   break;
    case 4: OutputDebugStringA("[Prof] ");   break;
    default: Assert(false, "");
    }
    OutputDebugStringA(buffer);
}

#endif
