/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <functional>
#include <tuple>

#include "productable-container.hpp"
#include "util.hpp"

namespace Idempotent
{
    constexpr auto rawContexts = std::tuple{
        Productable::ProductableCombination<COMPILE_TIME_RANDOM()>::value,
        Productable::ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Productable::ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Productable::ProductableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Productable::ProductableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        Productable::ProductableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Productable::ProductableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
    };

    template <typename ToContainer, typename When, size_t... Idxs>
    constexpr void assertCombinations(const std::index_sequence<Idxs...>)
    {
        (
            []()
            {
                constexpr auto completeContext = [&]()
                {
                    return std::apply(
                        []<typename Domain, typename... Domains>(Domain&& domain, Domains&&... domains)
                        {
                            return (std::invoke(When{}, domain()) + ... + (domains())).complete();
                        },
                        std::get<Idxs>(rawContexts)
                    );
                };

                constexpr auto compileTimeCompleted = completeContext();
                using tt = std::invoke_result_t<ToContainer, const std::array<int, 3>&>;
                static_assert(
                    std::invoke(ToContainer{}, compileTimeCompleted)
                    == std::invoke(ToContainer{}, std::invoke(When{}, compileTimeCompleted).complete())
                );

                const auto runTimeCompleted = completeContext();
                dynamic_assert(
                    std::invoke(ToContainer{}, runTimeCompleted)
                    == std::invoke(ToContainer{}, std::invoke(When{}, runTimeCompleted).complete())
                );
            }(),
            ...
        );
    }
}

template <typename ToContainer, typename When>
void idempotent()
{
    Idempotent::assertCombinations<ToContainer, When>(
        std::make_index_sequence<std::tuple_size_v<decltype(Idempotent::rawContexts)>>()
    );
}
