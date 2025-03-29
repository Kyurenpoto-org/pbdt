# SPDX-FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
#
# SPDX-License-Identifier: MIT

# tests
add_executable(example-runnable-scenario "src/runnable-scenario.cpp")

target_compile_features(
    example-runnable-scenario
    PRIVATE cxx_std_23
)
target_link_libraries(
    example-runnable-scenario
    PRIVATE pbdt
)

add_test(
    NAME pbdt.example.runnable-scenario
    COMMAND example-runnable-scenario
)
