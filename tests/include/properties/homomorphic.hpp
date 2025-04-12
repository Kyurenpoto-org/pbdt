/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "util.hpp"

template <typename TwoWayRunnableCombination>
struct HomomorphicValidation : ValidationBase<HomomorphicValidation<TwoWayRunnableCombination>>
{
    static constexpr size_t size()
    {
        return TwoWayRunnableCombination::size();
    }

    template <size_t Idx>
    constexpr auto a() const
    {
        return []()
        {
            return runnable.template morph<Idx>(
                runnable.beforeMorphOp(runnable.template propA<Idx>(), runnable.template propB<Idx>())
            );
        };
    }

    template <size_t Idx>
    constexpr auto b() const
    {
        return []()
        {
            return runnable.afterMorphOp(
                runnable.template morph<Idx>(runnable.template propA<Idx>()),
                runnable.template morph<Idx>(runnable.template propB<Idx>())
            );
        };
    }

private:
    static constexpr TwoWayRunnableCombination runnable{};
};

#include "generators/runnable-double-combination.hpp"

template <typename Expect, typename RunnableScenario, typename Then>
struct TwoWayRunnableScenarioWithThenCombination
{
    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(COMBINATIONS)>;
    }

    template <size_t Idx>
    constexpr auto propA() const
    {
        return std::get<Idx>(COMBINATIONS).template propA<Expect>();
    }

    template <size_t Idx>
    constexpr auto propB() const
    {
        return std::get<Idx>(COMBINATIONS).template propB<Expect>();
    }

    template <size_t Idx>
    constexpr auto morph(auto&& prop) const
    {
        constexpr auto combination = std::get<Idx>(COMBINATIONS);

        return runnableScenario(combination.target(), prop, combination.domain()).run().passable();
    }

    template <typename A, typename B>
    constexpr auto beforeMorphOp(A&& a, B&& b) const
    {
        return (then(std::forward<A>(a)) + std::forward<B>(b)).complete();
    }

    constexpr auto afterMorphOp(auto&& a, auto&& b) const
    {
        return a && b;
    }

private:
    static constexpr auto COMBINATIONS = Runnable::RunnableDoublePropCombinationSamples<
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value;
    static constexpr RunnableScenario runnableScenario{};
    static constexpr Then then{};
};
