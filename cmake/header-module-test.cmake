# SPDX-FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
#
# SPDX-License-Identifier: MIT

function(add_header_module_tests dep fixture test_target test_name)
    add_executable(${test_target}-header "src/${test_target}-header.cpp")

    target_compile_features(
        ${test_target}-header
        PRIVATE cxx_std_23
    )
    target_link_libraries(
        ${test_target}-header
        PRIVATE ${dep}
        PRIVATE ${fixture}
    )

    add_test(
        NAME ${test_name}.header
        COMMAND ${test_target}-header
    )

    add_executable(${test_target}-module "src/${test_target}-module.cpp")

    target_compile_features(
        ${test_target}-module
        PRIVATE cxx_std_23
    )
    target_link_libraries(
        ${test_target}-module
        PRIVATE ${dep}-module
        PRIVATE ${fixture}
    )

    add_test(
        NAME ${test_name}.module
        COMMAND ${test_target}-module
    )
endfunction()
