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
    struct DoubleTargetRunnableCombination
    {
        constexpr auto targetA() const
        {
            return Target<A % 2, D % 36>::value;
        }

        constexpr auto targetB() const
        {
            return Target<A / 2 % 2, D / 6 % 36>::value;
        }

        template <typename Expect>
        constexpr auto prop() const
        {
            return Prop<B % 4, (D % 6) + (D / 36 % 6 * 6)>::template value<Expect>;
        }

        constexpr auto domain() const
        {
            return Domain<C % 40, D % 6>::rvalue();
        }

        template <typename Expect>
        constexpr auto propForTargetA() const
        {
            return Prop<B % 4, D % 36>::template value<Expect>;
        }
    };

    template <size_t N>
    using IndexToTypeExt =
        decltype(std::tuple_cat(std::declval<IndexToType<N % 6>>(), std::declval<IndexToType<N / 6 % 6>>()));

    template <size_t N, size_t M>
    struct TargetExt : Composable::ComposableCallable<N % 2, IndexToType<M / 36 % 6>, IndexToTypeExt<M % 36>>
    {
    };

    template <size_t N, size_t M>
    struct PropExt : Foldable::FoldableCallable<N % 2, N / 2 % 2, IndexToTypeExt<M % 36>, IndexToFlatType<M / 36 % 6>>
    {
    };

    template <size_t N, size_t M, size_t LEN>
    struct TargetForDomainA :
        Composable::ComposableCallable<
            N % 2, std::tuple<std::array<IndexToFlatType<M / 36 % 6>, LEN>>, IndexToType<M % 6>>
    {
    };

    template <size_t N, size_t M, size_t LEN>
    struct TargetForDomainB :
        Composable::ComposableCallable<
            N % 2, std::tuple<std::array<IndexToFlatType<M / 36 % 6>, LEN>>, IndexToType<M / 6 % 6>>
    {
    };

    template <size_t N, size_t M, size_t LEN>
    struct PropForDomainA :
        Foldable::FoldableCallable<
            N % 2, N / 2 % 2, IndexToFlatType<M % 6>, std::array<IndexToFlatType<M / 36 % 6>, LEN>>
    {
    };

    template <size_t N, size_t M, size_t LEN>
    struct PropForDomainB :
        Foldable::FoldableCallable<
            N % 2, N / 2 % 2, IndexToFlatType<M / 6 % 6>, std::array<IndexToFlatType<M / 36 % 6>, LEN>>
    {
    };

    template <size_t A, size_t B, size_t C, size_t D>
    struct DoubleDomainRunnableCombination
    {
        constexpr auto target() const
        {
            return TargetExt<A % 2, D % 216>::value;
        }

        template <typename Expect>
        constexpr auto prop() const
        {
            return PropExt<B % 4, D % 216>::template value<Expect>;
        }

        constexpr auto domainA() const
        {
            return Domain<C % 40, D % 6>::rvalue();
        }

        constexpr auto domainB() const
        {
            return Domain<C / 40 % 40, D / 6 % 6>::rvalue();
        }

        constexpr auto targetForDomainA() const
        {
            return TargetForDomainA<A % 2, D % 216, C % 40 / 4 % 10 + 1>::value;
        }

        constexpr auto targetForDomainB() const
        {
            return TargetForDomainB<A % 2, D % 216, C / 40 % 40 / 4 % 10 + 1>::value;
        }

        template <typename Expect>
        constexpr auto propForDomainA() const
        {
            return PropForDomainA<B % 4, D % 216, C % 40 / 4 % 10 + 1>::template value<Expect>;
        }

        template <typename Expect>
        constexpr auto propForDomainB() const
        {
            return PropForDomainB<B % 4, D % 216, C / 40 % 40 / 4 % 10 + 1>::template value<Expect>;
        }
    };

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
    using RunnableDoubleTargetCombinationSample =
        DoubleTargetRunnableCombination<N / 216 % 4, N / 216 / 4 % 4, N / 216 / 16 % 40, N % 216>;

    template <size_t N>
    using RunnableDoubleDomainCombinationSample =
        DoubleDomainRunnableCombination<N / 216 % 2, N / 216 / 2 % 4, N / 216 / 8 % 1600, N % 216>;

    template <size_t N>
    using RunnableDoublePropCombinationSample =
        DoublePropRunnableCombination<N / 36 % 2, N / 36 / 2 % 16, N / 36 / 32 % 40, N % 36>;

    template <size_t First, size_t... Rest>
    struct RunnableDoubleTargetCombinationSamples
    {
        static constexpr auto value = std::tuple_cat(
            std::tuple{
                RunnableDoubleTargetCombinationSample<First>{},
            },
            RunnableDoubleTargetCombinationSamples<Rest...>::value
        );
    };

    template <size_t N>
    struct RunnableDoubleTargetCombinationSamples<N>
    {
        static constexpr auto value = std::tuple{
            RunnableDoubleTargetCombinationSample<N>{},
        };
    };

    template <size_t First, size_t... Rest>
    struct RunnableDoubleDomainCombinationSamples
    {
        static constexpr auto value = std::tuple_cat(
            std::tuple{
                RunnableDoubleDomainCombinationSample<First>{},
            },
            RunnableDoubleDomainCombinationSamples<Rest...>::value
        );
    };

    template <size_t N>
    struct RunnableDoubleDomainCombinationSamples<N>
    {
        static constexpr auto value = std::tuple{
            RunnableDoubleDomainCombinationSample<N>{},
        };
    };

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
