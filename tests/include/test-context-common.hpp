/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <array>
#include <functional>
#include <ranges>
#include <tuple>

#include "runnable-combination.hpp"
#include "util.hpp"

namespace Examples
{
    template <typename Expect>
    constexpr auto combinations = std::tuple{
        Runnable::SuccessRunnableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{},
        Runnable::SuccessRunnableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{},
        Runnable::SuccessRunnableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{},
        Runnable::SuccessRunnableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{},
        Runnable::SuccessRunnableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{},
        Runnable::SuccessRunnableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{},
        Runnable::SuccessRunnableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{},
        Runnable::SuccessRunnableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{},
    };

    template <typename Expect, typename PropertyContext, size_t... Idxs>
    constexpr void assertCombinations(const std::index_sequence<Idxs...>)
    {
        (
            []()
            {
                constexpr auto propertyContext = PropertyContext{};

                constexpr auto compileTimeCombination = std::get<Idxs>(combinations<Expect>);
                static_assert(propertyContext(
                                  compileTimeCombination.target(), compileTimeCombination.prop(),
                                  compileTimeCombination.domain()
                )
                                  .passable());

                const auto runtimeCombination = std::get<Idxs>(combinations<Expect>);
                dynamic_assert(
                    propertyContext(runtimeCombination.target(), runtimeCombination.prop(), runtimeCombination.domain())
                        .passable()
                );
            }(),
            ...
        );
    }
}

template <typename Expect, typename PropertyContext>
void examples()
{
    Examples::assertCombinations<Expect, PropertyContext>(
        std::make_index_sequence<std::tuple_size_v<decltype(Examples::combinations<Expect>)>>()
    );
}
