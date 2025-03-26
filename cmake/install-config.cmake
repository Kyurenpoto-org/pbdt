# SPDX-FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
#
# SPDX-License-Identifier: MIT

# Version config
write_basic_package_version_file(
    ${PROJECT_NAME}-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
    ARCH_INDEPENDENT
)

# CMake config
file(WRITE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake.in [[
@PACKAGE_INIT@
include("${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}-targets.cmake")
]])

configure_package_config_file(
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# pkg-config
file(WRITE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc.in [[
prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=@CMAKE_INSTALL_PREFIX@
libdir=@CMAKE_INSTALL_PREFIX@/@CMAKE_INSTALL_LIBDIR@
includedir=@CMAKE_INSTALL_PREFIX@/@CMAKE_INSTALL_INCLUDEDIR@

Name: @PROJECT_NAME@
Description: @PROJECT_DESCRIPTION@
Version: @PROJECT_VERSION@
Libs.msvc: /LIBPATH:${libdir} @PROJECT_NAME@.lib
Libs.gcc: -L${libdir} -l@PROJECT_NAME@
Libs.clang: -L${libdir} -l@PROJECT_NAME@
Cflags.header.msvc: /I${includedir}
Cflags.header.gcc: -I${includedir}
Cflags.header.clang: -I${includedir}
Cflags.module.msvc: /experimental:module /reference ${includedir}/@PROJECT_NAME@.ifc
Cflags.module.gcc: -fmodules-ts -fmodule-file=${includedir}/@PROJECT_NAME@.gcm
Cflags.module.clang: -fmodules-ts -fmodule-file=${includedir}/@PROJECT_NAME@.pcm
]])

configure_file(
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc"
    @ONLY
)

install(
    FILES
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
install(
    FILES
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig/${PROJECT_NAME}
)
