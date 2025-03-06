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
		message(FATAL_ERROR "Vcpkg toolchain file not found at ${vcpkg_toolchain_path}. Ensure vcpkg is properly integrated and installed correctly. See https://learn.microsoft.com/en-us/vcpkg/get_started/get-started#1---set-up-vcpkg for more information.")
	endif()

	unset(vcpkg_toolchain_path)
else()
	message(FATAL_ERROR "VCPKG_ROOT not defined. Please set the VCPKG_ROOT environment variable to the location of your Vcpkg installation. You can set it in your system environment variables. See https://learn.microsoft.com/en-us/vcpkg/get_started/get-started#2---set-up-the-project for more information.")
endif()

# conan
if(DEFINED ENV{CONAN_HOME})
	include(${CMAKE_CURRENT_LIST_DIR}/conan.cmake)
else()
	message(FATAL_ERROR "CONAN_HOME not defined. Please set the CONAN_HOME environment variable to the location of your Conan installation. You can set it in your system environment variables. See https://docs.conan.io/2/reference/environment.html#conan-home for more information.")
endif()
