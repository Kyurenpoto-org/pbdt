# SPDX-FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
#
# SPDX-License-Identifier: MIT

# For header based library
add_library(exstd INTERFACE)

target_compile_features(
    exstd
    INTERFACE cxx_std_23
)
target_include_directories(
    exstd
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

# For module based library
add_library(exstd-module STATIC)

target_compile_features(
    exstd-module
    PUBLIC cxx_std_23
)
target_link_libraries(
    exstd-module
    PRIVATE exstd
)
target_sources(
    exstd-module
    PUBLIC
    FILE_SET exstd_modules
    TYPE CXX_MODULES
    BASE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core.cppm
    ${CMAKE_CURRENT_SOURCE_DIR}/src/type-traits.cppm
)

if(PBDT_BUILD_TESTS)
    add_subdirectory(tests)
endif()
