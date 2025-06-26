/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

module;

#include <algorithm>
#include <array>
#include <ranges>
#include <span>
#include <tuple>
#include <type_traits>
#include <vector>

#include "exstd/module-unit-guard.hpp"

export module exstd.ranges;

export import exstd.type_traits;

export {
#include "exstd/ranges.hpp"
}
