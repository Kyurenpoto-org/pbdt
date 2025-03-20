/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <tuple>

#include "fixtures.hpp"
#include "productable-container.hpp"

namespace Associative
{
    constexpr auto productableCombinations = std::tuple{
        ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
    };

    template <typename When, size_t... Idxs>
    constexpr void assertCombinations(const std::index_sequence<Idxs...>)
    {
        (
            []()
            {
                constexpr auto combination = std::get<Idxs>(productableCombinations);

                constexpr auto a = std::get<0>(combination);
                constexpr auto b = std::get<1>(combination);
                constexpr auto c = std::get<2>(combination);

                constexpr auto compileTimeLhs = std::invoke(When{}, a())
                                                    .andWhen("", std::invoke(When{}, b()).andWhen("", c()).complete())
                                                    .complete();
                constexpr auto compileTimeRhs =
                    std::invoke(When{}, std::invoke(When{}, a()).andWhen("", b()).complete())
                        .andWhen("", c())
                        .complete();
                static_assert(exstd::toContainer(compileTimeLhs) == exstd::toContainer(compileTimeRhs));

                const auto runTimeLhs = std::invoke(When{}, a())
                                            .andWhen("", std::invoke(When{}, b()).andWhen("", c()).complete())
                                            .complete();
                const auto runTimeRhs = std::invoke(When{}, std::invoke(When{}, a()).andWhen("", b()).complete())
                                            .andWhen("", c())
                                            .complete();
                dynamic_assert(exstd::toContainer(runTimeLhs) == exstd::toContainer(runTimeRhs));
            }(),
            ...
        );
    }
}

template <typename When>
void associative()
{
    Associative::assertCombinations<When>(
        std::make_index_sequence<std::tuple_size_v<decltype(Associative::productableCombinations)>>()
    );
}
