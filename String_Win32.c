#include "CoreMini.h"

#include "String.h"

#define PLATFORM_WIN32      1
#include "Platform.h"
#include <stdio.h>


void String_FormatArgs(String* string, const tchar* format, va_list args)
{
    tchar buffer[4996];
    vsnprintf(buffer, 4996, format, args);

    String_Copy(string, buffer, 0);
}
