/*
 * SPDX - FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

module;

#include <algorithm>
#include <array>
#include <concepts>
#include <coroutine>
#include <exception>
#include <functional>
#include <optional>
#include <ranges>
#include <source_location>
#include <span>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <variant>
#include <vector>

export module prop_pp;

#define PBDT_MODULE

import exstd;

export
{
#include "prop-pp/bdd.hpp"
#include "prop-pp/test-context.hpp"
}
