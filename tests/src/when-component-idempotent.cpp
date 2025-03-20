/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <tuple>

#include "pbdt/bdd.hpp"

#include "fixtures.hpp"
#include "when-component-common.hpp"

namespace pbdt::bdd::detail
{
    template <typename... Domains, typename Domain>
    constexpr auto operator+(WhenContext<Domains...>&& context, Domain&& domain)
    {
        return context.andWhen("", std::forward<Domain>(domain));
    }
}

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
        ProductableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ProductableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
    };

    template <size_t... Idxs>
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
                            return (pbdt::bdd::when("", domain()) + ... + (domains())).complete();
                        },
                        std::get<Idxs>(rawContexts)
                    );
                };

                constexpr auto compileTimeCompleted = completeContext();
                static_assert(
                    exstd::toContainer(compileTimeCompleted)
                    == exstd::toContainer(pbdt::bdd::when("", compileTimeCompleted).complete())
                );

                const auto runTimeCompleted = completeContext();
                dynamic_assert(
                    exstd::toContainer(runTimeCompleted)
                    == exstd::toContainer(pbdt::bdd::when("", runTimeCompleted).complete())
                );
            }(),
            ...
        );
    }
}

void idempotent()
{
    Idempotent::assertCombinations(std::make_index_sequence<std::tuple_size_v<decltype(Idempotent::rawContexts)>>());
}

int main()
{
    // Set Category that has product between any two objects
    idempotent();

    return EXIT_SUCCESS;
}
