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
            return runnable.template beforeMorph<Idx>();
        };
    }

    template <size_t Idx>
    constexpr auto b() const
    {
        return []()
        {
            return runnable.template afterMorph<Idx>();
        };
    }

private:
    static constexpr TwoWayRunnableCombination runnable{};
};

#include "generators/runnable-double-combination.hpp"

template <typename Expect, typename RunnableScenario, typename Then>
struct TwoWayRunnableScenarioWithThenCombination
{
    static constexpr auto COMBINATIONS = Runnable::RunnableDoublePropCombinationSamples<
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value;
    static constexpr RunnableScenario runnableScenario{};
    static constexpr Then then{};

    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(COMBINATIONS)>;
    }

    constexpr auto morph(const auto& combination) const
    {
    }

    template <size_t Idx>
    constexpr bool beforeMorph() const
    {
        constexpr auto combination = std::get<Idx>(COMBINATIONS);

        return runnableScenario(
                   combination.target(),
                   (then(combination.template propA<Expect>()) + combination.template propB<Expect>()).complete(),
                   combination.domain()
        )
            .run()
            .passable();
    }

    template <size_t Idx>
    constexpr bool afterMorph() const
    {
        constexpr auto combination = std::get<Idx>(COMBINATIONS);

        return runnableScenario(combination.target(), combination.template propA<Expect>(), combination.domain())
                   .run()
                   .passable()
            && runnableScenario(combination.target(), combination.template propB<Expect>(), combination.domain())
                   .run()
                   .passable();
    }
};
