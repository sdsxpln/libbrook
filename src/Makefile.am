AM_CPPFLAGS = \
  -I$(top_srcdir)/include \
  -I$(top_srcdir)/uthash/src \
  -I$(top_srcdir)/libmicrohttpd/src/include
lib_LTLIBRARIES = libbrook.la
libbrook_la_SOURCES = \
  b4r_hs.c \
  b4r_hs.h \
  b4r_httpsrv_auth.c \
  b4r_httpsrv_auth.h \
  b4r_httpsrv.c \
  b4r_httpsrv_cfg.c \
  b4r_httpsrv_cfg.h \
  b4r_httpsrv.h \
  b4r_httpsrv_req.c \
  b4r_httpsrv_req.h \
  b4r_httpsrv_req_uplds.c \
  b4r_httpsrv_req_uplds.h \
  b4r_httpsrv_res.c \
  b4r_httpsrv_res_cache.c \
  b4r_httpsrv_res_cache.h \
  b4r_httpsrv_res.h \
  b4r_httpsrv_strs.h \
  b4r_macros.h \
  b4r_router.c \
  b4r_router.h \
  b4r_utils.c \
  b4r_utils.h \
  md5.c \
  md5.h
libbrook_la_CFLAGS = $(B4R_CFLAGS)
libbrook_la_LDFLAGS = $(B4R_LDFLAGS)
libbrook_la_LIBADD = \
  $(top_srcdir)/libmicrohttpd/src/microhttpd/libmicrohttpd.la