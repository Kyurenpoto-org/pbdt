/*
 * SPDX - FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <type_traits>

#include "generators/values/composable-callable.hpp"

namespace Callable
{
    template<typename Ret, typename... Args>
    using LambdaConceptInstance = decltype([](Args...) -> Ret
    {
        if constexpr (std::is_void_v<Ret>)
        {
            return;
        }
        else
        {
            return Ret{};
        }
    });

    template <typename Ret, typename... Args>
    using FreeFunctionConceptInstance = Ret(Args...);

    template <typename, typename, size_t>
    struct CallableConceptInstanceImpl;

    template <typename... Rets, typename... Args, size_t N>
    struct CallableConceptInstanceImpl<std::tuple<Rets...>, std::tuple<Args...>, 0>
    {
        using type = LambdaConceptInstance<std::tuple<Rets...>, Args...>;
    };

    template <typename... Rets, typename... Args, size_t N>
    struct CallableConceptInstanceImpl<std::tuple<Rets...>, std::tuple<Args...>, 1>
    {
        using type = FreeFunctionConceptInstance<std::tuple<Rets...>, Args...>;
    };

    template <typename Ret, typename... Args, size_t N>
    struct CallableConceptInstanceImpl<std::tuple<Ret>, std::tuple<Args...>, 0>
    {
        using type = LambdaConceptInstance<Ret, Args...>;
    };

    template <typename Ret, typename... Args, size_t N>
    struct CallableConceptInstanceImpl<std::tuple<Ret>, std::tuple<Args...>, 1>
    {
        using type = FreeFunctionConceptInstance<Ret, Args...>;
    };

    template <typename... Args, size_t N>
    struct CallableConceptInstanceImpl<std::tuple<>, std::tuple<Args...>, 0>
    {
        using type = LambdaConceptInstance<void, Args...>;
    };

    template <typename... Args, size_t N>
    struct CallableConceptInstanceImpl<std::tuple<>, std::tuple<Args...>, 1>
    {
        using type = FreeFunctionConceptInstance<void, Args...>;
    };

    constexpr size_t CALLABLE_CONCEPT_INSTANCE_INDEX_LIMIT = 2;

    template <size_t N>
    using CallableTargetConceptInstance = typename CallableConceptInstanceImpl<
        typename Composable::TypeSequence<
            N / CALLABLE_CONCEPT_INSTANCE_INDEX_LIMIT % Composable::TYPE_SEQUENCE_INDEX_LIMIT>::type,
        typename Composable::TypeSequence<
            N / CALLABLE_CONCEPT_INSTANCE_INDEX_LIMIT / Composable::TYPE_SEQUENCE_INDEX_LIMIT>::type,
        N % CALLABLE_CONCEPT_INSTANCE_INDEX_LIMIT>::type;

    template <typename Expect, size_t N>
    using CallablePropConceptInstance = typename CallableConceptInstanceImpl<
        Expect,
        std::tuple<
            typename Composable::TypeSequence<
                N / CALLABLE_CONCEPT_INSTANCE_INDEX_LIMIT % Composable::TYPE_SEQUENCE_INDEX_LIMIT>::type,
            typename Composable::TypeSequence<
                N / CALLABLE_CONCEPT_INSTANCE_INDEX_LIMIT / Composable::TYPE_SEQUENCE_INDEX_LIMIT>::type>,
        N % CALLABLE_CONCEPT_INSTANCE_INDEX_LIMIT>::type;
}
