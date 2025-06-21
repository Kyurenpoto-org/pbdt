/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <ranges>
#include <string>
#include <tuple>

namespace Composable
{
    struct TestStruct
    {
    };

    using TypeSequenceTable = std::tuple<
        std::tuple<int>, std::tuple<std::string>, std::tuple<TestStruct>, std::tuple<int, int>,
        std::tuple<int, std::string>, std::tuple<int, TestStruct>, std::tuple<std::string, int>,
        std::tuple<std::string, std::string>, std::tuple<std::string, TestStruct>, std::tuple<TestStruct, int>,
        std::tuple<TestStruct, std::string>, std::tuple<TestStruct, TestStruct>, std::tuple<int, int, int>,
        std::tuple<int, int, std::string>, std::tuple<int, int, TestStruct>, std::tuple<int, std::string, int>,
        std::tuple<int, std::string, std::string>, std::tuple<int, std::string, TestStruct>,
        std::tuple<int, TestStruct, int>, std::tuple<int, TestStruct, std::string>,
        std::tuple<int, TestStruct, TestStruct>, std::tuple<std::string, int, int>,
        std::tuple<std::string, int, std::string>, std::tuple<std::string, int, TestStruct>,
        std::tuple<std::string, std::string, int>, std::tuple<std::string, std::string, std::string>,
        std::tuple<std::string, std::string, TestStruct>, std::tuple<std::string, TestStruct, int>,
        std::tuple<std::string, TestStruct, std::string>, std::tuple<std::string, TestStruct, TestStruct>,
        std::tuple<TestStruct, int, int>, std::tuple<TestStruct, int, std::string>,
        std::tuple<TestStruct, int, TestStruct>, std::tuple<TestStruct, std::string, int>,
        std::tuple<TestStruct, std::string, std::string>, std::tuple<TestStruct, std::string, TestStruct>,
        std::tuple<TestStruct, TestStruct, int>, std::tuple<TestStruct, TestStruct, std::string>,
        std::tuple<TestStruct, TestStruct, TestStruct>>;

    constexpr size_t TYPE_SEQUENCE_INDEX_LIMIT = std::tuple_size_v<TypeSequenceTable>;
    constexpr size_t TYPE_SEQUENCE_DOUBLE_INDEX_LIMIT = TYPE_SEQUENCE_INDEX_LIMIT * TYPE_SEQUENCE_INDEX_LIMIT;

    template <size_t N>
    struct TypeSequence
    {
        using type = std::tuple_element_t<N, TypeSequenceTable>;
    };

    template <typename Ret, typename... Args>
    constexpr Ret freeFunction(Args...)
    {
        return Ret{};
    }

    template <typename Ret, typename... Args>
    struct Functor
    {
        constexpr Ret operator()(Args...) const
        {
            return Ret{};
        }

        constexpr bool operator==(const Functor) const
        {
            return true;
        }
    };

    template <size_t, typename, typename>
    struct ComposableCallable;

    template <typename... Rets, typename... Args>
    struct ComposableCallable<0, std::tuple<Rets...>, std::tuple<Args...>>
    {
        static constexpr std::tuple<Rets...> (*value)(Args...) = &freeFunction<std::tuple<Rets...>, Args...>;
    };

    template <typename Ret, typename... Args>
    struct ComposableCallable<0, std::tuple<Ret>, std::tuple<Args...>>
    {
        static constexpr Ret (*value)(Args...) = &freeFunction<Ret, Args...>;
    };

    template <typename... Rets, typename... Args>
    struct ComposableCallable<1, std::tuple<Rets...>, std::tuple<Args...>>
    {
        static constexpr Functor<std::tuple<Rets...>, Args...> value = Functor<std::tuple<Rets...>, Args...>{};
    };

    template <typename Ret, typename... Args>
    struct ComposableCallable<1, std::tuple<Ret>, std::tuple<Args...>>
    {
        static constexpr Functor<Ret, Args...> value = Functor<Ret, Args...>{};
    };

    template <size_t N, size_t First, size_t Second, size_t... Rest>
    struct ComposableCombination
    {
        static constexpr auto value = std::tuple_cat(
            std::tuple{
                ComposableCallable<
                    N % 2, typename TypeSequence<Second % TYPE_SEQUENCE_INDEX_LIMIT>::type,
                    typename TypeSequence<First % TYPE_SEQUENCE_INDEX_LIMIT>::type>::value,
            },
            ComposableCombination<N / 2, Second, Rest...>::value
        );
    };

    template <size_t N, size_t First, size_t Second, size_t Third>
    struct ComposableCombination<N, First, Second, Third>
    {
        static constexpr auto value = std::tuple{
            ComposableCallable<
                N % 2, typename TypeSequence<Second % TYPE_SEQUENCE_INDEX_LIMIT>::type,
                typename TypeSequence<First % TYPE_SEQUENCE_INDEX_LIMIT>::type>::value,
            ComposableCallable<
                N / 2 % 2, typename TypeSequence<Third % TYPE_SEQUENCE_INDEX_LIMIT>::type,
                typename TypeSequence<Second % TYPE_SEQUENCE_INDEX_LIMIT>::type>::value,
        };
    };
}
