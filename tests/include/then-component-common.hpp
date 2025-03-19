/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <string>
#include <tuple>

#include "pbdt/bdd.hpp"

namespace
{
    template <size_t N, typename Sample, typename Result>
    constexpr auto freeFunction(Sample, Result)
    {
        return pbdt::test_context::expect(N == 0);
    }

    template <size_t N, typename Sample, typename Result>
    struct Functor
    {
        constexpr auto operator()(Sample, Result)
        {
            return pbdt::test_context::expect(N == 0);
        }

        constexpr bool operator==(const Functor) const
        {
            return true;
        }
    };

    template <size_t N>
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

    template <size_t, size_t>
    struct FoldableFunc;

    template <size_t N>
    struct FoldableFunc<0, N>
    {
        static constexpr auto value = &freeFunction<N % 2, IndexToType<N / 2 % 2>, IndexToType<N / 4 % 2>>;
    };

    template <size_t N>
    struct FoldableFunc<1, N>
    {
        static constexpr auto value = Functor<N % 2, IndexToType<N / 2 % 2>, IndexToType<N / 4 % 2>>{};
    };

    template <size_t N, size_t First, size_t... Rest>
    struct FoldableCombination
    {
        static constexpr auto value = std::tuple_cat(
            std::tuple{
                FoldableFunc<First % 2, N>::value,
            },
            FoldableCombination<N, Rest...>::value
        );
    };

    template <size_t N, size_t M>
    struct FoldableCombination<N, M>
    {
        static constexpr auto value = std::tuple{
            FoldableFunc<M % 2, N>::value,
        };
    };
}
