/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "util.hpp"

template <typename TwoWayRunnableCombination>
struct ModelingValidation
{
    void run() const
    {
        runImpl<TwoWayRunnableCombination::size() - 1>();
    }

private:
    template <size_t Idx>
    void runImpl() const
    {
        twoWayAssert(
            []()
            {
                return runnable.template production<Idx>();
            },
            []()
            {
                return runnable.template model<Idx>();
            }
        );

        if constexpr (Idx > 0)
        {
            runImpl<Idx - 1>();
        }
    }

    static constexpr TwoWayRunnableCombination runnable{};
};

#include "generators/runnable-combination.hpp"

template <typename Expect, typename ApplyOrInvoke, typename RunnableScenario>
struct TwoWayRunnableScenarioCombination
{
    static constexpr auto COMBINATIONS = Runnable::RunnableCombinationSamples<
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value;
    static constexpr RunnableScenario runnableScenario{};
    static constexpr ApplyOrInvoke applyOrInvoke{};

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
};
