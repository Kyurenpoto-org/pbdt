/*
 * SPDX - FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <type_traits>

#include "generators/values/composable-callable.hpp"

namespace CallableType
{
    template<typename Ret, typename... Args>
    using LambdaConceptInstance = decltype([](Args...) -> Ret
    {
        if constexpr (std::is_void_v<Ret>)
        {
            return;
        }
        else if constexpr (std::is_default_constructible_v<Ret>)
        {
            return Ret{};
        }
        else
        {
            return Ret::prototype();
        }
    });

    template <typename Ret, typename... Args>
    using FreeFunctionConceptInstance = Ret (*)(Args...);

    template <typename, typename, size_t>
    struct CallableConceptInstanceImpl;

    template <typename... Rets, typename... Args>
    struct CallableConceptInstanceImpl<std::tuple<Rets...>, std::tuple<Args...>, 0>
    {
        using type = LambdaConceptInstance<std::tuple<Rets...>, Args...>;
    };

    template <typename... Rets, typename... Args>
    struct CallableConceptInstanceImpl<std::tuple<Rets...>, std::tuple<Args...>, 1>
    {
        using type = FreeFunctionConceptInstance<std::tuple<Rets...>, Args...>;
    };

    template <typename Ret, typename... Args>
    struct CallableConceptInstanceImpl<std::tuple<Ret>, std::tuple<Args...>, 0>
    {
        using type = LambdaConceptInstance<Ret, Args...>;
    };

    template <typename Ret, typename... Args>
    struct CallableConceptInstanceImpl<std::tuple<Ret>, std::tuple<Args...>, 1>
    {
        using type = FreeFunctionConceptInstance<Ret, Args...>;
    };

    template <typename... Args>
    struct CallableConceptInstanceImpl<std::tuple<>, std::tuple<Args...>, 0>
    {
        using type = LambdaConceptInstance<void, Args...>;
    };

    template <typename... Args>
    struct CallableConceptInstanceImpl<std::tuple<>, std::tuple<Args...>, 1>
    {
        using type = FreeFunctionConceptInstance<void, Args...>;
    };

    constexpr size_t CALLABLE_CONCEPT_INSTANCE_INDEX_LIMIT = 2;

    template <size_t N, size_t First, size_t Second>
    using CallableTargetCombinationImpl = typename CallableConceptInstanceImpl<
        typename Composable::TypeSequence<Second % Composable::TYPE_SEQUENCE_INDEX_LIMIT>::type,
        typename Composable::TypeSequence<First % Composable::TYPE_SEQUENCE_INDEX_LIMIT>::type,
        N % CALLABLE_CONCEPT_INSTANCE_INDEX_LIMIT>::type;

    template <size_t N, size_t First, size_t Second, size_t... Rest>
    struct CallableTargetCombination
    {
        using type = decltype(std::tuple_cat(
            std::declval<std::tuple<CallableTargetCombinationImpl<N, First, Second>>>(),
            std::declval<
                typename CallableTargetCombination<N / CALLABLE_CONCEPT_INSTANCE_INDEX_LIMIT, Second, Rest...>::type>()
        ));
    };

    template <size_t N, size_t First, size_t Second, size_t Third>
    struct CallableTargetCombination<N, First, Second, Third>
    {
        using type = std::tuple<
            CallableTargetCombinationImpl<N, First, Second>,
            CallableTargetCombinationImpl<N / CALLABLE_CONCEPT_INSTANCE_INDEX_LIMIT, Second, Third>>;
    };

    constexpr size_t NON_CALLABLE_TARGET_CONCEPT_INSTANCE_INDEX_LIMIT = 3;

    template <size_t, size_t, size_t>
    struct NonCallableTargetCombinationImpl;

    template <size_t N, size_t M>
    struct NonCallableTargetCombinationImpl<0, N, M>
    {
        using type = typename CallableConceptInstanceImpl<
            std::tuple<>, typename Composable::TypeSequence<M % Composable::TYPE_SEQUENCE_INDEX_LIMIT>::type,
            N % CALLABLE_CONCEPT_INSTANCE_INDEX_LIMIT>::type;
    };

    template <size_t N, size_t M>
    struct NonCallableTargetCombinationImpl<1, N, M>
    {
        using type = typename CallableConceptInstanceImpl<
            typename Composable::TypeSequence<M % Composable::TYPE_SEQUENCE_INDEX_LIMIT>::type, std::tuple<>,
            N % CALLABLE_CONCEPT_INSTANCE_INDEX_LIMIT>::type;
    };

    template <size_t N, size_t M>
    struct NonCallableTargetCombinationImpl<2, N, M>
    {
        using type = typename CallableConceptInstanceImpl<
            std::tuple<>, std::tuple<>, N % CALLABLE_CONCEPT_INSTANCE_INDEX_LIMIT>::type;
    };

    template <size_t N, size_t First, size_t... Rest>
    struct NonCallableTargetCombination
    {
        using type = decltype(std::tuple_cat(
            std::declval<std::tuple<NonCallableTargetCombinationImpl<
                N % NON_CALLABLE_TARGET_CONCEPT_INSTANCE_INDEX_LIMIT,
                N / NON_CALLABLE_TARGET_CONCEPT_INSTANCE_INDEX_LIMIT, First>>>(),
            std::declval<typename NonCallableTargetCombination<
                N / NON_CALLABLE_TARGET_CONCEPT_INSTANCE_INDEX_LIMIT / CALLABLE_CONCEPT_INSTANCE_INDEX_LIMIT,
                Rest...>::type>()
        ));
    };

    template <size_t N, size_t M>
    struct NonCallableTargetCombination<N, M>
    {
        using type = std::tuple<NonCallableTargetCombinationImpl<
            N % NON_CALLABLE_TARGET_CONCEPT_INSTANCE_INDEX_LIMIT, N / NON_CALLABLE_TARGET_CONCEPT_INSTANCE_INDEX_LIMIT,
            M>>;
    };

    template <typename Result, size_t N, size_t M>
    using CallablePropCombinationImpl = typename CallableConceptInstanceImpl<
        std::tuple<Result>,
        std::tuple<
            typename Composable::TypeSequence<N % Composable::TYPE_SEQUENCE_INDEX_LIMIT>::type,
            typename Composable::TypeSequence<
                N / Composable::TYPE_SEQUENCE_INDEX_LIMIT % Composable::TYPE_SEQUENCE_INDEX_LIMIT>::type>,
        M % CALLABLE_CONCEPT_INSTANCE_INDEX_LIMIT>::type;

    template <typename Result, size_t N, size_t First, size_t... Rest>
    struct CallablePropCombination
    {
        using type = decltype(std::tuple_cat(
            std::declval<std::tuple<CallablePropCombinationImpl<Result, N, First>>>(),
            std::declval<typename CallablePropCombination<Result, N, Rest...>::type>()
        ));
    };

    template <typename Result, size_t N, size_t M>
    struct CallablePropCombination<Result, N, M>
    {
        using type = std::tuple<CallablePropCombinationImpl<Result, N, M>>;
    };

    constexpr size_t NON_CALLABLE_PROP_CONCEPT_INSTANCE_INDEX_LIMIT = 3;

    template <typename, size_t, size_t, size_t>
    struct NonCallablePropCombinationImpl;

    template <typename Result, size_t N, size_t M>
    struct NonCallablePropCombinationImpl<Result, 0, N, M>
    {
        using type = typename CallableConceptInstanceImpl<
            typename Composable::TypeSequence<N % Composable::TYPE_SEQUENCE_INDEX_LIMIT>::type,
            std::tuple<
                typename Composable::TypeSequence<N % Composable::TYPE_SEQUENCE_INDEX_LIMIT>::type,
                typename Composable::TypeSequence<
                    N / Composable::TYPE_SEQUENCE_INDEX_LIMIT % Composable::TYPE_SEQUENCE_INDEX_LIMIT>::type>,
            M % CALLABLE_CONCEPT_INSTANCE_INDEX_LIMIT>::type;
    };

    template <typename Result, size_t N, size_t M>
    struct NonCallablePropCombinationImpl<Result, 1, N, M>
    {
        using type = typename CallableConceptInstanceImpl<
            std::tuple<Result>,
            std::tuple<typename Composable::TypeSequence<N % Composable::TYPE_SEQUENCE_INDEX_LIMIT>::type>,
            M % CALLABLE_CONCEPT_INSTANCE_INDEX_LIMIT>::type;
    };

    template <typename Result, size_t N, size_t M>
    struct NonCallablePropCombinationImpl<Result, 2, N, M>
    {
        using type = typename CallableConceptInstanceImpl<
            std::tuple<Result>,
            std::tuple<
                typename Composable::TypeSequence<N % Composable::TYPE_SEQUENCE_INDEX_LIMIT>::type,
                typename Composable::TypeSequence<N % Composable::TYPE_SEQUENCE_INDEX_LIMIT>::type,
                typename Composable::TypeSequence<
                    N / Composable::TYPE_SEQUENCE_INDEX_LIMIT % Composable::TYPE_SEQUENCE_INDEX_LIMIT>::type>,
            M % CALLABLE_CONCEPT_INSTANCE_INDEX_LIMIT>::type;
    };

    template <typename Result, size_t N, size_t First, size_t... Rest>
    struct NonCallablePropCombination
    {
        using type = decltype(std::tuple_cat(
            std::declval<std::tuple<NonCallablePropCombinationImpl<
                Result, First % NON_CALLABLE_PROP_CONCEPT_INSTANCE_INDEX_LIMIT, N,
                First / NON_CALLABLE_PROP_CONCEPT_INSTANCE_INDEX_LIMIT>>>(),
            std::declval<typename NonCallablePropCombination<Result, N, Rest...>::type>()
        ));
    };

    template <typename Result, size_t N, size_t M>
    struct NonCallablePropCombination<Result, N, M>
    {
        using type = std::tuple<NonCallablePropCombinationImpl<
            Result, M % NON_CALLABLE_PROP_CONCEPT_INSTANCE_INDEX_LIMIT, N,
            M / NON_CALLABLE_PROP_CONCEPT_INSTANCE_INDEX_LIMIT>>;
    };
}
