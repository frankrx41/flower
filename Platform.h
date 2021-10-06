#pragma once

#if PLATFORM_WIN32
#define KEEP_LEGACY_TYPE 1
#endif

#if KEEP_LEGACY_TYPE
#undef int
#undef char
#endif

