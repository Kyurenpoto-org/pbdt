/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include "generators/values/runnable-combination.hpp"

namespace Runnable
{
    template <size_t A, size_t B, size_t C, size_t D>
    struct DoubleTargetRunnableCombination
    {
        constexpr auto targetA() const
        {
            return Target<A % TARGET_VARIETY_LIMIT, D % TYPE_DOUBLE_INDEX_LIMIT>::value;
        }

        constexpr auto targetB() const
        {
            return Target<
                A / TARGET_VARIETY_LIMIT % TARGET_VARIETY_LIMIT, D / TYPE_INDEX_LIMIT % TYPE_DOUBLE_INDEX_LIMIT>::value;
        }

        template <typename Expect>
        constexpr auto prop() const
        {
            return Prop<
                B % PROP_VARIETY_LIMIT,
                (D % TYPE_INDEX_LIMIT)
                    + (D / TYPE_DOUBLE_INDEX_LIMIT % TYPE_INDEX_LIMIT * TYPE_INDEX_LIMIT)>::template value<Expect>;
        }

        constexpr auto domain() const
        {
            return Domain<C % DOMAIN_VARIETY_LIMIT, D % TYPE_INDEX_LIMIT>::rvalue();
        }

        template <typename Expect>
        constexpr auto propForTargetA() const
        {
            return Prop<B % PROP_VARIETY_LIMIT, D % TYPE_DOUBLE_INDEX_LIMIT>::template value<Expect>;
        }
    };

    template <size_t N>
    using IndexToTypeExt = decltype(std::tuple_cat(
        std::declval<IndexToType<N % TYPE_INDEX_LIMIT>>(),
        std::declval<IndexToType<N / TYPE_INDEX_LIMIT % TYPE_INDEX_LIMIT>>()
    ));

    template <size_t N, size_t M>
    struct TargetExt :
        Composable::ComposableCallable<
            N % TARGET_SHAPE_LIMIT, IndexToType<M / TYPE_DOUBLE_INDEX_LIMIT % TYPE_INDEX_LIMIT>,
            IndexToTypeExt<M % TYPE_DOUBLE_INDEX_LIMIT>>
    {
    };

    template <size_t N, size_t M>
    struct PropExt :
        Foldable::FoldableCallable<
            N % PROP_SHAPE_LIMIT, N / PROP_SHAPE_LIMIT % PROP_RESULT_LIMIT, IndexToTypeExt<M % TYPE_DOUBLE_INDEX_LIMIT>,
            IndexToFlatType<M / TYPE_DOUBLE_INDEX_LIMIT % TYPE_INDEX_LIMIT>>
    {
    };

    template <size_t N, size_t M, size_t LEN>
    struct TargetForDomainA :
        Composable::ComposableCallable<
            N % TARGET_SHAPE_LIMIT,
            std::tuple<std::array<IndexToFlatType<M / TYPE_DOUBLE_INDEX_LIMIT % TYPE_INDEX_LIMIT>, LEN>>,
            IndexToType<M % TYPE_INDEX_LIMIT>>
    {
    };

    template <size_t N, size_t M, size_t LEN>
    struct TargetForDomainB :
        Composable::ComposableCallable<
            N % TARGET_SHAPE_LIMIT,
            std::tuple<std::array<IndexToFlatType<M / TYPE_DOUBLE_INDEX_LIMIT % TYPE_INDEX_LIMIT>, LEN>>,
            IndexToType<M / TYPE_INDEX_LIMIT % TYPE_INDEX_LIMIT>>
    {
    };

    template <size_t N, size_t M, size_t LEN>
    struct PropForDomainA :
        Foldable::FoldableCallable<
            N % PROP_SHAPE_LIMIT, N / PROP_SHAPE_LIMIT % PROP_RESULT_LIMIT, IndexToFlatType<M % TYPE_INDEX_LIMIT>,
            std::array<IndexToFlatType<M / TYPE_DOUBLE_INDEX_LIMIT % TYPE_INDEX_LIMIT>, LEN>>
    {
    };

    template <size_t N, size_t M, size_t LEN>
    struct PropForDomainB :
        Foldable::FoldableCallable<
            N % PROP_SHAPE_LIMIT, N / PROP_SHAPE_LIMIT % PROP_RESULT_LIMIT,
            IndexToFlatType<M / TYPE_INDEX_LIMIT % TYPE_INDEX_LIMIT>,
            std::array<IndexToFlatType<M / TYPE_DOUBLE_INDEX_LIMIT % TYPE_INDEX_LIMIT>, LEN>>
    {
    };

    template <size_t A, size_t B, size_t C, size_t D>
    struct DoubleDomainRunnableCombination
    {
        constexpr auto target() const
        {
            return TargetExt<A % TARGET_VARIETY_LIMIT, D % TYPE_TRIPLE_INDEX_LIMIT>::value;
        }

        template <typename Expect>
        constexpr auto prop() const
        {
            return PropExt<B % PROP_VARIETY_LIMIT, D % TYPE_TRIPLE_INDEX_LIMIT>::template value<Expect>;
        }

        constexpr auto domainA() const
        {
            return Domain<C % DOMAIN_VARIETY_LIMIT, D % TYPE_INDEX_LIMIT>::rvalue();
        }

        constexpr auto domainB() const
        {
            return Domain<
                C / DOMAIN_VARIETY_LIMIT % DOMAIN_VARIETY_LIMIT, D / TYPE_INDEX_LIMIT % TYPE_INDEX_LIMIT>::rvalue();
        }

        constexpr auto targetForDomainA() const
        {
            return TargetForDomainA<
                A % TARGET_VARIETY_LIMIT, D % TYPE_TRIPLE_INDEX_LIMIT,
                domainVarietyLength(C % DOMAIN_VARIETY_LIMIT)>::value;
        }

        constexpr auto targetForDomainB() const
        {
            return TargetForDomainB<
                A % TARGET_VARIETY_LIMIT, D % TYPE_TRIPLE_INDEX_LIMIT,
                domainVarietyLength(C / DOMAIN_VARIETY_LIMIT % DOMAIN_VARIETY_LIMIT)>::value;
        }

        template <typename Expect>
        constexpr auto propForDomainA() const
        {
            return PropForDomainA<
                B % PROP_VARIETY_LIMIT, D % TYPE_TRIPLE_INDEX_LIMIT,
                domainVarietyLength(C % DOMAIN_VARIETY_LIMIT)>::template value<Expect>;
        }

        template <typename Expect>
        constexpr auto propForDomainB() const
        {
            return PropForDomainB<
                B % PROP_VARIETY_LIMIT, D % TYPE_TRIPLE_INDEX_LIMIT,
                domainVarietyLength(C / DOMAIN_VARIETY_LIMIT % DOMAIN_VARIETY_LIMIT)>::template value<Expect>;
        }
    };

    template <size_t A, size_t B, size_t C, size_t D>
    struct DoublePropRunnableCombination
    {
        constexpr auto target() const
        {
            return Target<A % TARGET_VARIETY_LIMIT, D % TYPE_DOUBLE_INDEX_LIMIT>::value;
        }

        template <typename Expect>
        constexpr auto propA() const
        {
            return Prop<B % PROP_VARIETY_LIMIT, D % TYPE_DOUBLE_INDEX_LIMIT>::template value<Expect>;
        }

        template <typename Expect>
        constexpr auto propB() const
        {
            return Prop<B / PROP_VARIETY_LIMIT % PROP_VARIETY_LIMIT, D % TYPE_DOUBLE_INDEX_LIMIT>::template value<
                Expect>;
        }

        constexpr auto domain() const
        {
            return Domain<C % DOMAIN_VARIETY_LIMIT, D % TYPE_INDEX_LIMIT>::rvalue();
        }
    };

    template <size_t N>
    using RunnableDoubleTargetCombinationSample = DoubleTargetRunnableCombination<
        N / TYPE_TRIPLE_INDEX_LIMIT % TARGET_DOUBLE_VARIETY_LIMIT,
        N / TYPE_TRIPLE_INDEX_LIMIT / TARGET_DOUBLE_VARIETY_LIMIT % PROP_VARIETY_LIMIT,
        N / TYPE_TRIPLE_INDEX_LIMIT / TARGET_DOUBLE_VARIETY_LIMIT / PROP_VARIETY_LIMIT % DOMAIN_VARIETY_LIMIT,
        N % TYPE_TRIPLE_INDEX_LIMIT>;

    template <size_t N>
    using RunnableDoubleDomainCombinationSample = DoubleDomainRunnableCombination<
        N / TYPE_TRIPLE_INDEX_LIMIT % TARGET_VARIETY_LIMIT,
        N / TYPE_TRIPLE_INDEX_LIMIT / TARGET_VARIETY_LIMIT % PROP_VARIETY_LIMIT,
        N / TYPE_TRIPLE_INDEX_LIMIT / TARGET_VARIETY_LIMIT / PROP_VARIETY_LIMIT % DOMAIN_DOUBLE_VARIETY_LIMIT,
        N % TYPE_TRIPLE_INDEX_LIMIT>;

    template <size_t N>
    using RunnableDoublePropCombinationSample = DoublePropRunnableCombination<
        N / TYPE_DOUBLE_INDEX_LIMIT % TARGET_VARIETY_LIMIT,
        N / TYPE_DOUBLE_INDEX_LIMIT / TARGET_VARIETY_LIMIT % PROP_DOUBLE_VARIETY_LIMIT,
        N / TYPE_DOUBLE_INDEX_LIMIT / TARGET_VARIETY_LIMIT / PROP_DOUBLE_VARIETY_LIMIT % DOMAIN_VARIETY_LIMIT,
        N % TYPE_DOUBLE_INDEX_LIMIT>;

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
