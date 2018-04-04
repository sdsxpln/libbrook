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

#ifdef __MINGW32__
#ifndef NDEBUG
BK_EXTERN
#endif
char *basename(const char *path);
#endif

#endif /* BK_UTILS_H */
