#include "CoreMini.h"

#include "String.h"
#include "Memory.h"

/*
Code Style:

String  - string
tchar   - str

*/

struct String
{
    uint32  m_length;
    crc32   m_crc;
    tchar*  m_char;
};

#define LOCAL_NAME          "String"
#define LOCAL_NAME_CHAR     "Char"


static void String_ReAllocSize(String* string, uint32 length)
{
    Assert( string != NULL, "" );

    // make length is a litter big
    length = (((length>>4) +1) << 4) + 4;

    if( string->m_char != NULL && Engine_Memory_GetSize(string->m_char) >= length )
    {
        return;
    }

    MemSafeDel(string->m_char);
    string->m_char = MemNewSize(LOCAL_NAME_CHAR, length);
}

////////////////////////////////////////////////////////////////////////////////
uint32 Str_CalcLength(const tchar* str)
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
crc32 Str_CalcCrc(const tchar* str, uint32 length)
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

uint32 String_GetLength(const String* string)
{
    return string->m_length;
}

crc32 String_GetCrc(const String* string)
{
    return string->m_crc;
}

tchar* String_CStr(const String* string)
{
    return string->m_char;
}

void String_Format(String* string, const tchar* format, ...)
{
    va_list ap;
    va_start(ap, format);

    String_FormatArgs(string, format, ap);
}

void String_FormatArgs(String* string, const tchar* format, va_list args)
{
    tchar buffer[4996];
    int32 vsprintf(tchar*, const tchar*, va_list);
    vsprintf(buffer, format, args);

    String_Copy(string, buffer, 0);
}

void String_Copy(String* string, const tchar* str, uint32 length)
{
    // length == 0 means copy full str
    if( length == 0 )
    {
        length = Str_CalcLength(str);
    }

    if( length < 0 )
    {
        Assert( false, "We hope negative number means copy from right side, but we have no impl it" );
    }

    {
        String_ReAllocSize(string, length);

        uint32 i = 0;
        for(; str[i] != NULL && i < length; i++)
        {
            string->m_char[i] = str[i];
        }
        string->m_char[i] = '\0';
        string->m_length = i;
    }

    {
        string->m_crc = Str_CalcCrc(string->m_char, string->m_length);
    }

}

String* String_New(const tchar* str)
{
    String* string = MemNew(LOCAL_NAME, String);
    string->m_char = NULL;
	string->m_length = 0;
	
    if( !Str_IsEmpty(str) )
    {
        int32 str_length = Str_CalcLength(str);
        String_Copy(string, str, str_length);
    }

    return string;
}

void String_Del(String* string)
{
    MemSafeDel(string->m_char);
    MemDel(string);
}

