/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <ranges>
#include <tuple>

#include "composable-callable.hpp"
#include "util.hpp"

namespace Idempotent
{
    constexpr auto rawContexts = std::tuple{
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
    };

    template <typename Given, size_t... Idxs>
    constexpr void assertCombinations(const std::index_sequence<Idxs...>)
    {
        (
            []()
            {
                constexpr auto completeContext = [&]()
                {
                    return std::apply(
                        []<typename Target, typename... Targets>(Target&& target, Targets&&... targets)
                        {
                            return (std::invoke(Given{}, std::forward<Target>(target)) + ...
                                    + std::forward<Targets>(targets))
                                .complete();
                        },
                        std::get<Idxs>(rawContexts)
                    );
                };

                constexpr auto compileTimeCompleted = completeContext();
                static_assert(compileTimeCompleted == std::invoke(Given{}, compileTimeCompleted).complete());

                const auto runTimeCompleted = completeContext();
                dynamic_assert(runTimeCompleted == std::invoke(Given{}, runTimeCompleted).complete());
            }(),
            ...
        );
    }
}

template <typename Given>
void idempotent()
{
    Idempotent::assertCombinations<Given>(
        std::make_index_sequence<std::tuple_size_v<decltype(Idempotent::rawContexts)>>()
    );
}
