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

# Add property based test suite for each header and module
# add_header_module_prop_suite(PROP <prop> DEP <dep> NAMESPACE <namespace> TARGETS <target> [<target> ...] [FIXTURES <fixture> [<fixture> ...]])
function(add_header_module_prop_suite)
    cmake_parse_arguments(
        SUITE
        ""
        "DEP;PROP;NAMESPACE"
        "FIXTURES;TARGETS"
        ${ARGN}
    )

    set(USAGE "Usage: add_header_module_prop_suite(PROP <prop> DEP <dep> NAMESPACE <namespace> TARGETS <target> [<target> ...] [FIXTURES <fixture> [<fixture> ...]])")

    if(NOT SUITE_PROP)
        message(FATAL_ERROR "Missing PROP argument")
        message(FATAL_ERROR $USAGE)
    endif()

    if(NOT SUITE_DEP)
        message(FATAL_ERROR "Missing DEP argument")
        message(FATAL_ERROR $USAGE)
    endif()

    if(NOT SUITE_NAMESPACE)
        message(FATAL_ERROR "Missing NAMESPACE argument")
        message(FATAL_ERROR $USAGE)
    endif()

    if(NOT SUITE_TARGETS)
        message(FATAL_ERROR "Missing TARGETS argument")
        message(FATAL_ERROR $USAGE)
    endif()

    function(add_suite SUITE_TYPE INNER_DEP)
        if(NOT TARGET ${SUITE_PROP}-${SUITE_TYPE})
            add_executable(${SUITE_PROP}-${SUITE_TYPE} "src/${SUITE_PROP}-${SUITE_TYPE}.cpp")

            target_compile_features(
                ${SUITE_PROP}-${SUITE_TYPE}
                PRIVATE cxx_std_23
            )
            target_link_libraries(
                ${SUITE_PROP}-${SUITE_TYPE}
                PRIVATE ${INNER_DEP}
            )
        endif()

        if(SUITE_FIXTURES)
            foreach(fixture IN LISTS SUITE_FIXTURES)
                target_link_libraries(
                    ${SUITE_PROP}-${SUITE_TYPE}
                    PRIVATE ${fixture}
                )
            endforeach()
        endif()

        foreach(target IN LISTS SUITE_TARGETS)
            add_test(
                NAME ${SUITE_NAMESPACE}.${target}.${SUITE_PROP}.${SUITE_TYPE}
                COMMAND ${SUITE_PROP}-${SUITE_TYPE} ${target}
            )
        endforeach()
    endfunction()

    add_suite(header ${SUITE_DEP})
    add_suite(module ${SUITE_DEP}-module)
endfunction()
