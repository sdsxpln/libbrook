#ifndef BK_UTILS_H
#define BK_UTILS_H

#include <stddef.h>
#ifndef NDEBUG
#include "brook.h"
#endif

#ifdef _WIN32
# ifndef PATH_MAX
#  define PATH_MAX _MAX_PATH
# endif
# define realpath(n, r) _fullpath((r), (n), PATH_MAX)
#endif

#if defined(__MINGW32__) || defined(__ANDROID__)
#ifndef NDEBUG
BK_EXTERN
#endif
char *basename(const char *path);
#endif

/* Converts US-ASCII string to lower case. */
#ifndef NDEBUG
BK_EXTERN
#endif
void bk__toasciilower(char *str);

#endif /* BK_UTILS_H */
