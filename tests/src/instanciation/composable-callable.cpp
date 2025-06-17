/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "generators/composable-callable.hpp"

template <size_t, size_t, typename>
struct InstanciatedComposableCallableImpl;

template <size_t N, size_t M, size_t... Ns>
struct InstanciatedComposableCallableImpl<N, M, std::index_sequence<Ns...>>
{
    using type = std::tuple<std::tuple<Composable::ComposableCallable<
        N, typename Composable::TypeSequence<M>::type, typename Composable::TypeSequence<Ns>::type>...>>;
};

template <size_t, typename>
struct InstanciatedComposableCallable;

template <size_t N, size_t... Ns>
struct InstanciatedComposableCallable<N, std::index_sequence<Ns...>>
{
    using type = std::tuple<typename InstanciatedComposableCallableImpl<N, Ns, std::index_sequence<Ns...>>::type...>;
};

using InstanciatedComposableCallable0 = typename InstanciatedComposableCallable<
    0, decltype(std::make_index_sequence<Composable::TYPE_SEQUENCE_INDEX_LIMIT>())>::type;
using InstanciatedComposableCallable1 = typename InstanciatedComposableCallable<
    1, decltype(std::make_index_sequence<Composable::TYPE_SEQUENCE_INDEX_LIMIT>())>::type;
