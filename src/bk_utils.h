#ifndef BK_UTILS_H
#define BK_UTILS_H

#include <stddef.h>
#ifndef NDEBUG
#include "brook.h"
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

#ifndef NDEBUG
BK_EXTERN
#endif
int bk__open(const char *filename, int flags);

#ifndef NDEBUG
BK_EXTERN
#endif
int bk__close(int fd);

#endif /* BK_UTILS_H */
