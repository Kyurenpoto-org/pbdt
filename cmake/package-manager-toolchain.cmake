# SPDX-FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
#
# SPDX-License-Identifier: MIT

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

# vcpkg
if(DEFINED ENV{VCPKG_ROOT})
	include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
endif()

# conan
execute_process(
	COMMAND conan --version
	RESULT_VARIABLE conan_result
	OUTPUT_VARIABLE conan_output
	ERROR_VARIABLE conan_error
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(conan_result OR NOT conan_output MATCHES "^Conan version [0-9]+\\.[0-9]+\\.[0-9]+.*$")
	message(FATAL_ERROR "Conan is not installed or the version string format is unexpected. Check your Conan installation. See https://docs.conan.io/2/installation.html for more information.")
else()
	include(${CMAKE_CURRENT_LIST_DIR}/conan.cmake)
endif()
