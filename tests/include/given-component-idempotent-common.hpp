/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "composable-callable.hpp"
#include "idempotent.hpp"

template <typename Given>
struct CompletableRawGivenContext
{
    static constexpr auto RAW_CONTEXT = Composable::ComposableCombination<
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value;
    static constexpr Given given{};

    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(RAW_CONTEXT)>;
    }

    template <size_t Idx, size_t... Idxs>
    constexpr auto complete(const std::index_sequence<Idx, Idxs...>) const
    {
        return (given(std::get<Idx>(RAW_CONTEXT)) + ... + std::get<Idxs>(RAW_CONTEXT)).complete();
    }
};

template <typename ToFlatTuple, typename Given>
void idempotent()
{
    const AcceptableRawContext<CompletableRawGivenContext<Given>> acceptable;
    acceptable.accept(IdempotentValidator<Given, ToFlatTuple>{});
}
