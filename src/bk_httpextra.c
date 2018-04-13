#include <stdbool.h>
#include "microhttpd.h"
#include "brook.h"

ssize_t bk_httpread_end(bool err) {
    return
#ifdef __ANDROID__
        (ssize_t)
#endif
            (err ? MHD_CONTENT_READER_END_WITH_ERROR : MHD_CONTENT_READER_END_OF_STREAM);
}
