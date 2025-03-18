# SPDX-FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
#
# SPDX-License-Identifier: MIT

# fixtures
add_library(bdd-fixture INTERFACE)

target_compile_features(
    bdd-fixture
    INTERFACE cxx_std_23
)
target_include_directories(
    bdd-fixture
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(
    bdd-fixture
    INTERFACE pbdt
    INTERFACE pbdt-module
)

# tests
add_executable(test-context "src/test-context.cpp")

target_compile_features(
    test-context
    PRIVATE cxx_std_23
)
target_link_libraries(
    test-context
    PRIVATE bdd-fixture
    PRIVATE exstd
    PRIVATE pbdt
    PRIVATE pbdt-module
)

add_test(
    NAME pbdt.bdd.test-context
    COMMAND test-context
)

add_executable(given-component "src/given-component.cpp")

target_compile_features(
    given-component
    PRIVATE cxx_std_23
)
target_link_libraries(
    given-component
    PRIVATE bdd-fixture
    PRIVATE exstd
    PRIVATE pbdt
    PRIVATE pbdt-module
)

add_test(
    NAME pbdt.bdd.given-component
    COMMAND given-component
)

add_executable(when-component "src/when-component.cpp")

target_compile_features(
    when-component
    PRIVATE cxx_std_23
)
target_link_libraries(
    when-component
    PRIVATE bdd-fixture
    PRIVATE exstd
    PRIVATE pbdt
    PRIVATE pbdt-module
)

add_test(
    NAME pbdt.bdd.when-component
    COMMAND when-component
)

add_executable(then-component "src/then-component.cpp")

target_compile_features(
    then-component
    PRIVATE cxx_std_23
)
target_link_libraries(
    then-component
    PRIVATE bdd-fixture
    PRIVATE exstd
    PRIVATE pbdt
    PRIVATE pbdt-module
)

add_test(
    NAME pbdt.bdd.then-component
    COMMAND then-component
)

add_executable(scenario "src/scenario.cpp")

target_compile_features(
    scenario
    PRIVATE cxx_std_23
)
target_link_libraries(
    scenario
    PRIVATE bdd-fixture
    PRIVATE exstd
    PRIVATE pbdt
    PRIVATE pbdt-module
)

add_test(
    NAME pbdt.bdd.scenario
    COMMAND scenario
)
