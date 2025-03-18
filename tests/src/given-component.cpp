/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <ranges>
#include <tuple>

#include "prop-pp/bdd.hpp"

#include "fixtures.hpp"

namespace prop_pp::bdd::detail
{
    template <typename... Targets, typename Target>
    constexpr auto operator+(GivenContext<Targets...>&& context, Target&& target)
    {
        return context.andGiven("", std::forward<Target>(target));
    }
}

namespace
{
    template <typename T>
    constexpr std::tuple<T> toTuple(const T t)
    {
        return { t };
    }

    template <typename... Targets, typename Ret, typename Args>
    constexpr std::tuple<Targets...>
    toTuple(const prop_pp::bdd::detail::TargetCompletion<std::tuple<Targets...>, Ret, Args> completion)
    {
        return { completion };
    }

    constexpr auto flatTuple(auto&& tup)
    {
        return std::apply(
            [](const auto&... args)
            {
                return std::tuple_cat(toTuple(args)...);
            },
            tup
        );
    }

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

    constexpr size_t typeSequenceCount = std::tuple_size_v<TypeSequenceTable>;

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
        constexpr Ret operator()(Args...)
        {
            return Ret{};
        }

        constexpr bool operator==(const Functor) const
        {
            return true;
        }
    };

    template <size_t, typename, typename>
    struct ComposableFunc;

    template <typename... Rets, typename... Args>
    struct ComposableFunc<0, std::tuple<Rets...>, std::tuple<Args...>>
    {
        static constexpr std::tuple<Rets...> (*value)(Args...) = &freeFunction<std::tuple<Rets...>, Args...>;
    };

    template <typename Ret, typename... Args>
    struct ComposableFunc<0, std::tuple<Ret>, std::tuple<Args...>>
    {
        static constexpr Ret (*value)(Args...) = &freeFunction<Ret, Args...>;
    };

    template <typename... Rets, typename... Args>
    struct ComposableFunc<1, std::tuple<Rets...>, std::tuple<Args...>>
    {
        static constexpr Functor<std::tuple<Rets...>, Args...> value = Functor<std::tuple<Rets...>, Args...>{};
    };

    template <typename Ret, typename... Args>
    struct ComposableFunc<1, std::tuple<Ret>, std::tuple<Args...>>
    {
        static constexpr Functor<Ret, Args...> value = Functor<Ret, Args...>{};
    };

    template <size_t N, size_t First, size_t Second, size_t... Rest>
    struct ComposableCombination
    {
        static constexpr auto value = std::tuple_cat(
            std::tuple{
                ComposableFunc<
                    N % 2, typename TypeSequence<Second % typeSequenceCount>::type,
                    typename TypeSequence<First % typeSequenceCount>::type>::value,
            },
            ComposableCombination<N / 2, Second, Rest...>::value
        );
    };

    template <size_t N, size_t First, size_t Second, size_t Third>
    struct ComposableCombination<N, First, Second, Third>
    {
        static constexpr auto value = std::tuple{
            ComposableFunc<
                N % 2, typename TypeSequence<Second % typeSequenceCount>::type,
                typename TypeSequence<First % typeSequenceCount>::type>::value,
            ComposableFunc<
                N / 2 % 2, typename TypeSequence<Third % typeSequenceCount>::type,
                typename TypeSequence<Second % typeSequenceCount>::type>::value,
        };
    };
}

namespace Idempotent
{
    constexpr auto rawContexts = std::tuple{
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
    };

    template <size_t... Idxs>
    constexpr void assertCombinations(const std::index_sequence<Idxs...>)
    {
        (
            []()
            {
                constexpr auto completeContext = [&]()
                {
                    return std::apply(
                        []<typename Target, typename... Targets>(Target&& target, Targets&&... targets)
                        {
                            return (prop_pp::bdd::given("", std::forward<Target>(target)) + ...
                                    + std::forward<Targets>(targets))
                                .complete();
                        },
                        std::get<Idxs>(rawContexts)
                    );
                };

                constexpr auto compileTimeCompleted = completeContext();
                static_assert(compileTimeCompleted == prop_pp::bdd::given("", compileTimeCompleted).complete());

                const auto runTimeCompleted = completeContext();
                dynamic_assert(runTimeCompleted == prop_pp::bdd::given("", runTimeCompleted).complete());
            }(),
            ...
        );
    }
}

void idempotent()
{
    Idempotent::assertCombinations(std::make_index_sequence<std::tuple_size_v<decltype(Idempotent::rawContexts)>>());
}

namespace Associative
{
    constexpr auto composableCombinations = std::tuple{
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
    };

    template <size_t... Idxs>
    constexpr void assertCombinations(const std::index_sequence<Idxs...>)
    {
        (
            []()
            {
                constexpr auto combination = std::get<Idxs>(composableCombinations);

                constexpr auto a = std::get<0>(combination);
                constexpr auto b = std::get<1>(combination);
                constexpr auto c = std::get<2>(combination);

                constexpr auto compileTimeLhs = prop_pp::bdd::given("", a)
                                                    .andGiven("", prop_pp::bdd::given("", b).andGiven("", c).complete())
                                                    .complete();
                constexpr auto compileTimeRhs =
                    prop_pp::bdd::given("", prop_pp::bdd::given("", a).andGiven("", b).complete())
                        .andGiven("", c)
                        .complete();
                static_assert(flatTuple(toTuple(compileTimeLhs)) == flatTuple(toTuple(compileTimeRhs)));

                const auto runTimeLhs = prop_pp::bdd::given("", a)
                                            .andGiven("", prop_pp::bdd::given("", b).andGiven("", c).complete())
                                            .complete();
                const auto runTimeRhs = prop_pp::bdd::given("", prop_pp::bdd::given("", a).andGiven("", b).complete())
                                            .andGiven("", c)
                                            .complete();
                dynamic_assert(flatTuple(toTuple(runTimeLhs)) == flatTuple(toTuple(runTimeRhs)));
            }(),
            ...
        );
    }
}

void associative()
{
    Associative::assertCombinations(
        std::make_index_sequence<std::tuple_size_v<decltype(Associative::composableCombinations)>>()
    );
}

int main()
{
    // Set Category
    idempotent();
    associative();
    // Cannot test for identity

    return EXIT_SUCCESS;
}
