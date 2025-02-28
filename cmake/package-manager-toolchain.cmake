# SPDX-FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
#
# SPDX-License-Identifier: MIT

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

# vcpkg
if(DEFINED ENV{VCPKG_ROOT})
	include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
endif()

# conan
if(DEFINED ENV{CONAN_HOME})
	include(${CMAKE_CURRENT_LIST_DIR}/conan.cmake)
endif()
