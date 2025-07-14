/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include "util.hpp"

template <typename HomomorphicRequirements>
struct HomomorphicValueValidation : ValueValidationBase<HomomorphicValueValidation<HomomorphicRequirements>>
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

#include "generators/values/runnable-double-combination.hpp"

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

    template <size_t Idx, typename Prop>
    constexpr auto morph(Prop&& prop) const
    {
        constexpr auto combination = std::get<Idx>(COMBINATIONS);

        return runnableScenario(combination.target(), std::forward<Prop>(prop), combination.domain()).run().passable();
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

template <typename EventCountable>
struct EventCountableDoubleValueCombination
{
    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(OPS)>;
    }

    template <size_t Idx>
    constexpr auto a() const
    {
        return value<A[Idx]>();
    }

    template <size_t Idx>
    constexpr auto b() const
    {
        return value<B[Idx]>();
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
    static constexpr std::array<size_t, 8> A{
        COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8,
        COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8,
    };
    static constexpr std::array<size_t, 8> B{
        COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8,
        COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8, COMPILE_TIME_RANDOM() % 8,
    };
    static constexpr EventCountable eventCountable{};
};

template <typename EventCountable>
struct HomomorphicEventCountableSumWithAccumulateRequirements
{
    static constexpr size_t size()
    {
        return COMBINATIONS.size();
    }

    template <size_t Idx>
    constexpr auto a() const
    {
        return COMBINATIONS.template a<Idx>();
    }

    template <size_t Idx>
    constexpr auto b() const
    {
        return COMBINATIONS.template b<Idx>();
    }

    template <size_t, typename T>
    constexpr size_t morph(T&& t) const
    {
        return std::forward<T>(t).sum();
    }

    template <typename T>
    constexpr auto beforeMorphOp(T&& a, T&& b) const
    {
        return std::forward<T>(a) + std::forward<T>(b);
    }

    constexpr size_t afterMorphOp(size_t a, size_t b) const
    {
        return a + b;
    }

private:
    static constexpr auto COMBINATIONS = EventCountableDoubleValueCombination<EventCountable>{};
};

template <typename EventCountable>
struct HomomorphicEventCountableSomeFailedWithAccumulateRequirements
{
    static constexpr size_t size()
    {
        return COMBINATIONS.size();
    }

    template <size_t Idx>
    constexpr auto a() const
    {
        return COMBINATIONS.template a<Idx>();
    }

    template <size_t Idx>
    constexpr auto b() const
    {
        return COMBINATIONS.template b<Idx>();
    }

    template <size_t, typename T>
    constexpr bool morph(T&& t) const
    {
        return std::forward<T>(t).someFailed();
    }

    template <typename T>
    constexpr auto beforeMorphOp(T&& a, T&& b) const
    {
        return std::forward<T>(a) + std::forward<T>(b);
    }

    constexpr bool afterMorphOp(bool a, bool b) const
    {
        return a || b;
    }

private:
    static constexpr auto COMBINATIONS = EventCountableDoubleValueCombination<EventCountable>{};
};
