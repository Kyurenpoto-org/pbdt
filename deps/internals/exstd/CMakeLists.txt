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
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
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
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/module/core.cppm
)

if(PBDT_BUILD_TESTS)
    add_subdirectory(tests)
endif()
