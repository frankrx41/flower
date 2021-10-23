#include "CoreMini.h"

#if PLATFORM_WIN32

#include "MemoryManager.h"

#include "String.h"

#include "Platform.h"


#pragma warning(disable: 4996)

void String_FormatArgs(String* string, const tchar* format, va_list args)
{
    tchar buffer[4996];
    Str_FormatArgs(buffer, 4996, format, args);

    String_Copy(string, buffer, 0);
}

void Str_FormatArgs(tchar* buffer, tsize length, const tchar* format, va_list args)
{
    Assert(buffer != NULL, "");
    vsnprintf(buffer, length, format, args);
}

wchar* wStr_New(const tchar* str)
{
    strcrc local_name;
    StrCrc(__FUNCTION__, 0, &local_name);

    const size_t size = Str_CalcLength(str) + 1;
    wchar* wstr = MemNewSize(&local_name, sizeof(wchar) * size);
    mbstowcs(wstr, str, size);
    return wstr;
}

#endif
