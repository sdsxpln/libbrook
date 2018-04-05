#ifndef BK_UTILS_H
#define BK_UTILS_H

#include <stddef.h>
#ifndef NDEBUG
#include "brook.h"
#endif

/* Converts US-ASCII string to lower case. */
#ifndef NDEBUG
BK_EXTERN
#endif
void bk__toasciilower(char *str);

#if defined(__MINGW32__) || defined(__ANDROID__)
#ifndef NDEBUG
BK_EXTERN
#endif
char *basename(const char *path);
#endif

#ifndef NDEBUG
BK_EXTERN
#endif
int bk_open(const char *filename, int flags);

#ifndef NDEBUG
BK_EXTERN
#endif
int bk_close(int fd);

#endif /* BK_UTILS_H */
