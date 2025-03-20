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

    template <size_t... Idxs>
    constexpr void assertCombinations(const std::index_sequence<Idxs...>)
    {
        (
            []()
            {
                constexpr auto combination = std::get<Idxs>(productableCombinations);

                constexpr auto a = std::get<0>(combination);
                constexpr auto b = std::get<1>(combination);
                constexpr auto c = std::get<2>(combination);

                constexpr auto compileTimeLhs = pbdt::bdd::when("", a())
                                                    .andWhen("", pbdt::bdd::when("", b()).andWhen("", c()).complete())
                                                    .complete();
                constexpr auto compileTimeRhs =
                    pbdt::bdd::when("", pbdt::bdd::when("", a()).andWhen("", b()).complete())
                        .andWhen("", c())
                        .complete();
                static_assert(exstd::toContainer(compileTimeLhs) == exstd::toContainer(compileTimeRhs));

                const auto runTimeLhs = pbdt::bdd::when("", a())
                                            .andWhen("", pbdt::bdd::when("", b()).andWhen("", c()).complete())
                                            .complete();
                const auto runTimeRhs = pbdt::bdd::when("", pbdt::bdd::when("", a()).andWhen("", b()).complete())
                                            .andWhen("", c())
                                            .complete();
                dynamic_assert(exstd::toContainer(runTimeLhs) == exstd::toContainer(runTimeRhs));
            }(),
            ...
        );
    }
}

void associative()
{
    Associative::assertCombinations(
        std::make_index_sequence<std::tuple_size_v<decltype(Associative::productableCombinations)>>()
    );
}

int main()
{
    // Set Category that has product between any two objects
    idempotent();
    associative();

    return EXIT_SUCCESS;
}
