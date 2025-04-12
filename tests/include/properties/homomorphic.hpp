/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "util.hpp"

template <typename HomomorphicRequirements>
struct HomomorphicValidation : ValidationBase<HomomorphicValidation<HomomorphicRequirements>>
{
    static constexpr size_t size()
    {
        return HomomorphicRequirements::size();
    }

    template <size_t Idx>
    constexpr auto a() const
    {
        return []()
        {
            return requirements.template morph<Idx>(
                requirements.beforeMorphOp(requirements.template a<Idx>(), requirements.template b<Idx>())
            );
        };
    }

    template <size_t Idx>
    constexpr auto b() const
    {
        return []()
        {
            return requirements.afterMorphOp(
                requirements.template morph<Idx>(requirements.template a<Idx>()),
                requirements.template morph<Idx>(requirements.template b<Idx>())
            );
        };
    }

private:
    static constexpr HomomorphicRequirements requirements{};
};

#include "generators/runnable-double-combination.hpp"

template <typename Expect, typename RunnableScenario, typename Then>
struct HomomorphicRunnableScenarioWithThenRequirements
{
    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(COMBINATIONS)>;
    }

    template <size_t Idx>
    constexpr auto a() const
    {
        return std::get<Idx>(COMBINATIONS).template propA<Expect>();
    }

    template <size_t Idx>
    constexpr auto b() const
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
