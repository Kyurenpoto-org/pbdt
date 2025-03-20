/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "exstd/ranges.hpp"

#include "when-component-associative-common.hpp"

#include "pbdt/bdd.hpp"

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
