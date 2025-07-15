/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

namespace Countable
{
    template <typename EventCountable, size_t... Ns>
    struct EventCountableDoubleValueCombination
    {
        static constexpr size_t size()
        {
            return COMBINATION_INDEX_LIMIT;
        }

        template <size_t Idx>
        constexpr auto a() const
        {
            return value<NS_VALUE[Idx] % COMBINATION_INDEX_LIMIT>();
        }

        template <size_t Idx>
        constexpr auto b() const
        {
            return value<NS_VALUE[Idx] / COMBINATION_INDEX_LIMIT % COMBINATION_INDEX_LIMIT>();
        }

    private:
        template <size_t Idx>
        constexpr auto value() const
        {
            if constexpr (Idx == 0)
            {
                return eventCountable.prototype();
            }
            else
            {
                return EventCountable::template Op<
                    NS_VALUE[Idx - 1] / COMBINATION_DOUBLE_INDEX_LIMIT % OP_VARIETY_LIMIT>()(value<Idx - 1>());
            }
        }

        static constexpr size_t COMBINATION_INDEX_LIMIT = sizeof...(Ns);
        static constexpr size_t COMBINATION_DOUBLE_INDEX_LIMIT = COMBINATION_INDEX_LIMIT * COMBINATION_INDEX_LIMIT;
        static constexpr size_t OP_VARIETY_LIMIT = 3;

        static constexpr std::array<size_t, COMBINATION_INDEX_LIMIT> NS_VALUE{ Ns... };
        static constexpr EventCountable eventCountable{};
    };
};
