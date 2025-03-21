/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "when-component-associative-common.hpp"

import exstd;
import pbdt;

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
    associative<ToContainer, When>();

    return EXIT_SUCCESS;
}
