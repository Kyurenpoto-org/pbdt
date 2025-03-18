/*
 * SPDX - FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <tuple>

#include "pbdt/bdd.hpp"

#include "fixtures.hpp"

#define ASSERT_IDEMPOTENT(arg)                                                                                         \
    {                                                                                                                  \
        constexpr auto compileTimeProperty = arg;                                                                      \
        static_assert(compileTimeProperty == pbdt::bdd::then("", compileTimeProperty).complete());                     \
                                                                                                                       \
        const auto runTimeProperty = arg;                                                                              \
        dynamic_assert(runTimeProperty == pbdt::bdd::then("", runTimeProperty).complete());                            \
    }

void idempotent()
{
    ASSERT_IDEMPOTENT(pbdt::bdd::then("", natural_components::prop::freeFuncProp).complete());
    ASSERT_IDEMPOTENT(pbdt::bdd::then("", natural_components::prop::funcObjProp).complete());
    ASSERT_IDEMPOTENT(pbdt::bdd::then("", natural_components::prop::freeFuncProp)
                          .andThen("", natural_components::prop::funcObjProp)
                          .complete());
    ASSERT_IDEMPOTENT(pbdt::bdd::then("", natural_components::prop::funcObjProp)
                          .andThen("", natural_components::prop::freeFuncProp)
                          .complete());
}

namespace
{
    template <typename T>
    constexpr std::tuple<T> toTuple(const T t)
    {
        return { t };
    }

    template <typename... Props, typename Sample, typename Result>
    constexpr std::tuple<Props...>
    toTuple(const pbdt::bdd::detail::PropertyCompletion<std::tuple<Props...>, Sample, Result> completion)
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

    template <size_t N, typename Sample, typename Result>
    constexpr auto freeFunction(Sample, Result)
    {
        return pbdt::test_context::expect(N == 0);
    }

    template <size_t N, typename Sample, typename Result>
    struct Functor
    {
        constexpr auto operator()(Sample, Result)
        {
            return pbdt::test_context::expect(N == 0);
        }

        constexpr bool operator==(const Functor) const
        {
            return true;
        }
    };

    template <size_t N>
    struct IndexToTypeImpl;

    template <>
    struct IndexToTypeImpl<0>
    {
        using type = int;
    };

    template <>
    struct IndexToTypeImpl<1>
    {
        using type = std::string;
    };

    template <size_t N>
    using IndexToType = typename IndexToTypeImpl<N>::type;

    template <size_t, size_t>
    struct FoldableFunc;

    template <size_t N>
    struct FoldableFunc<0, N>
    {
        static constexpr auto value = &freeFunction<N % 2, IndexToType<N / 2 % 2>, IndexToType<N / 4 % 2>>;
    };

    template <size_t N>
    struct FoldableFunc<1, N>
    {
        static constexpr auto value = Functor<N % 2, IndexToType<N / 2 % 2>, IndexToType<N / 4 % 2>>{};
    };
}

namespace Associative
{
    template <size_t N, size_t M>
    struct FoldableCombination
    {
        static constexpr auto value = std::tuple{
            FoldableFunc<N % 2, M>::value,
            FoldableFunc<N / 2 % 2, M>::value,
            FoldableFunc<N / 4 % 2, M>::value,
        };
    };

    constexpr auto foldableCombinations = std::tuple{
        FoldableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        FoldableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        FoldableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        FoldableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        FoldableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        FoldableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        FoldableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        FoldableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        FoldableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        FoldableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        FoldableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        FoldableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        FoldableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        FoldableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        FoldableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        FoldableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
    };

    template <size_t... Idxs>
    constexpr void assertCombinations(const std::index_sequence<Idxs...>)
    {
        (
            []()
            {
                constexpr auto combination = std::get<Idxs>(foldableCombinations);

                constexpr auto a = std::get<0>(combination);
                constexpr auto b = std::get<1>(combination);
                constexpr auto c = std::get<2>(combination);

                constexpr auto compileTimeLhs =
                    pbdt::bdd::then("", a).andThen("", pbdt::bdd::then("", b).andThen("", c).complete()).complete();
                constexpr auto compileTimeRhs =
                    pbdt::bdd::then("", pbdt::bdd::then("", a).andThen("", b).complete()).andThen("", c).complete();
                static_assert(flatTuple(toTuple(compileTimeLhs)) == flatTuple(toTuple(compileTimeRhs)));

                const auto runTimeLhs =
                    pbdt::bdd::then("", a).andThen("", pbdt::bdd::then("", b).andThen("", c).complete()).complete();
                const auto runTimeRhs =
                    pbdt::bdd::then("", pbdt::bdd::then("", a).andThen("", b).complete()).andThen("", c).complete();
                dynamic_assert(flatTuple(toTuple(runTimeLhs)) == flatTuple(toTuple(runTimeRhs)));
            }(),
            ...
        );
    }
}

void associative()
{
    Associative::assertCombinations(
        std::make_index_sequence<std::tuple_size_v<decltype(Associative::foldableCombinations)>>()
    );
}

int main()
{
    // 2-Category that has terminal object and each 1-Hom-set has product between any two morphisms in it
    idempotent();
    associative();
    // Cannot test for identity

    return EXIT_SUCCESS;
}
