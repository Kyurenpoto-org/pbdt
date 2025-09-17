/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <array>

namespace Util
{
    template <size_t INDEX, size_t... INDEX_LIMITS>
    struct MultiIndex
    {
        static constexpr size_t DIM = sizeof...(INDEX_LIMITS);

        consteval std::array<size_t, DIM> value() const
        {
            std::array<size_t, DIM> arr{};
            size_t idx = INDEX;
            for (size_t i = 0; i < DIM; ++i)
            {
                arr[i] = idx % LIMITS[i];
                idx /= LIMITS[i];
            }

            return arr;
        }

    private:
        static constexpr std::array<size_t, DIM> LIMITS{ INDEX_LIMITS... };
    };
}
