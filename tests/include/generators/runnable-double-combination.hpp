/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include "generators/runnable-combination.hpp"

namespace Runnable
{
    template <size_t A, size_t B, size_t C, size_t D>
    struct DoublePropRunnableCombination
    {
        constexpr auto target() const
        {
            return Target<A % 2, D % 36>::value;
        }

        template <typename Expect>
        constexpr auto propA() const
        {
            return Prop<B % 4, D % 36>::template value<Expect>;
        }

        template <typename Expect>
        constexpr auto propB() const
        {
            return Prop<B / 4 % 4, D % 36>::template value<Expect>;
        }

        constexpr auto domain() const
        {
            return Domain<C % 40, D % 6>::rvalue();
        }
    };

    template <size_t N>
    using RunnableDoublePropCombinationSample =
        DoublePropRunnableCombination<N / 36 % 2, N / 36 / 2 % 16, N / 36 / 32 % 40, N % 36>;

    template <size_t First, size_t... Rest>
    struct RunnableDoublePropCombinationSamples
    {
        static constexpr auto value = std::tuple_cat(
            std::tuple{
                RunnableDoublePropCombinationSample<First>{},
            },
            RunnableDoublePropCombinationSamples<Rest...>::value
        );
    };

    template <size_t N>
    struct RunnableDoublePropCombinationSamples<N>
    {
        static constexpr auto value = std::tuple{
            RunnableDoublePropCombinationSample<N>{},
        };
    };
}
