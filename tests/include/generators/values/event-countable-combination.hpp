/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <array>

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
            return value<opCountForA<Idx>()>();
        }

        template <size_t Idx>
        constexpr auto b() const
        {
            return value<opCountForB<Idx>()>();
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
                return EventCountable::template op<opIndex<Idx>()>()(value<Idx - 1>());
            }
        }

        template <size_t Idx>
        static consteval size_t opCountForA()
        {
            return NS_VALUE[Idx] % COMBINATION_INDEX_LIMIT;
        }

        template <size_t Idx>
        static consteval size_t opCountForB()
        {
            return NS_VALUE[Idx] / COMBINATION_INDEX_LIMIT % COMBINATION_INDEX_LIMIT;
        }

        template <size_t Idx>
        static consteval size_t opIndex()
        {
            return NS_VALUE[Idx - 1] / COMBINATION_DOUBLE_INDEX_LIMIT % OP_VARIETY_LIMIT;
        }

        static constexpr size_t COMBINATION_INDEX_LIMIT = sizeof...(Ns);
        static constexpr size_t COMBINATION_DOUBLE_INDEX_LIMIT = COMBINATION_INDEX_LIMIT * COMBINATION_INDEX_LIMIT;
        static constexpr size_t OP_VARIETY_LIMIT = 3;

        static constexpr std::array<size_t, COMBINATION_INDEX_LIMIT> NS_VALUE{ Ns... };
        static constexpr EventCountable eventCountable{};
    };

    template <typename EventCountable, size_t... Ns>
    struct EventCountableSingleDropCombination
    {
        template <size_t Idx>
        constexpr auto a() const
        {
            return std::array{
                valueA<opCount<Idx>(), 0>(),
                valueA<opCount<Idx>(), 1>(),
                valueA<opCount<Idx>(), 2>(),
            };
        }

        template <size_t Idx>
        constexpr auto b() const
        {
            return std::array{
                valueB<opCount<Idx>(), 0>(),
                valueB<opCount<Idx>(), 1>(),
                valueB<opCount<Idx>(), 2>(),
            };
        }

    private:
        template <size_t Idx, size_t Drop>
        constexpr auto valueA() const
        {
            if constexpr (Idx == 0)
            {
                return eventCountable.prototype();
            }
            else
            {
                return EventCountable::template op<opIndexA<Idx, Drop>()>()(valueA<Idx - 1, Drop>());
            }
        }

        template <size_t Idx, size_t Drop>
        constexpr auto valueB() const
        {
            if constexpr (Idx == 0)
            {
                return eventCountable.prototype();
            }
            else
            {
                return EventCountable::template op<opIndexB<Idx, Drop>()>()(valueB<Idx - 1, Drop>());
            }
        }

        template <size_t Idx>
        static consteval size_t opCount()
        {
            return NS_VALUE[Idx] % COMBINATION_INDEX_LIMIT;
        }

        template <size_t Idx, size_t Drop>
        static consteval size_t opIndexA()
        {
            return (NS_VALUE[Idx - 1] / COMBINATION_INDEX_LIMIT % (OP_VARIETY_LIMIT - 1) + Drop + 1) % OP_VARIETY_LIMIT;
        }

        template <size_t Idx, size_t Drop>
        static consteval size_t opIndexB()
        {
            return (NS_VALUE[Idx - 1] / COMBINATION_INDEX_LIMIT / (OP_VARIETY_LIMIT - 1) % (OP_VARIETY_LIMIT - 1) + Drop
                    + 1)
                 % OP_VARIETY_LIMIT;
        }

        static constexpr size_t COMBINATION_INDEX_LIMIT = sizeof...(Ns);
        static constexpr size_t COMBINATION_DOUBLE_INDEX_LIMIT = COMBINATION_INDEX_LIMIT * COMBINATION_INDEX_LIMIT;
        static constexpr size_t OP_VARIETY_LIMIT = 3;

        static constexpr std::array<size_t, COMBINATION_INDEX_LIMIT> NS_VALUE{ Ns... };
        static constexpr EventCountable eventCountable{};
    };
};
