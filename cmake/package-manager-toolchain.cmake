# SPDX-FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
#
# SPDX-License-Identifier: MIT

# vcpkg
if(DEFINED ENV{VCPKG_ROOT} AND EXISTS $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
	include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
endif()

# conan
if(DEFINED ENV{CONAN_HOME} AND EXISTS ${CMAKE_CURRENT_LIST_DIR}/conan.cmake)
	include(${CMAKE_CURRENT_LIST_DIR}/conan.cmake)
endif()
