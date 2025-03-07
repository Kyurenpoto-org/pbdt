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
execute_process(
	COMMAND conan --version
	RESULT_VARIABLE conan_result
	OUTPUT_VARIABLE conan_output
	ERROR_VARIABLE conan_error
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(conan_result OR NOT conan_output MATCHES "^Conan version [0-9]+\\.[0-9]+\\.[0-9]+")
	message(FATAL_ERROR "Conan is not installed. Check your Conan installation. See https://docs.conan.io/2/installation.html for more information.")
else()
	include(${CMAKE_CURRENT_LIST_DIR}/conan.cmake)
endif()
