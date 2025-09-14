# SPDX-FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
#
# SPDX-License-Identifier: MIT

# fixtures
add_library(pbdt-base-fixture INTERFACE)

target_compile_features(
    pbdt-base-fixture
    INTERFACE cxx_std_23
)
target_include_directories(
    pbdt-base-fixture
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(given-component-fixture STATIC)

target_compile_features(
    given-component-fixture
    PUBLIC cxx_std_23
)
target_link_libraries(
    given-component-fixture
    PUBLIC pbdt-base-fixture
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
target_link_libraries(
    when-component-fixture
    PUBLIC pbdt-base-fixture
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
target_link_libraries(
    then-component-fixture
    PUBLIC pbdt-base-fixture
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
target_link_libraries(
    runnable-scenario-fixture
    PUBLIC pbdt-base-fixture
)
target_sources(
    runnable-scenario-fixture
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/instanciation/runnable-combination.cpp
)

# Unit Tests
# Given: Set Category
# When: Set Category that has product between any two objects
# Then: 2-Category that has terminal object and each 1-Hom-set has product between any two morphisms in it
add_header_module_prop_suite(
    PROP idempotent
    DEP pbdt
    NAMESPACE pbdt.unit
    TARGETS given when then
    FIXTURES given-component-fixture when-component-fixture then-component-fixture
)
add_header_module_prop_suite(
    PROP associative
    DEP pbdt
    NAMESPACE pbdt.unit
    TARGETS given when then expectation-context
    FIXTURES given-component-fixture when-component-fixture then-component-fixture
)
add_header_module_prop_suite(
    PROP modeling
    DEP pbdt
    NAMESPACE pbdt.unit
    TARGETS runnable-scenario
    FIXTURES runnable-scenario-fixture
)
add_header_module_prop_suite(
    PROP inclusive
    DEP pbdt
    NAMESPACE pbdt.unit
    TARGETS callable-target callable-property event-countable
    FIXTURES pbdt-base-fixture
)
add_header_module_prop_suite(
    PROP homomorphic
    DEP pbdt
    NAMESPACE pbdt.unit
    TARGETS event-countable expectation-context
    FIXTURES pbdt-base-fixture
)

# Integration Tests
add_header_module_prop_suite(
    PROP projective
    DEP pbdt
    NAMESPACE pbdt.integration
    TARGETS runnable-scenario-with-given-component runnable-scenario-with-when-component
    FIXTURES runnable-scenario-fixture
)
add_header_module_prop_suite(
    PROP homomorphic
    DEP pbdt
    NAMESPACE pbdt.integration
    TARGETS runnable-scenario-with-then-component
    FIXTURES runnable-scenario-fixture
)
add_header_module_prop_suite(
    PROP mutual-exclusive
    DEP pbdt
    NAMESPACE pbdt.integration
    TARGETS range-domain-with-callable-target
    FIXTURES pbdt-base-fixture
)
