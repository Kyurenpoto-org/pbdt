/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <array>
#include <ranges>
#include <vector>

namespace Util
{
    template <std::ranges::input_range Range>
    struct SequenceFromIndice
    {
        consteval SequenceFromIndice(Range indice) :
            indice{
                indice,
            }
        {
        }

        template <typename T, size_t N, typename Initial, typename Next>
        consteval std::array<T, N> value(Initial&& initial, Next&& next) const
        {
            return generate<T>(initial(indice[0]), next, std::make_index_sequence<N>{});
        }

    private:
        template <typename T, typename Next, size_t... INDICE>
        consteval std::array<T, sizeof...(INDICE)>
        generate(const T& initial, const Next& next, std::index_sequence<INDICE...>) const
        {
            constexpr size_t N = sizeof...(INDICE);
            std::vector<T> vec;
            vec.reserve(N);
            vec.push_back(initial);

            for (size_t i = 1; i < N; ++i)
            {
                vec.push_back(next(vec[i - 1], indice[i]));
            }

            return { vec[INDICE]... };
        }

        Range indice;
    };
}
