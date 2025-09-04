/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <array>
#include <ranges>
#include <vector>

namespace Countable
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

    template <typename Range>
    struct EventCountableSequence
    {
        consteval EventCountableSequence(Range indice) :
            indice(indice)
        {
        }

        template <typename EventCountable, size_t N>
        consteval auto value() const
        {
            return toArr<EventCountable>(generate<EventCountable>(), std::make_index_sequence<N>{});
        }

    private:
        template <typename EventCountable>
        consteval std::vector<EventCountable> generate() const
        {
            std::vector<EventCountable> vec;
            vec.reserve(indice.size());
            vec.push_back(EventCountable::prototype());

            static constexpr std::array<EventCountable (EventCountable::*)() const, 3> EVENT_METHODS{
                &EventCountable::pass,
                &EventCountable::fail,
                &EventCountable::skip,
            };

            for (size_t i = 0; i < indice.size() - 1; ++i)
            {
                vec.push_back((vec[i].*EVENT_METHODS[indice[i]])());
            }

            return vec;
        }

        template <typename EventCountable, size_t... INDICE>
        consteval auto toArr(std::vector<EventCountable> x, std::index_sequence<INDICE...>) const
        {
            return std::array<EventCountable, sizeof...(INDICE)>{ x[INDICE]... };
        }

        Range indice;
    };

    template <typename Range>
    EventCountableSequence(Range) -> EventCountableSequence<Range>;

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
            return SEQUENCE[INDICE[Idx][0]];
        }

        template <size_t Idx>
        constexpr auto b() const
        {
            return SEQUENCE[INDICE[Idx][1]];
        }

    private:
        static constexpr size_t COMBINATION_INDEX_LIMIT = sizeof...(Ns);
        static constexpr size_t OP_VARIETY_LIMIT = 3;

        static constexpr auto INDICE = std::array{
            (MultiIndex<Ns, COMBINATION_INDEX_LIMIT, COMBINATION_INDEX_LIMIT, OP_VARIETY_LIMIT>{}.value())...
        };
        static constexpr auto SEQUENCE = EventCountableSequence{
            INDICE
            | std::views::transform(
                [](const auto& x)
                {
                    return x[2];
                }
            )
        }.template value<EventCountable, COMBINATION_INDEX_LIMIT>();
    };

    template <typename EventCountable, size_t... Ns>
    struct EventCountableSingleDropCombination
    {
        template <size_t Idx>
        constexpr auto a() const
        {
            return std::array{
                SEQUENCES[0][0][INDICE[Idx][0]],
                SEQUENCES[0][1][INDICE[Idx][0]],
                SEQUENCES[0][2][INDICE[Idx][0]],
            };
        }

        template <size_t Idx>
        constexpr auto b() const
        {
            return std::array{
                SEQUENCES[1][0][INDICE[Idx][0]],
                SEQUENCES[1][1][INDICE[Idx][0]],
                SEQUENCES[1][2][INDICE[Idx][0]],
            };
        }

    private:
        static constexpr size_t COMBINATION_INDEX_LIMIT = sizeof...(Ns);
        static constexpr size_t OP_VARIETY_LIMIT = 3;

        static constexpr auto INDICE = std::array{
            (MultiIndex<Ns, COMBINATION_INDEX_LIMIT, OP_VARIETY_LIMIT - 1, OP_VARIETY_LIMIT - 1>{}.value())...
        };
        static constexpr auto SEQUENCES = std::array{
            std::array{
                EventCountableSequence{
                    INDICE
                        | std::views::transform(
                            [](const auto& x)
                            {
                                return (x[1] + 1) % OP_VARIETY_LIMIT;
                            }
                        ),
                }
                    .template value<EventCountable, COMBINATION_INDEX_LIMIT>(),
                EventCountableSequence{
                    INDICE
                        | std::views::transform(
                            [](const auto& x)
                            {
                                return (x[1] + 2) % OP_VARIETY_LIMIT;
                            }
                        ),
                }
                    .template value<EventCountable, COMBINATION_INDEX_LIMIT>(),
                EventCountableSequence{
                    INDICE
                        | std::views::transform(
                            [](const auto& x)
                            {
                                return x[1];
                            }
                        ),
                }
                    .template value<EventCountable, COMBINATION_INDEX_LIMIT>(),
            },
            std::array{
                EventCountableSequence{
                    INDICE
                        | std::views::transform(
                            [](const auto& x)
                            {
                                return (x[2] + 1) % OP_VARIETY_LIMIT;
                            }
                        ),
                }
                    .template value<EventCountable, COMBINATION_INDEX_LIMIT>(),
                EventCountableSequence{
                    INDICE
                        | std::views::transform(
                            [](const auto& x)
                            {
                                return (x[2] + 2) % OP_VARIETY_LIMIT;
                            }
                        ),
                }
                    .template value<EventCountable, COMBINATION_INDEX_LIMIT>(),
                EventCountableSequence{
                    INDICE
                        | std::views::transform(
                            [](const auto& x)
                            {
                                return x[2];
                            }
                        ),
                }
                    .template value<EventCountable, COMBINATION_INDEX_LIMIT>(),
            },
        };
    };
};
