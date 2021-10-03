#pragma once

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

// 0 info, 1 warn, 2 error
error   Log(int32 type, const tchar* format, ...);
void    Engine_Debug_Break();

#if CONFIG_DEBUG
#define Assert(must_true_condition, msg, ...) do{ \
    if (!(must_true_condition)) { \
        if (msg) { Log(2, msg, ##__VA_ARGS__); } \
        else { Log(2, "Error at file %s, function %s, line %u: %s", __FILE__, __FUNCTION__, __LINE__, #must_true_condition); } \
    } \
}while(0)

#define StaticAssert(must_be_true_condition, msg)   static tchar[(must_be_true_condition) ? 0 : -1]
#else
#define Assert(must_be_true_condition, msg).
#define StaticAssert(must_be_true_condition, msg)
#endif