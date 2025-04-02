/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "test-context-common.hpp"

#include "pbdt/bdd.hpp"

struct Expect
{
    constexpr auto
    operator()(const bool expression, const std::source_location& location = std::source_location::current())
    {
        return pbdt::test_context::expect(expression, location);
    }
};

struct PropertyContext
{
    template <typename Target, typename Prop, typename Domain>
    constexpr auto operator()(Target&& target, Prop&& prop, Domain&& domain)
    {
        return pbdt::bdd::scenario(std::forward<Target>(target), std::forward<Prop>(prop), std::forward<Domain>(domain))
            .run();
    }
};

int main()
{
    examples<Expect, PropertyContext>();

    return EXIT_SUCCESS;
}
