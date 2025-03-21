/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <functional>
#include <tuple>

#include "foldable-callable.hpp"
#include "util.hpp"

namespace Idempotent
{
    template <typename Expect>
    constexpr auto rawContexts = std::tuple{
        Foldable::FoldableCombination<Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
    };

    template <typename Expect, typename ToFlatTuple, typename Then, size_t... Idxs>
    constexpr void assertCombinations(const std::index_sequence<Idxs...>)
    {
        (
            []()
            {
                constexpr auto completeContext = [&]()
                {
                    return std::apply(
                        []<typename Prop, typename... Props>(Prop&& prop, Props&&... props)
                        {
                            return (std::invoke(Then{}, std::forward<Prop>(prop)) + ... + std::forward<Props>(props))
                                .complete();
                        },
                        std::get<Idxs>(rawContexts<Expect>)
                    );
                };

                constexpr auto compileTimeCompleted = completeContext();
                static_assert(
                    std::invoke(ToFlatTuple{}, compileTimeCompleted)
                    == std::invoke(ToFlatTuple{}, std::invoke(Then{}, compileTimeCompleted).complete())
                );

                const auto runTimeCompleted = completeContext();
                dynamic_assert(
                    std::invoke(ToFlatTuple{}, runTimeCompleted)
                    == std::invoke(ToFlatTuple{}, std::invoke(Then{}, runTimeCompleted).complete())
                );
            }(),
            ...
        );
    }
}

template <typename Expect, typename ToFlatTuple, typename Then>
void idempotent()
{
    Idempotent::assertCombinations<Expect, ToFlatTuple, Then>(
        std::make_index_sequence<std::tuple_size_v<decltype(Idempotent::rawContexts<Expect>)>>()
    );
}
