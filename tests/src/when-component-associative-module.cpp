/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

import pbdt;

#include "when-component-associative-common.hpp"

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
    associative<When>();

    return EXIT_SUCCESS;
}
