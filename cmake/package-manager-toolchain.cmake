# SPDX-FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
#
# SPDX-License-Identifier: MIT

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

# vcpkg
if(DEFINED ENV{VCPKG_ROOT})
	set(vcpkg_toolchain_path $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

	if(EXISTS ${vcpkg_toolchain_path})
		include(${vcpkg_toolchain_path})
	else()
		message(WARNING "Vcpkg toolchain file not found at ${vcpkg_toolchain_path}. Ensure vcpkg is properly integrated and installed correctly.")
	endif()

	unset(vcpkg_toolchain_path)
else()
	message(WARNING "VCPKG_ROOT not defined. Please set the VCPKG_ROOT environment variable to the location of your Vcpkg installation.")
endif()

# conan
if(DEFINED ENV{CONAN_HOME})
	include(${CMAKE_CURRENT_LIST_DIR}/conan.cmake)
else()
	message(WARNING "CONAN_HOME not defined. Please set the CONAN_HOME environment variable to the location of your Conan installation.")
endif()
