#pragma once

// TODO: impl stdarg by engine
#include <stdarg.h>

typedef struct String String;

int32   String_GetLength    (const String* string);
tchar*  String_CStr         (String* string);
error   String_Format       (String* buff, const tchar* format, ...);
error   String_FormatArgs   (String* buff, const tchar* format, va_list);
error   String_Copy         (String* dest, const tchar* sour, int32 length);

crc32   String_Crc          (const tchar* string);
int32   String_CalcLength   (const tchar* string);

String* String_Clone        (const String* string);
String* String_New          (const tchar* string, int32 length);
error   String_Del          (String* string);

