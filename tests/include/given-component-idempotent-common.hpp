/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <functional>
#include <tuple>

#include "composable-callable.hpp"
#include "util.hpp"

template <typename ToFlatTuple, typename Given, typename CompletableRawContext>
struct HybridValidatableIdempotentGivenComponent
{
    template <size_t... Idxs>
    constexpr void validateImpl(const std::index_sequence<Idxs...>)
    {
        (
            []()
            {
                constexpr auto compileTimeCompleted = CompletableRawContext{}.template complete<Idxs>();
                static_assert(
                    std::invoke(ToFlatTuple{}, compileTimeCompleted)
                    == std::invoke(ToFlatTuple{}, std::invoke(Given{}, compileTimeCompleted).complete())
                );

                const auto runTimeCompleted = CompletableRawContext{}.template complete<Idxs>();
                dynamic_assert(
                    std::invoke(ToFlatTuple{}, runTimeCompleted)
                    == std::invoke(ToFlatTuple{}, std::invoke(Given{}, runTimeCompleted).complete())
                );
            }(),
            ...
        );
    }

    constexpr void validate()
    {
        validateImpl(std::make_index_sequence<CompletableRawContext::size>());
    }
};

template <typename Given>
struct CompletableRawGivenContext
{
    static constexpr auto rawContexts = std::tuple{
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
    };

    static constexpr size_t size = std::tuple_size_v<decltype(rawContexts)>;

    template <size_t Idx>
    constexpr auto complete() const
    {
        return std::apply(
            []<typename Target, typename... Targets>(Target&& target, Targets&&... targets)
            {
                return (std::invoke(Given{}, std::forward<Target>(target)) + ... + std::forward<Targets>(targets))
                    .complete();
            },
            std::get<Idx>(rawContexts)
        );
    }
};

template <typename ToFlatTuple, typename Given>
void idempotent()
{
    HybridValidatableIdempotentGivenComponent<ToFlatTuple, Given, CompletableRawGivenContext<Given>>{}.validate();
}
