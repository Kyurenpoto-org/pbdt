/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "util.hpp"

template <typename ToComparable, typename TwoWayCompletableRawContext>
struct AssociativeValidation
{
    void run() const
    {
        runImpl<TwoWayCompletableRawContext::size() - 1>();
    }

private:
    template <size_t Idx>
    void runImpl() const
    {
        twoWayAssert(
            []()
            {
                return toComparable(completable.template l2rComplete<Idx>());
            },
            []()
            {
                return toComparable(completable.template r2lComplete<Idx>());
            }
        );

        if constexpr (Idx > 0)
        {
            runImpl<Idx - 1>();
        }
    }

    static constexpr TwoWayCompletableRawContext completable{};
    static constexpr ToComparable toComparable{};
};

#include "generators/composable-callable.hpp"
#include "generators/foldable-callable.hpp"
#include "generators/productable-container.hpp"

template <typename Given>
struct TwoWayCompletableRawGivenContext
{
    static constexpr auto COMBINATIONS = Composable::ComposableCombination<
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value;
    static constexpr Given given{};

    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(COMBINATIONS)> - 3 + 1;
    }

    template <size_t Idx>
    constexpr auto l2rComplete() const
    {
        constexpr auto a = std::get<Idx>(COMBINATIONS);
        constexpr auto b = std::get<Idx + 1>(COMBINATIONS);
        constexpr auto c = std::get<Idx + 2>(COMBINATIONS);

        return (given((given(a) + b).complete()) + c).complete();
    }

    template <size_t Idx>
    constexpr auto r2lComplete() const
    {
        constexpr auto a = std::get<Idx>(COMBINATIONS);
        constexpr auto b = std::get<Idx + 1>(COMBINATIONS);
        constexpr auto c = std::get<Idx + 2>(COMBINATIONS);

        return (given(a) + (given(b) + c).complete()).complete();
    }
};

template <typename When>
struct TwoWayCompletableRawWhenContext
{
    static constexpr auto COMBINATIONS = Productable::ProductableCombination<
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM()>::value;
    static constexpr When when{};

    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(COMBINATIONS)> - 3 + 1;
    }

    template <size_t Idx>
    constexpr auto l2rComplete() const
    {
        constexpr auto a = std::get<Idx>(COMBINATIONS);
        constexpr auto b = std::get<Idx + 1>(COMBINATIONS);
        constexpr auto c = std::get<Idx + 2>(COMBINATIONS);

        return (when((when(a()) + b()).complete()) + c()).complete();
    }

    template <size_t Idx>
    constexpr auto r2lComplete() const
    {
        constexpr auto a = std::get<Idx>(COMBINATIONS);
        constexpr auto b = std::get<Idx + 1>(COMBINATIONS);
        constexpr auto c = std::get<Idx + 2>(COMBINATIONS);

        return (when(a()) + (when(b()) + c()).complete()).complete();
    }
};

template <typename Expect, typename Then>
struct TwoWayCompletableRawThenContext
{
    static constexpr auto COMBINATIONS = Foldable::FoldableCombination<
        Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value;
    static constexpr Then then{};

    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(COMBINATIONS)> - 3 + 1;
    }

    template <size_t Idx>
    constexpr auto l2rComplete() const
    {
        constexpr auto a = std::get<Idx>(COMBINATIONS);
        constexpr auto b = std::get<Idx + 1>(COMBINATIONS);
        constexpr auto c = std::get<Idx + 2>(COMBINATIONS);

        return (then((then(a) + b).complete()) + c).complete();
    }

    template <size_t Idx>
    constexpr auto r2lComplete() const
    {
        constexpr auto a = std::get<Idx>(COMBINATIONS);
        constexpr auto b = std::get<Idx + 1>(COMBINATIONS);
        constexpr auto c = std::get<Idx + 2>(COMBINATIONS);

        return (then(a) + (then(b) + c).complete()).complete();
    }
};
