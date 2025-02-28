# SPDX-FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.28)

project(
	"your-project-name"
	VERSION
	0.1.0
	DESCRIPTION
	"project description"
	LANGUAGES
	CXX
)

if(POLICY CMP0141)
	cmake_policy(SET CMP0141 NEW)
	set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# packages
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/package-manager-toolchain.cmake)

list(PREPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/prefix/)

# modules
add_subdirectory("example")

# add_subdirectory("your-module-name")

# tests
enable_testing()

add_subdirectory("tests/example")

# add_subdirectory("tests/your-module-name")
