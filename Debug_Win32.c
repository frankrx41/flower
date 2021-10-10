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
    String* string = String_New(LOCAL_NAME, "", false);
    va_list ap;
    va_start(ap, format);
    String_FormatArgs(string, format, ap);
    va_end(ap);

    OutputDebugStringA(String_CStr(string));
    String_Del(string);
}

#endif
