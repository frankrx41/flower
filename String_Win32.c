#include "CoreMini.h"
#if PLATFORM_WIN32

#include "String.h"

#include <stdio.h>

#pragma warning(disable: 4996)

void String_FormatArgs(String* string, const tchar* format, va_list args)
{
    tchar buffer[4996];
    vsnprintf(buffer, 4996, format, args);

    String_Copy(string, buffer, 0);
}

#endif
