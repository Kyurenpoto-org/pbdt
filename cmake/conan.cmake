﻿# SPDX-FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
#
# SPDX-License-Identifier: MIT

# Check if default profile exists
execute_process(
    COMMAND conan profile list
    OUTPUT_VARIABLE conan_profiles
)

if(NOT conan_profiles MATCHES "default")
    # Overwrite default profile if it does not exist
    execute_process(
        COMMAND
        conan profile detect --force
    )
endif()

# Install conan packages
execute_process(
    COMMAND
    conan install "${CMAKE_SOURCE_DIR}"
    --settings:all build_type=${CMAKE_BUILD_TYPE}
    --settings:all compiler.cppstd=23
    --conf:all tools.cmake.cmaketoolchain:generator=Ninja
    --output-folder=${CMAKE_BINARY_DIR}/conan_installed
    --build=missing
    --deployer=full_deploy
)

# Include toolchain
set(conan-toolchain-path ${CMAKE_BINARY_DIR}/conan_installed/build/${CMAKE_BUILD_TYPE}/generators/conan_toolchain.cmake)

if(EXISTS ${conan-toolchain-path})
    include(${conan-toolchain-path})
else()
    message(WARNING "Conan toolchain file not found at ${conan-toolchain-path}")
endif()

unset(conan-toolchain-path)
