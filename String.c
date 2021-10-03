#include "CoreMini.h"

#include "String.h"
#include "Memory.h"

struct String
{
    int32 m_length;
    int32 m_alloc_length;
    tchar m_char[1];
};

////////////////////////////////////////////////////////////////////////////////
int32 String_CalcLength(const tchar* string)
{
    int32 i = 0;
    for(i; string[i] != NULL; i++)
    {
        ;
    }
    return i;
}

int32 String_GetLength(const String* string)
{
    return string->m_length;
}

tchar* String_CStr(String* string)
{
    return string->m_char;
}
 
error String_Format(String* buff, const tchar* format, ...)
{
    va_list ap;
    va_start(ap, format);

    return
    String_FormatArgs(buff, format, ap);
}

error String_FormatArgs(String* buff, const tchar* format, va_list args)
{
    tchar buffer[4996];
    int32 vsprintf(tchar*, const tchar*, va_list);
    vsprintf(buffer, format, args);

    return
    String_Copy(buff, buffer, 0);
}

error String_Copy(String* dest, const tchar* sour, int32 length)
{
    int32 str_length = String_CalcLength(sour);
    Assert(dest->m_alloc_length > str_length, "Plese re-alloc string");

    if( length <= 0 )
    {
        length = INT32_MAX;
    }
    int32 i = 0;
    for(; sour[i]!=NULL && i<length; i++)
    {
        dest->m_char[i] = sour[i];
    }
    dest->m_char[i] = 0;
    dest->m_length = i;
    
    return 0;
}

// Copy: https://rosettacode.org/wiki/CRC-32#C
crc32 String_Crc(const tchar *buf)
{
    uint32 crc = 0;
    tsize len = String_CalcLength(buf);
	static uint32 table[256];
	static int32 have_table = 0;
	uint32 rem;
	byte octet;
	int32 i, j;
	const tchar *p, *q;
 
	/* This check is not thread safe; there is no mutex. */
	if (have_table == 0) {
		/* Calculate CRC table. */
		for (i = 0; i < 256; i++) {
			rem = i;  /* remainder from polynomial division */
			for (j = 0; j < 8; j++) {
				if (rem & 1) {
					rem >>= 1;
					rem ^= 0xedb88320;
				} else
					rem >>= 1;
			}
			table[i] = rem;
		}
		have_table = 1;
	}
 
	crc = ~crc;
	q = buf + len;
	for (p = buf; p < q; p++) {
		octet = *p;  /* Cast to unsigned octet. */
		crc = (crc >> 8) ^ table[(crc & 0xff) ^ octet];
	}
	return ~crc;
}

String* String_Clone(const String* string)
{
    return MemClone("strings", string);
}

String* String_New(const tchar* string, int32 length)
{
    if( length <= 0 )
    {
        length = String_CalcLength(string);
    }

    length = ((length>>4) +1) << 4;

    String* new_string = MemNewSize("strings", length);
    new_string->m_alloc_length = length;

    if( string )
    {
        String_Copy(new_string, string, 0);
    }

    return new_string;
}

error String_Del(String* string)
{
    MemDel("strings", string);
    return 0;
}

