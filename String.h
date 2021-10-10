#pragma once

typedef struct String String;


uint32  Str_CalcLength      (const tchar* str);
crc32   Str_CalcCrc         (const tchar* str, uint32 length);
bool    Str_IsEmpty         (const tchar* str);
bool    Str_IsSame          (const tchar* str1, const tchar* str2);
void    Str_Copy            (tchar* dest, const tchar* from, uint32 length);

uint32  String_GetLength    (const String* string);
crc32   String_GetCrc       (const String* string);
tchar*  String_CStr         (const String* string);
bool    String_IsSame       (const String* str1, const String* str2);
void    String_Format       (String* string, const tchar* format, ...);
void    String_FormatArgs   (String* string, const tchar* format, va_list args);
void    String_Copy         (String* string, const tchar* str, uint32 length);

String* String_New          (const tchar* local_name, const tchar* str, bool is_const);
String* String_Clone        (const tchar* local_name, String* string);
void    String_Del          (String* string);

