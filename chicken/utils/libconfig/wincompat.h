#ifndef __wincompat_h
#define __wincompat_h

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

#ifdef _MSC_VER
#pragma warning (disable: 4996)
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define snprintf  _snprintf

#ifndef __MINGW32__
#define atoll     _atoi64
#define strtoull  _strtoui64
#endif /* __MINGW32__ */

#endif

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) \
     || defined(__MINGW32__))

/* Why does gcc on MinGW use the Visual C++ style format directives
 * for 64-bit integers? Inquiring minds want to know....
 */

#define INT64_FMT "%I64d"
#define UINT64_FMT "%I64u"

#define INT64_HEX_FMT "%I64X"

#define FILE_SEPARATOR "\\"

#else /* defined(WIN32) || defined(__MINGW32__) */

#define INT64_FMT "%lld"
#define UINT64_FMT "%llu"

#define INT64_HEX_FMT "%llX"

#define FILE_SEPARATOR "/"

#endif /* defined(WIN32) || defined(__MINGW32__) */

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) \
  && ! defined(__MINGW32__)

#define INT64_CONST(I)  (I ## i64)
#define UINT64_CONST(I) (I ## Ui64)

#ifndef INT32_MAX
#define INT32_MAX (2147483647)
#endif

#ifndef INT32_MIN
#define INT32_MIN (-2147483647-1)
#endif

#else /* defined(WIN32) && ! defined(__MINGW32__) */

#define INT64_CONST(I)  (I ## LL)
#define UINT64_CONST(I) (I ## ULL)

#endif /* defined(WIN32) && ! defined(__MINGW32__) */

#endif /* __wincompat_h */
