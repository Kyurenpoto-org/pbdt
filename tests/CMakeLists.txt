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
)

add_library(given-component-fixture STATIC)

target_compile_features(
    given-component-fixture
    PUBLIC cxx_std_23
)
target_include_directories(
    given-component-fixture
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_sources(
    given-component-fixture
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/composable-callable.cpp
)

add_library(when-component-fixture STATIC)

target_compile_features(
    when-component-fixture
    PUBLIC cxx_std_23
)
target_include_directories(
    when-component-fixture
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_sources(
    when-component-fixture
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/productable-container.cpp
)

# tests
add_header_module_tests("pbdt" "bdd-fixture" "test-context" "")

# Set Category
add_executable(given-component "src/given-component.cpp")

target_compile_features(
    given-component
    PRIVATE cxx_std_23
)
target_link_libraries(
    given-component
    PRIVATE pbdt
    PRIVATE given-component-fixture
)

add_test(
    NAME pbdt.bdd.given-component
    COMMAND given-component
)

# Set Category that has product between any two objects
add_header_module_tests("pbdt" "when-component-fixture" "when-component-idempotent" "pbdt.when-component.idempotent")
add_header_module_tests("pbdt" "when-component-fixture" "when-component-associative" "pbdt.when-component.associative")

# 2-Category that has terminal object and each 1-Hom-set has product between any two morphisms in it
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
