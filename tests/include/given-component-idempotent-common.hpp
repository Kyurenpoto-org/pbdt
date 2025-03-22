/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <functional>
#include <tuple>

#include "composable-callable.hpp"
#include "idempotent.hpp"

template <typename Given>
struct CompletableRawGivenContext
{
    static constexpr auto rawContexts = Composable::ComposableCombination<
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value;

    template <size_t Idx, size_t... Idxs>
    static constexpr auto complete(const std::index_sequence<Idx, Idxs...>)
    {
        return (std::invoke(Given{}, std::get<Idx>(rawContexts)) + ... + std::get<Idxs>(rawContexts)).complete();
    }

    template <size_t N, typename Visitor>
    static void acceptImpl(Visitor&& visitor)
    {
        std::invoke(
            std::forward<Visitor>(visitor),
            []()
            {
                return CompletableRawGivenContext::complete(std::make_index_sequence<N + 1>());
            }
        );

        if constexpr (N > 0)
        {
            acceptImpl<N - 1>(std::forward<Visitor>(visitor));
        }
    }

    template <typename Visitor>
    static void accept(Visitor&& visitor)
    {
        CompletableRawGivenContext::acceptImpl<std::tuple_size_v<decltype(rawContexts)> - 1>(
            std::forward<Visitor>(visitor)
        );
    }
};

template <typename ToFlatTuple, typename Given>
void idempotent()
{
    CompletableRawGivenContext<Given>::accept(IdempotentValidator<IdempotentProperty<ToFlatTuple, Given>>{});
}
