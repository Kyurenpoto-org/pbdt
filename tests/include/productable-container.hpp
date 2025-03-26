/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <array>
#include <ranges>
#include <span>
#include <string_view>
#include <tuple>

namespace Productable
{
    template <typename, size_t>
    struct SequenceArray;

    template <size_t... Ns>
    struct SequenceArray<std::index_sequence<Ns...>, 0>
    {
        static constexpr auto value = std::array<int, sizeof...(Ns)>{ Ns... };

        static constexpr auto rvalue()
        {
            return value;
        }
    };

    constexpr std::string_view charTable = "0123456789";

    template <size_t... Ns>
    struct SequenceArray<std::index_sequence<Ns...>, 1>
    {
        static constexpr auto value = std::array{ (charTable.substr(Ns, 1))... };

        static constexpr auto rvalue()
        {
            return value;
        }
    };

    template <typename Seq, size_t N>
    struct SequenceArraySpan
    {
        static constexpr auto rvalue()
        {
            return std::span{ SequenceArray<Seq, N>::value };
        }
    };

    template <typename Seq, size_t N>
    struct SequenceArrayOwningView
    {
        static constexpr auto rvalue()
        {
            return std::ranges::owning_view{ SequenceArray<Seq, N>::rvalue() };
        }
    };

    template <typename Seq, size_t N>
    struct SequenceArrayRefView
    {
        static constexpr auto value = std::ranges::ref_view{ SequenceArray<Seq, N>::value };

        static constexpr auto rvalue()
        {
            return value;
        }
    };

    template <size_t, size_t>
    struct ProductableContainer;

    template <size_t N>
    struct ProductableContainer<0, N>
    {
        static constexpr auto rvalue()
        {
            return SequenceArray<decltype(std::make_index_sequence<N / 2 % 3 + 1>()), N % 2>::rvalue();
        }
    };

    template <size_t N>
    struct ProductableContainer<1, N>
    {
        static constexpr auto rvalue()
        {
            return SequenceArraySpan<decltype(std::make_index_sequence<N / 2 % 3 + 1>()), N % 2>::rvalue();
        }
    };

    template <size_t N>
    struct ProductableContainer<2, N>
    {
        static constexpr auto rvalue()
        {
            return SequenceArrayOwningView<decltype(std::make_index_sequence<N / 2 % 3 + 1>()), N % 2>::rvalue();
        }
    };

    template <size_t N>
    struct ProductableContainer<3, N>
    {
        static constexpr auto rvalue()
        {
            return SequenceArrayRefView<decltype(std::make_index_sequence<N / 2 % 3 + 1>()), N % 2>::rvalue();
        }
    };

    template <size_t First, size_t... Rest>
    struct ProductableCombination
    {
        static constexpr auto value = std::tuple_cat(
            std::tuple{
                []()
                {
                    return ProductableContainer<First % 4, First / 4 % 6>::rvalue();
                },
            },
            ProductableCombination<Rest...>::value
        );
    };

    template <size_t N>
    struct ProductableCombination<N>
    {
        static constexpr auto value = std::tuple{
            []()
            {
                return ProductableContainer<N % 4, N / 4 % 6>::rvalue();
            },
        };
    };
}
