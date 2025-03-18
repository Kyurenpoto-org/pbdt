# SPDX-FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.28.2)

project(
	"pbdt"
	VERSION
	0.1.0
	DESCRIPTION
	"Property-Behavior Driven Test for C++"
	LANGUAGES
	CXX
)

if(POLICY CMP0141)
	cmake_policy(SET CMP0141 NEW)
	set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

option(PBDT_BUILD_TESTS "Build the tests" OFF)

# packages
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/package-manager-toolchain.cmake)

list(PREPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/prefix/)

if(PBDT_BUILD_TESTS)
	enable_testing()
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/header-module-test.cmake)
endif()

# macos config
if(APPLE)
	execute_process(
		COMMAND xcrun --sdk macosx --show-sdk-path
		RESULT_VARIABLE sysroot_result
		OUTPUT_VARIABLE sysroot_output
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	if(sysroot_result)
		message(FATAL_ERROR "Failed to get macOS SDK path. Command: xcrun --sdk macosx --show-sdk-path, Error code: ${sysroot_result}")
	elseif(NOT sysroot_output)
		message(FATAL_ERROR "macOS SDK path is empty. Check your Xcode installation and ensure command line tools are installed (`xcode-select --install`).")
	else()
		set(CMAKE_OSX_SYSROOT ${sysroot_output})
	endif()
endif()

# clang config
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	find_program(CLANG_SCAN_DEPS_EXECUTABLE NAMES clang-scan-deps)

	if(NOT CLANG_SCAN_DEPS_EXECUTABLE)
		message(FATAL_ERROR "clang-scan-deps not found. Ensure that clang-scan-deps is installed and available in your PATH. You can install it via your package manager (e.g., brew install llvm on macOS, apt install clang-tools on Debian/Ubuntu) or download it from the official Clang website.")
	else()
		set(CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS ${CLANG_SCAN_DEPS_EXECUTABLE})
	endif()

	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/opt/homebrew/opt/llvm/lib/c++ -Wl,-rpath,/opt/homebrew/opt/llvm/lib/c++")
endif()

# modules
add_subdirectory("deps/internals")

# For header based library
add_library(prop-pp INTERFACE)

target_compile_features(
	prop-pp
	INTERFACE cxx_std_23
)
target_link_libraries(
	prop-pp
	INTERFACE exstd
)
target_include_directories(
	prop-pp
	INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# For module based library
add_library(prop-pp-module STATIC)

target_compile_features(
	prop-pp-module
	PUBLIC cxx_std_23
)
target_link_libraries(
	prop-pp-module
	PRIVATE prop-pp
	PRIVATE exstd-module
)
target_sources(
	prop-pp-module
	PUBLIC
	FILE_SET prop_pp_modules
	TYPE CXX_MODULES
	FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/module/core.cppm
)

if(PBDT_BUILD_TESTS)
	add_subdirectory(tests)
endif()
