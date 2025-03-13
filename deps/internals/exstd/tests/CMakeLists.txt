# SPDX-FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
#
# SPDX-License-Identifier: MIT

# tests
add_executable(callable-traits "src/callable-traits.cpp")

target_compile_features(
    callable-traits
    PRIVATE cxx_std_23
)
target_link_libraries(
    callable-traits
    PRIVATE exstd
    PRIVATE exstd-module
)

add_test(
    NAME exstd.callable-traits
    COMMAND callable-traits
)
