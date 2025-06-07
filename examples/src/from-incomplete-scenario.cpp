/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "pbdt/bdd.hpp"

namespace
{
    constexpr auto target = [](int x) -> int
    {
        return x + 0;
    };
    constexpr auto prop = [](int x, int result)
    {
        return pbdt::test_context::expect(x == result);
    };
    constexpr auto domain = std::array<int, 5>{ 1, 2, 3, 4, 5 };
}

int main()
{
    constexpr auto context = pbdt::bdd::scenario()
                                 .given(
                                     [](int x) -> int
                                     {
                                         return x + 0;
                                     }
                                 )
                                 .then(
                                     [](int x, int result)
                                     {
                                         return pbdt::test_context::expect(x == result);
                                     }
                                 );

    static_assert(context.when(domain).complete().run().passable());

    return EXIT_SUCCESS;
}
