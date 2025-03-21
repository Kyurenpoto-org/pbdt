/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <functional>
#include <tuple>

#include "foldable-callable.hpp"
#include "util.hpp"

namespace Associative
{
    template <typename Expect>
    constexpr auto foldableCombinations = std::tuple{
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
    };

    template <typename Expect, typename ToFlatTuple, typename Then, size_t... Idxs>
    constexpr void assertCombinations(const std::index_sequence<Idxs...>)
    {
        (
            []()
            {
                constexpr auto combination = std::get<Idxs>(foldableCombinations<Expect>);

                constexpr auto a = std::get<0>(combination);
                constexpr auto b = std::get<1>(combination);
                constexpr auto c = std::get<2>(combination);

                constexpr auto compileTimeLhs =
                    std::invoke(Then{}, a).andThen("", std::invoke(Then{}, b).andThen("", c).complete()).complete();
                constexpr auto compileTimeRhs =
                    std::invoke(Then{}, std::invoke(Then{}, a).andThen("", b).complete()).andThen("", c).complete();
                static_assert(std::invoke(ToFlatTuple{}, compileTimeLhs) == std::invoke(ToFlatTuple{}, compileTimeRhs));

                const auto runTimeLhs =
                    std::invoke(Then{}, a).andThen("", std::invoke(Then{}, b).andThen("", c).complete()).complete();
                const auto runTimeRhs =
                    std::invoke(Then{}, std::invoke(Then{}, a).andThen("", b).complete()).andThen("", c).complete();
                dynamic_assert(std::invoke(ToFlatTuple{}, runTimeLhs) == std::invoke(ToFlatTuple{}, runTimeRhs));
            }(),
            ...
        );
    }
}

template <typename Expect, typename ToFlatTuple, typename Then>
void associative()
{
    Associative::assertCombinations<Expect, ToFlatTuple, Then>(
        std::make_index_sequence<std::tuple_size_v<decltype(Associative::foldableCombinations<Expect>)>>()
    );
}
