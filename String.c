#include "CoreMini.h"

#include "MemoryManager.h"

#include "String.h"

/*
Code Style:

String  - string
tchar   - str

*/

#define HELP_CALC_CRC 1

struct String
{
    bool    m_is_const;
    crc32   m_crc;
    tsize   m_length;
    strcrc  m_local_name;
    tchar*  m_char;
};

static void String_ReAllocSize(String* string, const tsize length, bool keep_data)
{
    Assert( string != NULL, "" );

    // make length is a litter big
    const tsize size = (((length>>4) +1) << 4) + 4;

    if( string->m_char != NULL && Memory_GetSize(string->m_char) >= size )
    {
        return;
    }

    tsize origin_length = String_GetLength(string);
    tchar* origin_str   = string->m_char;
    string->m_char = MemNewSize(&string->m_local_name, size);

    if( keep_data )
    {
        Assert(origin_str != NULL && origin_length != 0, "You try to keep a empty data!");
        Str_Copy(string->m_char, origin_str, origin_length);
        string->m_length = origin_length;
    }
    else
    {
        string->m_length = 0;
    }
    MemSafeDel(origin_str);
}

////////////////////////////////////////////////////////////////////////////////
tsize Str_CalcLength(const tchar* str)
{
    if( str == NULL )
    {
        return 0;
    }

    uint32 i = 0;
    for(; str[i] != NULL; i++)
    {
        ;
    }
    return i;
}

// Copy: https://rosettacode.org/wiki/CRC-32#C
crc32 Str_CalcCrc(const tchar* str, tsize length)
{
    crc32 crc = 0;
    if(length == 0)
    {
        length = Str_CalcLength(str);
    }
    static uint32 table[256];
    static int32 have_table = 0;
    uint32 rem;
    byte octet;
    int32 i,j;
    const tchar *p,*q;

    /* This check is not thread safe; there is no mutex. */
    if(have_table == 0) {
        /* Calculate CRC table. */
        for(i = 0; i < 256; i++) {
            rem = i;  /* remainder from polynomial division */
            for(j = 0; j < 8; j++) {
                if(rem & 1) {
                    rem >>= 1;
                    rem ^= 0xedb88320;
                }
                else
                    rem >>= 1;
            }
            table[i] = rem;
        }
        have_table = 1;
    }

    crc = ~crc;
    q = str + length;
    for(p = str; p < q; p++) {
        octet = *p;  /* Cast to unsigned octet. */
        crc = (crc >> 8) ^ table[(crc & 0xff) ^ octet];
    }
    return ~crc;
}

bool Str_IsEmpty(const tchar* str)
{
    return str ? str[0] == NULL : true;
}

bool Str_IsSame(const tchar* str1, const tchar* str2)
{
    if( str1 == NULL && str2 == NULL )
    {
        return true;
    }
    if( str1 == NULL || str2 == NULL )
    {
        return false;
    }

    uint32 i = 0;
    for( ; ; i++ )
    {
        if( str1[i] != str2[i] )
        {
            return false;
        }
        if( str1[i] == NULL )
        {
            break;
        }
    }
    return true;
}

tsize Str_Copy(tchar* dest, const tchar* from, tsize length)
{
    const tsize from_str_length = Str_CalcLength(from);
    if( length == 0 )
    {
        length = from_str_length;
    }
    else
    {
        length = MIN( length, from_str_length );
    }

    Assert(length > 0, "");
    Assert(Memory_IsInBounds(dest, dest+length), "");

    uint32 i=0;
    for(i=0; i<length; i++)
    {
        dest[i] = from[i];
    }
    dest[i] = '\0';

    return length;
}

////////////////////////////////////////////////////////////////////////////////
strcrc StrCrc(const tchar* str, crc32 crc)
{
    strcrc out_strcrc;
#if HELP_CALC_CRC
    if( crc == 0 )
    {
        crc = Str_CalcCrc(str, Str_CalcLength(str));
    }
#endif
    Assert(crc == Str_CalcCrc(str, Str_CalcLength(str)), "");

    out_strcrc.m_str    = str;
    out_strcrc.m_crc32  = crc;

    return out_strcrc;
}

strcrc* StrCrc_Copy(const strcrc* source_strcrc, strcrc* out_strcrc)
{
    Assert(source_strcrc->m_crc32 != 0, "");
    out_strcrc->m_crc32 = source_strcrc->m_crc32;
    out_strcrc->m_str   = source_strcrc->m_str;

    return out_strcrc;
}

bool StrCrc_IsSame(const strcrc* strcrc1, const strcrc* strcrc2)
{
    return strcrc1->m_crc32 == strcrc2->m_crc32;
}

bool StrCrc_IsEmpty(const strcrc* strcrc)
{
    return strcrc->m_crc32 == 0;
}

////////////////////////////////////////////////////////////////////////////////
tsize String_GetLength(const String* string)
{
    return string->m_length;
}

crc32 String_GetCrc(const String* string)
{
    Assert(string->m_crc != 0, "");
    return string->m_crc;
}

tchar* String_CStr(const String* string)
{
    return string->m_char;
}

bool String_IsSame(const String* str1, const String* str2)
{
    if( str1->m_is_const && str2->m_is_const )
    {
        return String_GetCrc(str1) == String_GetCrc(str2);
    }
    else
    {
        return Str_IsSame(String_CStr(str1), String_CStr(str2));
    }
}

void String_Format(String* string, const tchar* format, ...)
{
    Assert(string->m_is_const != true, "You try to modify a const string!");

    va_list ap;
    va_start(ap, format);

    String_FormatArgs(string, format, ap);
    va_end(ap);
}

void String_Concatenate(String* string, const tchar* str, const tsize str_length)
{
    Assert(string->m_is_const != true, "");

    const tsize string_length   = String_GetLength(string);
    tsize str_copy_length = str_length;
    if( str_length == 0 )
    {
        str_copy_length = Str_CalcLength(str);
    }
    const tsize total_length    = string_length + str_copy_length;
    String_ReAllocSize(string, total_length, true);

    Str_Copy(string->m_char+string_length, str, str_copy_length);
    string->m_length = total_length;
}

void String_Copy(String* string, const tchar* str, const tsize length)
{
    Assert(string->m_is_const != true, "You try to modify a const string!");
    Assert( length >= 0 , "");

    tsize size = length;
    // length == 0 means copy full str
    if( length == 0 )
    {
        size = Str_CalcLength(str);
    }

    {
        String_ReAllocSize(string, size, false);

        const tsize string_length = Str_Copy(string->m_char, str, length);
        string->m_length = string_length;
    }

    if( true )
    {
        Assert(str != NULL, "You can not create a const NULL string!");
        string->m_crc = Str_CalcCrc(string->m_char, string->m_length);
    }
}

String* String_New(const strcrc* local_name, const tchar* str, crc32 crc, bool is_const)
{
    Assert(local_name != NULL, "");

    String* string = MemNew(local_name, String);

    StrCrc_Copy(local_name, &string->m_local_name);
    string->m_char          = NULL;
    string->m_length        = 0;
    string->m_crc           = crc;
    string->m_is_const      = is_const;

    if( !Str_IsEmpty(str) )
    {
        if( string->m_is_const )
        {
            const int32 str_length = Str_CalcLength(str);

            string->m_char = (tchar*)str;
            string->m_length = str_length;

#if HELP_CALC_CRC
            if( crc == 0 )
            {
                string->m_crc = Str_CalcCrc(string->m_char, string->m_length);
            }
#endif
            Assert(string->m_crc == Str_CalcCrc(string->m_char, string->m_length), "");
        }
        else
        {
            const int32 str_length = Str_CalcLength(str);
            String_Copy(string, str, str_length);
        }
    }


    return string;
}

void String_Del(String* string)
{
    if( !string->m_is_const )
    {
        MemSafeDel(string->m_char);
    }
    MemDel(string);
}

