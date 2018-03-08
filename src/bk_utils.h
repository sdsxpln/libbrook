#ifndef BK_UTILS_H
#define BK_UTILS_H

#include <stddef.h>

#ifdef __MINGW32__
/* Returns a pointer to a new string duplicate from \p str. */
char *bk__strndup(const char *str, size_t len);
#else
#define bk__strndup strndup
#endif

/* Converts US-ASCII string to lower case. */
int bk__toasciilower(char *str, size_t len);

#endif /* BK_UTILS_H */
