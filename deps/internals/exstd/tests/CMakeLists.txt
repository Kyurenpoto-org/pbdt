# SPDX-FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
#
# SPDX-License-Identifier: MIT

# fixtures
add_library(type-assertion INTERFACE)

target_compile_features(
    type-assertion
    INTERFACE cxx_std_23
)
target_include_directories(
    type-assertion
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# tests
add_header_module_tests("exstd" "type-assertion" "callable-concept")
add_header_module_tests("exstd" "type-assertion" "simple-callable")
