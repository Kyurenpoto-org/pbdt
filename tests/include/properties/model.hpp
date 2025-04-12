/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "util.hpp"

template <typename TwoWayRunnableCombination>
struct ModelingValidation : ValidationBase<ModelingValidation<TwoWayRunnableCombination>>
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
            return runnable.template production<Idx>();
        };
    }

    template <size_t Idx>
    constexpr auto b() const
    {
        return []()
        {
            return runnable.template model<Idx>();
        };
    }

private:
    static constexpr TwoWayRunnableCombination runnable{};
};

#include "generators/runnable-combination.hpp"

template <typename Expect, typename ApplyOrInvoke, typename RunnableScenario>
struct TwoWayRunnableScenarioCombination
{
    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(COMBINATIONS)>;
    }

    template <size_t Idx>
    constexpr bool production() const
    {
        constexpr auto combination = std::get<Idx>(COMBINATIONS);

        return runnableScenario(combination.target(), combination.template prop<Expect>(), combination.domain())
            .run()
            .passable();
    }

    template <size_t Idx>
    constexpr bool model() const
    {
        constexpr auto combination = std::get<Idx>(COMBINATIONS);
        for (const auto& sample : combination.domain())
        {
            if (!combination.template prop<Expect>()(sample, applyOrInvoke(combination.target(), sample)).passable())
            {
                return false;
            }
        }

        return true;
    }

private:
    static constexpr auto COMBINATIONS = Runnable::RunnableCombinationSamples<
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value;
    static constexpr RunnableScenario runnableScenario{};
    static constexpr ApplyOrInvoke applyOrInvoke{};
};
