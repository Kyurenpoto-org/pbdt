/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "idempotent.hpp"
#include "productable-container.hpp"

template <typename When>
struct CompletableRawWhenContext
{
    static constexpr auto RAW_CONTEXT = Productable::ProductableCombination<
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value;
    static constexpr When when{};

    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(RAW_CONTEXT)>;
    }

    template <size_t Idx, size_t... Idxs>
    constexpr auto complete(const std::index_sequence<Idx, Idxs...>) const
    {
        return (when(std::get<Idx>(RAW_CONTEXT)()) + ... + std::get<Idxs>(RAW_CONTEXT)()).complete();
    }
};

template <typename ToContainer, typename When>
void idempotent()
{
    const AcceptableRawContext<CompletableRawWhenContext<When>> acceptable;
    acceptable.accept(IdempotentValidator<When, ToContainer>{});
}
