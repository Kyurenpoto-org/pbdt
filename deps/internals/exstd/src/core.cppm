/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

module;

#include <algorithm>
#include <array>
#include <concepts>
#include <functional>
#include <ranges>
#include <span>
#include <tuple>
#include <type_traits>
#include <variant>
#include <vector>

#include "exstd/module-unit-guard.hpp"

export module exstd;

export import exstd.callable_traits;

export {
#include "exstd/type-container.hpp"
#include "exstd/type-traits.hpp"

#include "exstd/functional.hpp"
#include "exstd/ranges.hpp"
}
