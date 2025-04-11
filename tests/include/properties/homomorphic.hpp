/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "util.hpp"

struct HomomorphicValidator
{
    void operator()(auto beforeMorph, auto afterMorph) const
    {
        constexpr auto compileTimeBeforeMorph = beforeMorph();
        constexpr auto compileTimeAfterMorph = afterMorph();
        static_assert(compileTimeBeforeMorph == compileTimeAfterMorph);

        const auto runTimeBeforeMorph = beforeMorph();
        const auto runTimeAfterMorph = afterMorph();
        dynamic_assert(runTimeBeforeMorph == runTimeAfterMorph);
    }
};

template <typename TwoWayRunnableCombination>
struct AcceptableCombination
{
    template <typename Visitor>
    void accept(Visitor&& visitor) const
    {
        acceptImpl<TwoWayRunnableCombination::size() - 1>(std::forward<Visitor>(visitor));
    }

private:
    template <size_t Idx, typename Visitor>
    void acceptImpl(Visitor&& visitor) const
    {
        visitor(
            []()
            {
                return runnable.template beforeMorph<Idx>();
            },
            []()
            {
                return runnable.template afterMorph<Idx>();
            }
        );

        if constexpr (Idx > 0)
        {
            acceptImpl<Idx - 1>(std::forward<Visitor>(visitor));
        }
    }

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
