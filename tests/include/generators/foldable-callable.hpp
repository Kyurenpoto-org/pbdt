/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <functional>
#include <string>
#include <tuple>

namespace Foldable
{
    template <size_t N, typename Sample, typename Result, typename Expect>
    constexpr auto freeFunction(Sample, Result)
    {
        return std::invoke(Expect{}, N == 0);
    }

    template <size_t N, typename Sample, typename Result, typename Expect>
    struct Functor
    {
        constexpr auto operator()(Sample, Result) const
        {
            return std::invoke(Expect{}, N == 0);
        }

        constexpr bool operator==(const Functor) const
        {
            return true;
        }
    };

    template <size_t>
    struct IndexToTypeImpl;

    template <>
    struct IndexToTypeImpl<0>
    {
        using type = int;
    };

    template <>
    struct IndexToTypeImpl<1>
    {
        using type = std::string;
    };

    template <size_t N>
    using IndexToType = typename IndexToTypeImpl<N>::type;

    template <size_t, size_t, typename, typename>
    struct FoldableCallable;

    template <size_t N, typename T, typename U>
    struct FoldableCallable<0, N, T, U>
    {
        template <typename Expect>
        static constexpr auto value = &freeFunction<N, T, U, Expect>;
    };

    template <size_t N, typename T, typename U>
    struct FoldableCallable<1, N, T, U>
    {
        template <typename Expect>
        static constexpr auto value = Functor<N, T, U, Expect>{};
    };

    template <size_t N, size_t M>
    using FoldableCombinationImpl = FoldableCallable<M % 2, N % 2, IndexToType<N / 2 % 2>, IndexToType<N / 4 % 2>>;

    template <typename Expect, size_t N, size_t First, size_t... Rest>
    struct FoldableCombination
    {
        static constexpr auto value = std::tuple_cat(
            std::tuple{
                FoldableCombinationImpl<N, First>::template value<Expect>,
            },
            FoldableCombination<Expect, N, Rest...>::value
        );
    };

    template <typename Expect, size_t N, size_t M>
    struct FoldableCombination<Expect, N, M>
    {
        static constexpr auto value = std::tuple{
            FoldableCombinationImpl<N, M>::template value<Expect>,
        };
    };
}
