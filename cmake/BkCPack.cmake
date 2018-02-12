#    _____   _____    _____   _____   _   __
#   |  _  \ |  _  \  /  _  \ /  _  \ | | / /
#   | |_) | | |_) |  | | | | | | | | | |/ /
#   |  _ <  |  _ <   | | | | | | | | |   (
#   | |_) | | | \ \  | |_| | | |_| | | |\ \
#   |_____/ |_|  \_\ \_____/ \_____/ |_| \_\
#
#   –– a small library which helps you write quickly REST APIs.
#
# Copyright (c) 2012-2018 Silvio Clecio <silvioprog@gmail.com>
#
# This file is part of Brook library.
#
# Brook library is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Brook library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with Brook library.  If not, see <http://www.gnu.org/licenses/>.
#

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "The Brook library team")
set(CPACK_PACKAGE_VERSION_MAJOR ${BK_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${BK_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${BK_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${CPACK_PACKAGE_NAME} ${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_FILE_NAME "lib${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
if (NOT CPACK_GENERATOR)
    set(CPACK_GENERATOR "TGZ;ZIP")
endif ()

set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}")
if (NOT CPACK_SOURCE_GENERATOR)
    set(CPACK_SOURCE_GENERATOR ${CPACK_GENERATOR})
endif ()
if (NOT CPACK_SOURCE_IGNORE_FILES)
    set(CPACK_SOURCE_IGNORE_FILES
            "${CMAKE_BINARY_DIR}/"
            "/.idea/"
            "/.git/"
            ".gitignore"
            "cmake-build-*")
endif ()

add_custom_target(dist
        COMMAND ${CMAKE_MAKE_PROGRAM} package_source)

include(CPack)