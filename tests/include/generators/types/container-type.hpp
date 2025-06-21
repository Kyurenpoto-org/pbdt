/*
 * SPDX - FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <array>
#include <ranges>
#include <span>
#include <vector>

#include "generators/values/composable-callable.hpp"

namespace Container
{
    template <typename T, size_t N>
    using SequenceArray = std::array<T, N>;

    template <typename T, size_t>
    using SequenceVector = std::vector<T>;

    template <typename T, size_t N>
    using SequenceFixedSpan = std::span<T, N>;

    template <typename T, size_t>
    using SequenceDynamicSpan = std::span<T, std::dynamic_extent>;

    template <typename T, size_t N>
    using SequenceArrayOwningView = std::ranges::owning_view<std::array<T, N>>;

    template <typename T, size_t>
    using SequenceVectorOwningView = std::ranges::owning_view<std::vector<T>>;

    template <typename T, size_t N>
    using SequenceArrayRefView = std::ranges::ref_view<std::array<T, N>>;

    template <typename T, size_t>
    using SequenceVectorRefView = std::ranges::ref_view<std::vector<T>>;

    template <typename T, size_t N>
    using SequenceFixedSpanRefView = std::ranges::ref_view<std::span<T, N>>;

    template <typename T, size_t>
    using SequenceDynamicSpanRefView = std::ranges::ref_view<std::span<T, std::dynamic_extent>>;

    template <typename, size_t, size_t>
    struct RangeConceptInstanceImpl;

    template <typename T, size_t N>
    struct RangeConceptInstanceImpl<T, N, 0>
    {
        using type = SequenceArray<T, N>;
    };

    template <typename T, size_t N>
    struct RangeConceptInstanceImpl<T, N, 1>
    {
        using type = SequenceVector<T, N>;
    };

    template <typename T, size_t N>
    struct RangeConceptInstanceImpl<T, N, 2>
    {
        using type = SequenceFixedSpan<T, N>;
    };

    template <typename T, size_t N>
    struct RangeConceptInstanceImpl<T, N, 3>
    {
        using type = SequenceDynamicSpan<T, N>;
    };

    template <typename T, size_t N>
    struct RangeConceptInstanceImpl<T, N, 4>
    {
        using type = SequenceArrayOwningView<T, N>;
    };

    template <typename T, size_t N>
    struct RangeConceptInstanceImpl<T, N, 5>
    {
        using type = SequenceVectorOwningView<T, N>;
    };

    template <typename T, size_t N>
    struct RangeConceptInstanceImpl<T, N, 6>
    {
        using type = SequenceArrayRefView<T, N>;
    };

    template <typename T, size_t N>
    struct RangeConceptInstanceImpl<T, N, 7>
    {
        using type = SequenceVectorRefView<T, N>;
    };

    template <typename T, size_t N>
    struct RangeConceptInstanceImpl<T, N, 8>
    {
        using type = SequenceFixedSpanRefView<T, N>;
    };

    template <typename T, size_t N>
    struct RangeConceptInstanceImpl<T, N, 9>
    {
        using type = SequenceDynamicSpanRefView<T, N>;
    };

    constexpr size_t RANGE_CONCEPT_INSTANCE_INDEX_LIMIT = 10;
    constexpr size_t RANCE_CONCEPT_INSTANCE_LENGTH_LIMIT = 10;

    template <size_t N, size_t M>
    using RangeDomainCombinationImpl = typename RangeConceptInstanceImpl<
        typename Composable::TypeSequence<M % Composable::TYPE_SEQUENCE_INDEX_LIMIT>::type,
        N % RANCE_CONCEPT_INSTANCE_LENGTH_LIMIT + 1,
        N / RANCE_CONCEPT_INSTANCE_LENGTH_LIMIT % RANGE_CONCEPT_INSTANCE_INDEX_LIMIT>::type;

    template <size_t N, size_t First, size_t... Rest>
    struct RangeDomainCombination
    {
        using type = decltype(std::tuple_cat(
            std::declval<std::tuple<RangeDomainCombinationImpl<N, First>>>(),
            std::declval<typename RangeDomainCombination<N / RANCE_CONCEPT_INSTANCE_LENGTH_LIMIT, Rest...>::type>()
        ));
    };
    template <size_t N, size_t M>
    struct RangeDomainCombination<N, M>
    {
        using type = std::tuple<RangeDomainCombinationImpl<N, M>>;
    };
}
