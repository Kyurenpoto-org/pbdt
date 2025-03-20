/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "exstd/ranges.hpp"

#include "when-component-idempotent-common.hpp"

#include "pbdt/bdd.hpp"

namespace pbdt::bdd::detail
{
    template <typename... Domains, typename Domain>
    constexpr auto operator+(WhenContext<Domains...>&& context, Domain&& domain)
    {
        return context.andWhen("", std::forward<Domain>(domain));
    }
}

struct When
{
    template <typename Domain>
    constexpr auto operator()(Domain&& domain) const
    {
        return pbdt::bdd::when("", std::forward<Domain>(domain));
    }
};

int main()
{
    // Set Category that has product between any two objects
    idempotent<When>();

    return EXIT_SUCCESS;
}
