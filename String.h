#pragma once

// TODO: impl stdarg by engine
#include <stdarg.h>

typedef struct String String;


uint32  Str_CalcLength      (const tchar* str);
crc32   Str_CalcCrc         (const tchar *str, uint32 length);

uint32  String_GetLength    (const String* string);
crc32   String_GetCrc       (const String* string);
tchar*  String_CStr         (const String* string);
void    String_Format       (String* string, const tchar* format, ...);
void    String_FormatArgs   (String* string, const tchar* format, va_list);
void    String_Copy         (String* string, const tchar* str, uint32 length);


String* String_New          (const tchar* str);
String* String_Clone        (String* string);
void    String_Del          (String* string);

