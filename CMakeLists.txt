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

# modules
add_subdirectory("deps/internals")
