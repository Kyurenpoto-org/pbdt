/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include "generators/values/composable-callable.hpp"
#include "generators/values/foldable-callable.hpp"
#include "generators/values/productable-container.hpp"

namespace Productable
{
    template <size_t... Ns>
    struct SequenceArray<std::index_sequence<Ns...>, 2>
    {
        static constexpr auto value = std::array{
            std::tuple{ static_cast<int>(Ns), static_cast<int>(Ns) }...,
        };

        static constexpr auto rvalue()
        {
            return value;
        }
    };

    template <size_t... Ns>
    struct SequenceArray<std::index_sequence<Ns...>, 3>
    {
        static constexpr auto value = std::array{
            std::tuple<int, std::string_view>{ static_cast<int>(Ns), charTable.substr(Ns % 10, 1) }...,
        };

        static constexpr auto rvalue()
        {
            return value;
        }
    };

    template <size_t... Ns>
    struct SequenceArray<std::index_sequence<Ns...>, 4>
    {
        static constexpr auto value = std::array{
            std::tuple<std::string_view, int>{ charTable.substr(Ns % 10, 1), static_cast<int>(Ns) }...,
        };

        static constexpr auto rvalue()
        {
            return value;
        }
    };

    template <size_t... Ns>
    struct SequenceArray<std::index_sequence<Ns...>, 5>
    {
        static constexpr auto value = std::array{
            std::tuple{ charTable.substr(Ns % 10, 1), charTable.substr(Ns % 10, 1) }...,
        };

        static constexpr auto rvalue()
        {
            return value;
        }
    };
}

namespace Runnable
{
    template <size_t>
    struct IndexToTypeImpl;

    template <>
    struct IndexToTypeImpl<0>
    {
        using type = std::tuple<int>;
        using flatType = int;
    };

    template <>
    struct IndexToTypeImpl<1>
    {
        using type = std::tuple<std::string_view>;
        using flatType = std::string_view;
    };

    template <>
    struct IndexToTypeImpl<2>
    {
        using type = std::tuple<int, int>;
        using flatType = std::tuple<int, int>;
    };

    template <>
    struct IndexToTypeImpl<3>
    {
        using type = std::tuple<int, std::string_view>;
        using flatType = std::tuple<int, std::string_view>;
    };

    template <>
    struct IndexToTypeImpl<4>
    {
        using type = std::tuple<std::string_view, int>;
        using flatType = std::tuple<std::string_view, int>;
    };

    template <>
    struct IndexToTypeImpl<5>
    {
        using type = std::tuple<std::string_view, std::string_view>;
        using flatType = std::tuple<std::string_view, std::string_view>;
    };

    template <size_t N>
    using IndexToType = typename IndexToTypeImpl<N>::type;

    template <size_t N>
    using IndexToFlatType = typename IndexToTypeImpl<N>::flatType;

    constexpr size_t TYPE_INDEX_LIMIT = 6;
    constexpr size_t TYPE_DOUBLE_INDEX_LIMIT = TYPE_INDEX_LIMIT * TYPE_INDEX_LIMIT;
    constexpr size_t TYPE_TRIPLE_INDEX_LIMIT = TYPE_DOUBLE_INDEX_LIMIT * TYPE_INDEX_LIMIT;
    constexpr size_t TARGET_SHAPE_LIMIT = 2;
    constexpr size_t TARGET_VARIETY_LIMIT = TARGET_SHAPE_LIMIT;
    constexpr size_t TARGET_DOUBLE_VARIETY_LIMIT = TARGET_VARIETY_LIMIT * TARGET_VARIETY_LIMIT;
    constexpr size_t PROP_SHAPE_LIMIT = 2;
    constexpr size_t PROP_RESULT_LIMIT = 2;
    constexpr size_t PROP_VARIETY_LIMIT = PROP_SHAPE_LIMIT * PROP_RESULT_LIMIT;
    constexpr size_t PROP_DOUBLE_VARIETY_LIMIT = PROP_VARIETY_LIMIT * PROP_VARIETY_LIMIT;
    constexpr size_t DOMAIN_SHAPE_LIMIT = 4;
    constexpr size_t DOMAIN_LENGTH_LIMIT = 10;
    constexpr size_t DOMAIN_VARIETY_LIMIT = DOMAIN_SHAPE_LIMIT * DOMAIN_LENGTH_LIMIT;
    constexpr size_t DOMAIN_DOUBLE_VARIETY_LIMIT = DOMAIN_VARIETY_LIMIT * DOMAIN_VARIETY_LIMIT;

    consteval size_t domainVarietyLength(size_t variety)
    {
        return variety / DOMAIN_SHAPE_LIMIT % DOMAIN_LENGTH_LIMIT + 1;
    }

    template <size_t N, size_t M>
    struct Target :
        Composable::ComposableCallable<
            N % TARGET_SHAPE_LIMIT, IndexToType<M / TYPE_INDEX_LIMIT % TYPE_INDEX_LIMIT>,
            IndexToType<M % TYPE_INDEX_LIMIT>>
    {
    };

    template <size_t N, size_t M>
    struct Prop :
        Foldable::FoldableCallable<
            N % PROP_SHAPE_LIMIT, N / PROP_SHAPE_LIMIT % PROP_RESULT_LIMIT, IndexToFlatType<M % TYPE_INDEX_LIMIT>,
            IndexToFlatType<M / TYPE_INDEX_LIMIT % TYPE_INDEX_LIMIT>>
    {
    };

    template <size_t N, size_t M>
    struct Domain :
        Productable::ProductableContainer<
            N % DOMAIN_SHAPE_LIMIT, decltype(std::make_index_sequence<domainVarietyLength(N)>()), M % TYPE_INDEX_LIMIT>
    {
    };

    template <size_t A, size_t B, size_t C, size_t D>
    struct SuccessRunnableCombination
    {
        constexpr auto target() const
        {
            return Target<A % TARGET_VARIETY_LIMIT, D % TYPE_DOUBLE_INDEX_LIMIT>::value;
        }

        template <typename Expect>
        constexpr auto prop() const
        {
            return Prop<B % PROP_RESULT_LIMIT, D % TYPE_DOUBLE_INDEX_LIMIT>::template value<Expect>;
        }

        constexpr auto domain() const
        {
            return Domain<C % DOMAIN_VARIETY_LIMIT, D % TYPE_INDEX_LIMIT>::rvalue();
        }
    };

    template <size_t A, size_t B, size_t C, size_t D>
    struct FailureRunnableCombination
    {
        constexpr auto target() const
        {
            return Target<A % TARGET_VARIETY_LIMIT, D % TYPE_DOUBLE_INDEX_LIMIT>::value;
        }

        template <typename Expect>
        constexpr auto prop() const
        {
            return Prop<B % PROP_RESULT_LIMIT + PROP_RESULT_LIMIT, D % TYPE_DOUBLE_INDEX_LIMIT>::template value<Expect>;
        }

        constexpr auto domain() const
        {
            return Domain<C % DOMAIN_VARIETY_LIMIT, D % TYPE_INDEX_LIMIT>::rvalue();
        }
    };

    template <size_t N>
    using RunnableCombinationSuccessSample = SuccessRunnableCombination<
        N / TYPE_DOUBLE_INDEX_LIMIT % TARGET_VARIETY_LIMIT,
        N / TYPE_DOUBLE_INDEX_LIMIT / TARGET_VARIETY_LIMIT % PROP_RESULT_LIMIT,
        N / TYPE_DOUBLE_INDEX_LIMIT / TARGET_VARIETY_LIMIT / PROP_RESULT_LIMIT % DOMAIN_VARIETY_LIMIT,
        N % TYPE_DOUBLE_INDEX_LIMIT>;

    template <size_t N>
    using RunnableCombinationFailureSample = FailureRunnableCombination<
        N / TYPE_DOUBLE_INDEX_LIMIT % TARGET_VARIETY_LIMIT,
        N / TYPE_DOUBLE_INDEX_LIMIT / TARGET_VARIETY_LIMIT % PROP_RESULT_LIMIT,
        N / TYPE_DOUBLE_INDEX_LIMIT / TARGET_VARIETY_LIMIT / PROP_RESULT_LIMIT % DOMAIN_VARIETY_LIMIT,
        N % TYPE_DOUBLE_INDEX_LIMIT>;

    template <size_t First, size_t... Rest>
    struct RunnableCombinationSamples
    {
        static constexpr auto value = std::tuple_cat(
            std::tuple{
                RunnableCombinationSuccessSample<First>{},
                RunnableCombinationFailureSample<First>{},
            },
            RunnableCombinationSamples<Rest...>::value
        );
    };

    template <size_t N>
    struct RunnableCombinationSamples<N>
    {
        static constexpr auto value = std::tuple{
            RunnableCombinationSuccessSample<N>{},
            RunnableCombinationFailureSample<N>{},
        };
    };
}
