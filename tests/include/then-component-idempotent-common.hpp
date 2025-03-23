/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "foldable-callable.hpp"
#include "idempotent.hpp"

template <typename Then, typename Expect>
struct CompletableRawThenContext
{
    static constexpr auto RAW_CONTEXT = Foldable::FoldableCombination<
        Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value;
    static constexpr Then then;

    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(RAW_CONTEXT)>;
    }

    template <size_t Idx, size_t... Idxs>
    constexpr auto complete(const std::index_sequence<Idx, Idxs...>) const
    {
        return (then(std::get<Idx>(RAW_CONTEXT)) + ... + std::get<Idxs>(RAW_CONTEXT)).complete();
    }
};

template <typename Expect, typename ToFlatTuple, typename Then>
void idempotent()
{
    const AcceptableRawContext<CompletableRawThenContext<Then, Expect>> acceptable;
    acceptable.accept(IdempotentValidator<Then, ToFlatTuple>{});
}
