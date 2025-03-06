# SPDX-FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
#
# SPDX-License-Identifier: MIT

# vcpkg
set(vcpkg-toolchain-path $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

if(DEFINED ENV{VCPKG_ROOT} AND EXISTS ${vcpkg-toolchain-path})
	include(${vcpkg-toolchain-path})
else()
	message(WARNING "VCPKG_ROOT not defined or Vcpkg toolchain file not found at ${vcpkg-toolchain-path}")
endif()

unset(vcpkg-toolchain-path)

# conan
if(DEFINED ENV{CONAN_HOME} AND EXISTS ${CMAKE_CURRENT_LIST_DIR}/conan.cmake)
	include(${CMAKE_CURRENT_LIST_DIR}/conan.cmake)
else()
	message(WARNING "CONAN_HOME not defined or toolchain file not found at ${CMAKE_CURRENT_LIST_DIR}/conan.cmake")
endif()
