/*
 * SPDX - FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

module;

#include <algorithm>
#include <array>
#include <ranges>
#include <source_location>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>

export module pbdt;

#define PBDT_MODULE

import exstd;

export
{
#include "pbdt/bdd.hpp"
#include "pbdt/test-context.hpp"
}
