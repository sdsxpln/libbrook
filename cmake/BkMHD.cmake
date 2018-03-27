set(MHD_NAME "libmicrohttpd")

set(MHD_URL "https://ftp.gnu.org/gnu/libmicrohttpd/libmicrohttpd-0.9.59.tar.gz")

set(MHD_SHA256 "9b9ccd7d0b11b0e179f1f58dc2caa3e0c62c8609e1e1dc7dcaadf941b67d923c")

set(MHD_DIR "${CMAKE_BINARY_DIR}/libmicrohttpd")

set(MHD_INCLUDE_DIR "${MHD_DIR}/include")

set(MHD_STATIC_LIB "${MHD_DIR}/lib/${MHD_NAME}.a")

set(MHD_OPTIONS
        "--enable-static=yes"
        "--enable-shared=no"
        "--enable-https=no"
        "--enable-asserts=no"
        "--enable-coverage=no"
        "--disable-doc"
        "--disable-examples"
        "--disable-curl")

#TODO: ExternalProject_Add()
#TODO: include MHD