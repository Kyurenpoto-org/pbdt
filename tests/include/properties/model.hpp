/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "util.hpp"

struct ModelValidator
{
    void operator()(const auto production, const auto model) const
    {
        constexpr auto compileTimeProduction = production();
        constexpr auto compileTimeModel = model();
        static_assert(compileTimeProduction == compileTimeModel);

        const auto runTimeProduction = production();
        const auto runTimeModel = model();
        dynamic_assert(runTimeProduction == runTimeModel);
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
                return runnable.template production<Idx>();
            },
            []()
            {
                return runnable.template model<Idx>();
            }
        );

        if constexpr (Idx > 0)
        {
            acceptImpl<Idx - 1>(std::forward<Visitor>(visitor));
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
