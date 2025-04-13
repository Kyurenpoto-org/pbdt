/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include "generators/composable-callable.hpp"
#include "generators/foldable-callable.hpp"
#include "generators/productable-container.hpp"

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

    template <size_t N, size_t M>
    struct Target : Composable::ComposableCallable<N % 2, IndexToType<M / 6 % 6>, IndexToType<M % 6>>
    {
    };

    template <size_t N, size_t M>
    struct Prop : Foldable::FoldableCallable<N % 2, N / 2 % 2, IndexToFlatType<M % 6>, IndexToFlatType<M / 6 % 6>>
    {
    };

    template <size_t N, size_t M>
    struct Domain :
        Productable::ProductableContainer<N % 4, decltype(std::make_index_sequence<N / 4 % 10 + 1>()), M % 6>
    {
    };

    template <size_t A, size_t B, size_t C, size_t D>
    struct SuccessRunnableCombination
    {
        constexpr auto target() const
        {
            return Target<A % 2, D % 36>::value;
        }

        template <typename Expect>
        constexpr auto prop() const
        {
            return Prop<B % 2, D % 36>::template value<Expect>;
        }

        constexpr auto domain() const
        {
            return Domain<C % 40, D % 36>::rvalue();
        }
    };

    template <size_t A, size_t B, size_t C, size_t D>
    struct FailureRunnableCombination
    {
        constexpr auto target() const
        {
            return Target<A % 2, D % 36>::value;
        }

        template <typename Expect>
        constexpr auto prop() const
        {
            return Prop<B % 2 + 2, D % 36>::template value<Expect>;
        }

        constexpr auto domain() const
        {
            return Domain<C % 40, D % 6>::rvalue();
        }
    };

    template <size_t N>
    using RunnableCombinationSuccessSample =
        SuccessRunnableCombination<N / 36 % 2, N / 36 / 2 % 2, N / 36 / 4 % 40, N % 36>;

    template <size_t N>
    using RunnableCombinationFailureSample =
        FailureRunnableCombination<N / 36 % 2, N / 36 / 2 % 2, N / 36 / 4 % 40, N % 36>;

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
