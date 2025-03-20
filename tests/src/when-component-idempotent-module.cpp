/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "when-component-idempotent-common.hpp"

import exstd;
import pbdt;

namespace pbdt::bdd::detail
{
    template <typename... Domains, typename Domain>
    constexpr auto operator+(WhenContext<Domains...>&& context, Domain&& domain)
    {
        return context.andWhen("", std::forward<Domain>(domain));
    }
}

struct ToContainer
{
    template <typename T>
    constexpr auto operator()(T&& t) const
    {
        return exstd::toContainer(std::forward<T>(t));
    }
};

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
    idempotent<ToContainer, When>();

    return EXIT_SUCCESS;
}
