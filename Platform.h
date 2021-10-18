#pragma once

#if PLATFORM_WIN32

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#elif PLATFORM_PS5

#elif PLATFORM_NX

#elif PLATFORM_XSX

#elif PLATFORM_UNIX

#else

COMPILE_TIME_ASSERT(false, "");

#endif