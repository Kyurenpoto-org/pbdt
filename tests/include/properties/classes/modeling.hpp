/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include "util.hpp"

template <typename ModelingRequirements>
struct ModelingValueValidation : ValueValidationBase<ModelingValueValidation<ModelingRequirements>>
{
    static constexpr size_t size()
    {
        return ModelingRequirements::size();
    }

    template <size_t Idx>
    constexpr auto a() const
    {
        return []()
        {
            return requirements.template production<Idx>();
        };
    }

    template <size_t Idx>
    constexpr auto b() const
    {
        return []()
        {
            return requirements.template model<Idx>();
        };
    }

private:
    static constexpr ModelingRequirements requirements{};
};

#include "generators/values/runnable-combination.hpp"

template <typename Expect, typename ApplyOrInvoke, typename RunnableScenario>
struct ModelingRunnableScenarioRequirements
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

template <typename EventCountable>
struct ModelingEventCountableSumRequirements
{
    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(OPS)>;
    }

    template <size_t Idx>
    constexpr bool production() const
    {
        return compute<POST[Idx]>(value<PRE[Idx]>()).sum();
    }

    template <size_t Idx>
    constexpr bool model() const
    {
        return value<PRE[Idx]>().sum() + Idx + 1;
    }

private:
    template <size_t Idx>
    constexpr auto value() const
    {
        if constexpr (Idx == 0)
        {
            return eventCountable.prototype();
        }
        else
        {
            return OPS[Idx - 1](value<Idx - 1>());
        }
    }

    template <size_t Idx>
    constexpr auto compute(auto x) const
    {
        if constexpr (Idx == 0)
        {
            return OPS[Idx](x);
        }
        else
        {
            return OPS[Idx](compute<Idx - 1>(x));
        }
    }

    static constexpr auto OPS = std::array<typename EventCountable::OpContainer, 8>{
        EventCountable::template Op<COMPILE_TIME_RANDOM() % 3>(),
        EventCountable::template Op<COMPILE_TIME_RANDOM() % 3>(),
        EventCountable::template Op<COMPILE_TIME_RANDOM() % 3>(),
        EventCountable::template Op<COMPILE_TIME_RANDOM() % 3>(),
        EventCountable::template Op<COMPILE_TIME_RANDOM() % 3>(),
        EventCountable::template Op<COMPILE_TIME_RANDOM() % 3>(),
        EventCountable::template Op<COMPILE_TIME_RANDOM() % 3>(),
        EventCountable::template Op<COMPILE_TIME_RANDOM() % 3>(),
    };
    static constexpr std::array<size_t, 8> PRE{
        COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8,
        COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8,
    };
    static constexpr std::array<size_t, 8> POST{
        COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8,
        COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8,
    };
    static constexpr EventCountable eventCountable{};
};
