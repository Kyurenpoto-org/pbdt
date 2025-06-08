/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "generators/runnable-combination.hpp"

template <typename>
struct RunnableCombinationSuccessSampleTuple;

template <size_t... Ns>
struct RunnableCombinationSuccessSampleTuple<std::index_sequence<Ns...>>
{
    using type = Runnable::RunnableCombinationSamples<Ns...>;
};

using RunnableCombinationSuccessSampleTupleType =
    RunnableCombinationSuccessSampleTuple<decltype(std::make_index_sequence<36 * 4 * 40>())>;
