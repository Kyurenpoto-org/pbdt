/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <array>
#include <vector>

namespace Countable
{
    template <typename EventCountable, size_t... Ns>
        requires((Ns < 3) && ...)
    struct EventCountableSequence
    {
        constexpr std::array<EventCountable, sizeof...(Ns)> value() const
        {
            return toArr(generate(), std::make_index_sequence<sizeof...(Ns)>{});
        }

    private:
        constexpr std::vector<EventCountable> generate() const
        {
            std::vector<EventCountable> vec;
            vec.reserve(sizeof...(Ns));
            vec.push_back(EventCountable::prototype());

            for (size_t i = 0; i < sizeof...(Ns) - 1; ++i)
            {
                vec.push_back((vec[i].*EVENT_METHODS[NS_VALUE[i]])());
            }

            return vec;
        }

        template <size_t... INDICE>
        constexpr std::array<EventCountable, sizeof...(Ns)>
        toArr(std::vector<EventCountable> x, std::index_sequence<INDICE...>) const
        {
            return { x[INDICE]... };
        }

        using EventMethodType = EventCountable (EventCountable::*)() const;
        static constexpr std::array<EventMethodType, 3> EVENT_METHODS{
            &EventCountable::pass,
            &EventCountable::fail,
            &EventCountable::skip,
        };
        static constexpr std::array<size_t, sizeof...(Ns)> NS_VALUE{ Ns... };
    };

    // 다른 횟수, 동일 조합
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
            return seq[opCountForA<Idx>()];
        }

        template <size_t Idx>
        constexpr auto b() const
        {
            return seq[opCountForB<Idx>()];
        }

    private:
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

        static constexpr size_t COMBINATION_INDEX_LIMIT = sizeof...(Ns);
        static constexpr size_t COMBINATION_DOUBLE_INDEX_LIMIT = COMBINATION_INDEX_LIMIT * COMBINATION_INDEX_LIMIT;
        static constexpr size_t OP_VARIETY_LIMIT = 3;

        static constexpr std::array<size_t, COMBINATION_INDEX_LIMIT> NS_VALUE{ Ns... };
        static constexpr auto seq =
            EventCountableSequence<EventCountable, (Ns / COMBINATION_DOUBLE_INDEX_LIMIT % OP_VARIETY_LIMIT)...>{}
                .value();
    };

    // 동일 횟수, 다른 조합
    template <typename EventCountable, size_t... Ns>
    struct EventCountableSingleDropCombination
    {
        template <size_t Idx>
        constexpr auto a() const
        {
            return std::array{
                seqs[0][0][opCount<Idx>()],
                seqs[0][1][opCount<Idx>()],
                seqs[0][2][opCount<Idx>()],
            };
        }

        template <size_t Idx>
        constexpr auto b() const
        {
            return std::array{
                seqs[1][0][opCount<Idx>()],
                seqs[1][1][opCount<Idx>()],
                seqs[1][2][opCount<Idx>()],
            };
        }

    private:
        template <size_t Idx>
        static consteval size_t opCount()
        {
            return NS_VALUE[Idx] % COMBINATION_INDEX_LIMIT;
        }

        static constexpr size_t COMBINATION_INDEX_LIMIT = sizeof...(Ns);
        static constexpr size_t OP_VARIETY_LIMIT = 3;

        static constexpr std::array<size_t, COMBINATION_INDEX_LIMIT> NS_VALUE{ Ns... };
        static constexpr auto seqs = std::array{
            std::array{
                EventCountableSequence<
                    EventCountable,
                    ((Ns / COMBINATION_INDEX_LIMIT % (OP_VARIETY_LIMIT - 1) + 1) % OP_VARIETY_LIMIT)...>{}
                    .value(),
                EventCountableSequence<
                    EventCountable,
                    ((Ns / COMBINATION_INDEX_LIMIT % (OP_VARIETY_LIMIT - 1) + 2) % OP_VARIETY_LIMIT)...>{}
                    .value(),
                EventCountableSequence<
                    EventCountable,
                    ((Ns / COMBINATION_INDEX_LIMIT % (OP_VARIETY_LIMIT - 1) + 3) % OP_VARIETY_LIMIT)...>{}
                    .value(),
            },
            std::array{
                EventCountableSequence<
                    EventCountable,
                    ((Ns / COMBINATION_INDEX_LIMIT / (OP_VARIETY_LIMIT - 1) % (OP_VARIETY_LIMIT - 1) + 1)
                     % OP_VARIETY_LIMIT)...>{}
                    .value(),
                EventCountableSequence<
                    EventCountable,
                    ((Ns / COMBINATION_INDEX_LIMIT / (OP_VARIETY_LIMIT - 1) % (OP_VARIETY_LIMIT - 1) + 2)
                     % OP_VARIETY_LIMIT)...>{}
                    .value(),
                EventCountableSequence<
                    EventCountable,
                    ((Ns / COMBINATION_INDEX_LIMIT / (OP_VARIETY_LIMIT - 1) % (OP_VARIETY_LIMIT - 1) + 3)
                     % OP_VARIETY_LIMIT)...>{}
                    .value(),
            },
        };
    };
};
