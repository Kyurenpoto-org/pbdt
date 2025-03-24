# SPDX-FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
#
# SPDX-License-Identifier: MIT

# fixtures
add_library(exstd-fixture INTERFACE)

target_compile_features(
    exstd-fixture
    INTERFACE cxx_std_23
)
target_include_directories(
    exstd-fixture
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# tests
add_header_module_tests("exstd" "exstd-fixture" "callable-concept" "exstd.callable-concept")
add_header_module_tests("exstd" "exstd-fixture" "simple-callable" "exstd.simple-callable")
