# SPDX-FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
#
# SPDX-License-Identifier: MIT

add_library(example INTERFACE)

target_compile_features(
    example
    INTERFACE cxx_std_23
)
target_include_directories(
    example
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)
