# SPDX-FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
#
# SPDX-License-Identifier: MIT

find_package(ut REQUIRED)
find_package(frozen REQUIRED)

# tests
add_executable(example-test "src/example-test.cpp")

target_compile_features(
    example-test
    PRIVATE cxx_std_23
)
target_link_libraries(
    example-test
    PRIVATE example
    PRIVATE Boost::ut
    PRIVATE frozen::frozen
)

add_test(
    NAME example.example-test
    COMMAND example-test
)
