/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <functional>
#include <tuple>

#include "composable-callable.hpp"
#include "util.hpp"

namespace Associative
{
    constexpr auto composableCombinations = std::tuple{
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
    };

    template <typename ToFlatTuple, typename Given, size_t... Idxs>
    constexpr void assertCombinations(const std::index_sequence<Idxs...>)
    {
        (
            []()
            {
                constexpr auto combination = std::get<Idxs>(composableCombinations);

                constexpr auto a = std::get<0>(combination);
                constexpr auto b = std::get<1>(combination);
                constexpr auto c = std::get<2>(combination);

                constexpr auto compileTimeLhs =
                    std::invoke(Given{}, a).andGiven("", std::invoke(Given{}, b).andGiven("", c).complete()).complete();
                constexpr auto compileTimeRhs =
                    std::invoke(Given{}, std::invoke(Given{}, a).andGiven("", b).complete()).andGiven("", c).complete();
                static_assert(std::invoke(ToFlatTuple{}, compileTimeLhs) == std::invoke(ToFlatTuple{}, compileTimeRhs));

                const auto runTimeLhs =
                    std::invoke(Given{}, a).andGiven("", std::invoke(Given{}, b).andGiven("", c).complete()).complete();
                const auto runTimeRhs =
                    std::invoke(Given{}, std::invoke(Given{}, a).andGiven("", b).complete()).andGiven("", c).complete();
                dynamic_assert(std::invoke(ToFlatTuple{}, runTimeLhs) == std::invoke(ToFlatTuple{}, runTimeRhs));
            }(),
            ...
        );
    }
}

template <typename ToFlatTuple, typename Given>
void associative()
{
    Associative::assertCombinations<ToFlatTuple, Given>(
        std::make_index_sequence<std::tuple_size_v<decltype(Associative::composableCombinations)>>()
    );
}
