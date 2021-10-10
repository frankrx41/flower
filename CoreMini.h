#pragma once

#define NO_COMPILE_TIME_CHECK   0
#define KEEP_LEGACY_TYPE        0
#define X_64                    0
#define X_32                    0

#if PLATFORM_WIN32
#undef  KEEP_LEGACY_TYPE
#define KEEP_LEGACY_TYPE        1
#define X_32                    1
#endif

#include <stdarg.h>

#undef va_arg
#undef va_start
#undef va_end
#define va_start(ap, type)  (ap = (va_list)ADDRESS_OF(type) + INT32_SIZEOF(type))
#define va_end(ap)          (ap = (va_list)NULL)
#define va_arg(ap,t)        (*(t *)((ap += INT32_SIZEOF(t)) - INT32_SIZEOF(t)))

/*
Code Style

Start with '_' means not want be call

Variable list:
XXXManager* local_name Event/Scene/Actor/String... others

*/

// New key word
typedef int                 int32;
typedef unsigned int        uint32;
typedef unsigned char       byte;
typedef char*               va_list;
typedef long long           int64;
typedef unsigned long long  uint64;

typedef signed char     tchar;
typedef void*           tptr;
typedef uint32          crc32;

#if X_64
typedef uint64          tsize;

#elif X_32
typedef uint32          tsize;

#else
#error "Unknow platfrom!"

#endif


#undef NULL
#define NULL            ( 0 )

// boolean
typedef tchar bool;
#define true    ( 1 )
#define false   ( 0 )

#if KEEP_LEGACY_TYPE
#undef int
#undef char
#else
// Do not use those key word
// For multi-platform
#define int
#define char
#endif


#if CONFIG_DEBUG || CONFIG_RELEASE

// 0 info, 1 warn, 2 error
void    Engine_Debug_Log        (int32 type, const tchar* format, ...);
bool    Str_IsEmpty             (const tchar* str);
void    Engine_Debug_Break      ();
void    Engine_Profile_Memory   ();
#define Log                     Engine_Debug_Log

#define Assert(must_true_condition, msg, ...) do{ \
    if(!(must_true_condition)) { \
        if (!(Str_IsEmpty(msg))) { Log(2, msg, ##__VA_ARGS__); } \
        else { Log(2, "Error at file %s, function %s, line %u: %s\n", __FILE__, __FUNCTION__, __LINE__, #must_true_condition); } \
        Engine_Debug_Break(); \
    } \
}while(0)
#else

#define Log(...)
#define Assert(must_be_true_condition, msg)
#define Engine_Profile_Memory()

#endif


#define INT32_MAX       2147483647i32
#define UINT32_MAX      0xffffffffui32

#define OFFSET_OF(s, m) (tsize)&(((s *)0)->m)

#define MAX(a, b)       ((a) > (b) ? (a) : (b))
#define MIN(a, b)       ((a) < (b) ? (a) : (b))

#define _MACRO_REMOVE_BRACKETS(...)  __VA_ARGS__
#define MACRO_REMOVE_BRACKETS(...)  _MACRO_REMOVE_BRACKETS##__VA_ARGS__
#define _MACRO_TOSTR(...) #__VA_ARGS__
#define MACRO_TOSTR(...) _MACRO_TOSTR(__VA_ARGS__)
#define _MACRO_CONNNECT(a,b)    a##b
#define MACRO_CONNNECT(a,b)     _MACRO_CONNNECT(a,b)

#define ADDRESS_OF(v)       (&(v))
#define INT32_SIZEOF(n)     ((sizeof(n) + sizeof(int32) - 1) & ~(sizeof(int32) - 1))

#if NO_COMPILE_TIME_CHECK
#define COMPILE_TIME_ASSERT(...)
#define COMPILE_TIME_REQUIRE(...)
#else
#define COMPILE_TIME_ASSERT(must_be_true_condition, msg)    typedef int32 static_assertion[(must_be_true_condition) ? 1 : -1]
#define COMPILE_TIME_REQUIRE(defined_header_file)           COMPILE_TIME_ASSERT(MACRO_CONNNECT(defined_header_file, _IS_DEFINED), "")
#endif
