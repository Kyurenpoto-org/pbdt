# SPDX-FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
#
# SPDX-License-Identifier: MIT

# vcpkg
if(DEFINED ENV{VCPKG_ROOT})
	set(vcpkg_toolchain_path $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

	if(EXISTS ${vcpkg_toolchain_path})
		include(${vcpkg_toolchain_path})
	else()
		message(WARNING "Vcpkg toolchain file not found at ${vcpkg_toolchain_path}")
	endif()

	unset(vcpkg_toolchain_path)
else()
	message(WARNING "VCPKG_ROOT not defined")
endif()

# conan
if(DEFINED ENV{CONAN_HOME})
	include(${CMAKE_CURRENT_LIST_DIR}/conan.cmake)
else()
	message(WARNING "CONAN_HOME not defined")
endif()
