/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "model.hpp"

import pbdt;

struct Expect
{
    constexpr auto
    operator()(const bool expression, const std::source_location& location = std::source_location::current())
    {
        return pbdt::test_context::expect(expression, location);
    }
};

struct ApplyOrInvoke
{
    template <typename Func, typename Arg>
    constexpr auto operator()(Func&& func, Arg&& arg) const
    {
        return exstd::applyOrInvoke(std::forward<Func>(func), std::forward<Arg>(arg));
    }
};

struct RunnableScenario
{
    template <typename Target, typename Prop, typename Domain>
    constexpr auto operator()(Target&& target, Prop&& prop, Domain&& domain) const
    {
        return pbdt::bdd::scenario(
            std::forward<Target>(target), std::forward<Prop>(prop), std::forward<Domain>(domain)
        );
    }
};

int main()
{
    const AcceptableCombination<TwoWayRunnableScenarioCombination<Expect, ApplyOrInvoke, RunnableScenario>> acceptable;
    acceptable.accept(ModelValidator{});

    return EXIT_SUCCESS;
}
