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
add_executable(callable-concept-header "src/callable-concept-header.cpp")

target_compile_features(
    callable-concept-header
    PRIVATE cxx_std_23
)
target_link_libraries(
    callable-concept-header
    PRIVATE exstd
    PRIVATE type-assertion
)

add_test(
    NAME exstd.callable-concept.header
    COMMAND callable-concept-header
)

add_executable(simple-callable-header "src/simple-callable-header.cpp")

target_compile_features(
    simple-callable-header
    PRIVATE cxx_std_23
)
target_link_libraries(
    simple-callable-header
    PRIVATE exstd
    PRIVATE type-assertion
)

add_test(
    NAME exstd.simple-callable.header
    COMMAND simple-callable-header
)
