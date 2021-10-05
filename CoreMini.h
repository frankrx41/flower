#pragma once

/*
Code Style

Start with _ means not want be call
*/

#if PLATFORM_WIN32
#define KEEP_LEGACY_TYPE 1
#endif

// New key word
typedef int             int32;
typedef unsigned int    uint32;
typedef unsigned char   byte;

typedef signed char     tchar;
typedef void *          tptr;
typedef uint32          tsize;
typedef int32           error;
typedef uint32          crc32;

#define NULL            ( 0 )

// boolean
typedef tchar bool;
#define true    ( 1 )
#define false   ( 0 )

// Do not use those key word
// For multi-platform
#if !KEEP_LEGACY_TYPE
#define int
#define char
#endif

// Useful function


#if CONFIG_DEBUG

// 0 info, 1 warn, 2 error
error   Log                 (int32 type, const tchar* format, ...);
bool    Str_IsEmpty         (const tchar* str);
void    Engine_Debug_Break  ();

#define Assert(must_true_condition, msg, ...) do{ \
    if(!(must_true_condition)) { \
        if (!(Str_IsEmpty(msg))) { Log(2, msg, ##__VA_ARGS__); } \
        else { Log(2, "Error at file %s, function %s, line %u: %s\n", __FILE__, __FUNCTION__, __LINE__, #must_true_condition); } \
        Engine_Debug_Break(); \
    } \
}while(0)

#define StaticAssert(must_be_true_condition, msg)   static tchar[(must_be_true_condition) ? 0 : -1]
#else

#define Log(...)
#define Assert(must_be_true_condition, msg).
#define StaticAssert(must_be_true_condition, msg)

#endif


#define INT32_MAX       2147483647i32
#define UINT32_MAX      0xffffffffui32
#define offsetof(s, m)  (tsize)&(((s *)0)->m)

#define MAX(a, b)       ((a) > (b) ? (a) : (b))
#define MIN(a, b)       ((a) < (b) ? (a) : (b))

#define _MACRO_REMOVE_BRACKETS(...)  __VA_ARGS__
#define MACRO_REMOVE_BRACKETS(...)  _MACRO_REMOVE_BRACKETS##__VA_ARGS__
#define _MACRO_TOSTR(...) #__VA_ARGS__
#define MACRO_TOSTR(...) _MACRO_TOSTR(__VA_ARGS__)
#define _MACRO_CONNNECT(a,b)    a##b
#define MACRO_CONNNECT(a,b)     _MACRO_CONNNECT(a,b)

