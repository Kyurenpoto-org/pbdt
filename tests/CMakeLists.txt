# SPDX-FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
#
# SPDX-License-Identifier: MIT

# fixtures
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
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/instanciation/composable-callable.cpp
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
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/instanciation/productable-container.cpp
)

add_library(then-component-fixture STATIC)

target_compile_features(
    then-component-fixture
    PUBLIC cxx_std_23
)
target_include_directories(
    then-component-fixture
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_sources(
    then-component-fixture
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/instanciation/foldable-callable.cpp
)

add_library(runnable-scenario-fixture STATIC)

target_compile_features(
    runnable-scenario-fixture
    PUBLIC cxx_std_23
)
target_include_directories(
    runnable-scenario-fixture
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_sources(
    runnable-scenario-fixture
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/instanciation/runnable-combination.cpp
)

# tests
# Given Component: Set Category
add_header_module_tests("pbdt" "given-component-fixture" "given-component-idempotent" "pbdt.unit.given-component.idempotent")
add_header_module_tests("pbdt" "given-component-fixture" "given-component-associative" "pbdt.unit.given-component.associative")

# When Component: Set Category that has product between any two objects
add_header_module_tests("pbdt" "when-component-fixture" "when-component-idempotent" "pbdt.unit.when-component.idempotent")
add_header_module_tests("pbdt" "when-component-fixture" "when-component-associative" "pbdt.unit.when-component.associative")

# Then Component: 2-Category that has terminal object and each 1-Hom-set has product between any two morphisms in it
add_header_module_tests("pbdt" "then-component-fixture" "then-component-idempotent" "pbdt.unit.then-component.idempotent")
add_header_module_tests("pbdt" "then-component-fixture" "then-component-associative" "pbdt.unit.then-component.associative")

# Runnable Scenario: Modeling
add_header_module_tests("pbdt" "runnable-scenario-fixture" "runnable-scenario-model" "pbdt.unit.runnable-scenario.model")
