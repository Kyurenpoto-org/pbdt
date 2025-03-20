/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>
#include <functional>
#include <print>
#include <ranges>
#include <source_location>
#include <tuple>

import pbdt;

#include "test-context-common.hpp"

struct PropertyContext
{
    template <typename Target, typename Prop, typename Domain>
    constexpr auto operator()(Target&& target, Prop&& prop, Domain&& domain)
    {
        return pbdt::test_context::propertyContext(target, prop, domain);
    }
};

int main()
{
    examples<PropertyContext>();

    return EXIT_SUCCESS;
}
