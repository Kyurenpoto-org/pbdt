/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <tuple>

#include "productable-container.hpp"
#include "util.hpp"

namespace Idempotent
{
    constexpr auto rawContexts = std::tuple{
        ProductableCombination<COMPILE_TIME_RANDOM()>::value,
        ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ProductableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ProductableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ProductableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ProductableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
    };

    template <typename When, size_t... Idxs>
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
                static_assert(
                    exstd::toContainer(compileTimeCompleted)
                    == exstd::toContainer(std::invoke(When{}, compileTimeCompleted).complete())
                );

                const auto runTimeCompleted = completeContext();
                dynamic_assert(
                    exstd::toContainer(runTimeCompleted)
                    == exstd::toContainer(std::invoke(When{}, runTimeCompleted).complete())
                );
            }(),
            ...
        );
    }
}

template <typename When>
void idempotent()
{
    Idempotent::assertCombinations<When>(std::make_index_sequence<std::tuple_size_v<decltype(Idempotent::rawContexts)>>(
    ));
}
