/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <array>
#include <variant>

namespace Countable
{
    template <typename EventCountable>
    struct EventContainer
    {
        template <size_t N>
        static constexpr EventContainer prototype()
        {
            return EventContainer{
                std::variant_alternative_t<N, std::variant<Pass, Fail, Skip>>{},
            };
        }

        constexpr EventCountable operator()(const EventCountable x) const
        {
            return std::visit(
                [x](const auto event)
                {
                    return event(x);
                },
                event
            );
        }

    private:
        struct Pass
        {
            constexpr EventCountable operator()(const EventCountable x) const
            {
                return x.pass();
            }
        };

        struct Fail
        {
            constexpr EventCountable operator()(const EventCountable x) const
            {
                return x.fail();
            }
        };

        struct Skip
        {
            constexpr EventCountable operator()(const EventCountable x) const
            {
                return x.skip();
            }
        };

        constexpr EventContainer(std::variant<Pass, Fail, Skip>&& event) :
            event(std::move(event))
        {
        }

        std::variant<Pass, Fail, Skip> event;
    };

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
                return EventCountable::prototype();
            }
            else
            {
                return EventContainer<EventCountable>::template prototype<opIndex<Idx>()>()(value<Idx - 1>());
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
                return EventCountable::prototype();
            }
            else
            {
                return EventContainer<EventCountable>::template prototype<opIndexA<Idx, Drop>()>()(
                    valueA<Idx - 1, Drop>()
                );
            }
        }

        template <size_t Idx, size_t Drop>
        constexpr auto valueB() const
        {
            if constexpr (Idx == 0)
            {
                return EventCountable::prototype();
            }
            else
            {
                return EventContainer<EventCountable>::template prototype<opIndexB<Idx, Drop>()>()(
                    valueB<Idx - 1, Drop>()
                );
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
    };
};
