#pragma once

typedef struct String String;


tsize   Str_CalcLength      (const tchar* str);
crc32   Str_CalcCrc         (const tchar* str, tsize length);
bool    Str_IsEmpty         (const tchar* str);
bool    Str_IsSame          (const tchar* str1, const tchar* str2);
tsize   Str_Copy            (tchar* dest, const tchar* from, tsize length);
void    Str_FormatArgs      (tchar* buffer, tsize length, const tchar* format, va_list args);
void    Str_Join            (tchar* buffer, uint32 count, ...);
wchar*  wStr_New  (const tchar* str);

tsize   String_GetLength    (const String* string);
crc32   String_GetCrc       (const String* string);
tchar*  String_CStr         (const String* string);
bool    String_IsSame       (const String* str1, const String* str2);
void    String_Format       (String* string, const tchar* format, ...);
void    String_Concatenate  (String* string, const tchar* str, const tsize str_length);
void    String_FormatArgs   (String* string, const tchar* format, va_list args);
void    String_Copy         (String* string, const tchar* str, const tsize length);

String* String_New          (const tchar* local_name, const tchar* str, bool is_const);
// String* String_New          (const tchar* local_name, bool is_const, bool is_need_crc, uint32 str_count, ...);
String* String_Clone        (const tchar* local_name, String* string);
void    String_Del          (String* string);

