#ifndef BK_UTILS_H
#define BK_UTILS_H

#include <stddef.h>

/**
 * Converts US-ASCII string to lower case. All non-US-ASCII or lower case characters are kept unmodified.
 * \param str[in,out] US-ASCII string to be converted to lower case.
 * \param len Length of the US-ASCII string to be converted to lower case.
 * \return 0 - Success.
 * \retval -EINVAL - Invalid argument.
 */
int bk__toasciilower(char *str, size_t len);

#endif /* BK_UTILS_H */