/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

module;

#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <variant>

#include "exstd/module-unit-guard.hpp"

export module exstd.functional;

export import exstd.type_traits;

export {
#include "exstd/functional.hpp"
}
