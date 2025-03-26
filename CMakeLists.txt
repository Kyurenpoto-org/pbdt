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
add_library(pbdt INTERFACE)

target_compile_features(
	pbdt
	INTERFACE cxx_std_23
)
target_link_libraries(
	pbdt
	INTERFACE exstd
)
target_include_directories(
	pbdt
	INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

# For module based library
add_library(pbdt-module STATIC)

target_compile_features(
	pbdt-module
	PUBLIC cxx_std_23
)
target_link_libraries(
	pbdt-module
	PRIVATE pbdt
	PRIVATE exstd-module
)
target_sources(
	pbdt-module
	PUBLIC
	FILE_SET pbdt_modules
	TYPE CXX_MODULES
	BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src
	FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/core.cppm
)

if(PBDT_BUILD_TESTS)
	add_subdirectory(tests)
endif()

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
	include(GNUInstallDirs)
	include(CMakePackageConfigHelpers)

	# Header files
	install(
		DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)

	install(
		DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/deps/internals/exstd/include/exstd
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
	)

	# Header based targets
	install(
		TARGETS pbdt exstd
		EXPORT ${PROJECT_NAME}-targets
	)

	install(
		EXPORT ${PROJECT_NAME}-targets
		FILE ${PROJECT_NAME}-targets.cmake
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
	)

	# Module based targets
	install(
		TARGETS pbdt-module exstd-module
		EXPORT ${PROJECT_NAME}-module-targets
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		FILE_SET pbdt_modules DESTINATION lib/cxx/miu/${PROJECT_NAME}
		FILE_SET exstd_modules DESTINATION lib/cxx/miu/${PROJECT_NAME}/exstd
	)

	install(
		EXPORT ${PROJECT_NAME}-module-targets
		FILE ${PROJECT_NAME}-module-targets.cmake
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
	)

	# Version config
	write_basic_package_version_file(
		${PROJECT_NAME}-config-version.cmake
		VERSION ${PROJECT_VERSION}
		COMPATIBILITY SameMajorVersion
		ARCH_INDEPENDENT
	)

	# CMake config
	file(WRITE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake.in [[
@PACKAGE_INIT@
include("${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}-targets.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}-module-targets.cmake")
	]])

	configure_package_config_file(
		"${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake.in"
		"${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
		INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
	)

	# pkg-config
	file(WRITE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc.in [[
prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=@CMAKE_INSTALL_PREFIX@
libdir=@CMAKE_INSTALL_PREFIX@/@CMAKE_INSTALL_LIBDIR@
includedir=@CMAKE_INSTALL_PREFIX@/@CMAKE_INSTALL_INCLUDEDIR@

Name: @PROJECT_NAME@
Description: @PROJECT_DESCRIPTION@
Version: @PROJECT_VERSION@
Libs.msvc: /LIBPATH:${libdir} @PROJECT_NAME@.lib
Libs.gcc: -L${libdir} -l@PROJECT_NAME@
Libs.clang: -L${libdir} -l@PROJECT_NAME@
Cflags.header.msvc: /I${includedir}
Cflags.header.gcc: -I${includedir}
Cflags.header.clang: -I${includedir}
Cflags.module.msvc: /experimental:module /reference ${includedir}/@PROJECT_NAME@.ifc
Cflags.module.gcc: -fmodules-ts -fmodule-file=${includedir}/@PROJECT_NAME@.gcm
Cflags.module.clang: -fmodules-ts -fmodule-file=${includedir}/@PROJECT_NAME@.pcm
	]])

	configure_file(
		"${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc.in"
		"${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc"
		@ONLY
	)

	install(
		FILES
		${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
		${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
	)
	install(
		FILES
		${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig/${PROJECT_NAME}
	)
endif()
