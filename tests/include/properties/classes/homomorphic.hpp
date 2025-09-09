/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include "util.hpp"

/**
 * @brief A value validation structure that checks homomorphic requirements.
 *
 * @details Homomorphic property: morph(a op b) == morph(a) op' morph(b).
 *
 * @tparam HomomorphicRequirements
 *
 * @see TwoWayValueValidationBase
 */
template <typename HomomorphicRequirements>
struct HomomorphicValueValidation : TwoWayValueValidationBase<HomomorphicValueValidation<HomomorphicRequirements>>
{
    /**
     * @brief The size of index range.
     *
     * @details Same as HomomorphicRequirements::size().
     *
     * @return constexpr size_t
     */
    static constexpr size_t size()
    {
        return HomomorphicRequirements::size();
    }

    /**
     * @brief Construct the operand-before-morph callable for the given index.
     *
     * @tparam Idx
     * @return constexpr auto The callable that same as () -> decltype(morph(beforeMorphOp(a, b))).
     */
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

    /**
     * @brief Construct the operand-after-morph callable for the given index.
     *
     * @tparam Idx
     * @return constexpr auto The callable that same as () -> decltype(afterMorphOp(morph(a), morph(b))).
     */
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

#include "generators/values/event-countable-combination.hpp"
#include "generators/values/runnable-double-combination.hpp"

/**
 * @brief A structure that defines homomorphic requirements for RunnableScenario<..., Then>::run().passable().
 *
 * @tparam Expect
 * @tparam RunnableScenario
 * @tparam Then
 */
template <typename Expect, typename RunnableScenario, typename Then>
struct HomomorphicRunnableScenarioWithThenRequirements
{
    /**
     * @brief The size of index range.
     *
     * @return constexpr size_t
     */
    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(COMBINATIONS)>;
    }

    /**
     * @brief Get the property object A for the given index.
     *
     * @tparam Idx
     * @return constexpr auto The callable that same as () -> decltype(propA()).
     */
    template <size_t Idx>
    constexpr auto a() const
    {
        return std::get<Idx>(COMBINATIONS).template propA<Expect>();
    }

    /**
     * @brief Get the property object B for the given index.
     *
     * @tparam Idx
     * @return constexpr auto The callable that same as () -> decltype(propA()).
     */
    template <size_t Idx>
    constexpr auto b() const
    {
        return std::get<Idx>(COMBINATIONS).template propB<Expect>();
    }

    /**
     * @brief Construct the RunnableScenario object from property object and check passable for given index.
     *
     * @tparam Idx
     * @tparam Prop
     * @return constexpr bool
     */
    template <size_t Idx, typename Prop>
    constexpr bool morph(Prop&& prop) const
    {
        constexpr auto combination = std::get<Idx>(COMBINATIONS);

        return runnableScenario(combination.target(), std::forward<Prop>(prop), combination.domain()).run().passable();
    }

    /**
     * @brief Compute then-before-run expression.
     *
     * @tparam A
     * @tparam B
     * @param a
     * @param b
     * @return constexpr auto The callable that same as propA or propB
     */
    template <typename A, typename B>
    constexpr auto beforeMorphOp(A&& a, B&& b) const
    {
        return (then(std::forward<A>(a)) + std::forward<B>(b)).complete();
    }

    /**
     * @brief Compute and-after-run expression.
     *
     * @param a
     * @param b
     * @return bool
     */
    constexpr bool afterMorphOp(const bool a, const bool b) const
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

/**
 * @brief A structure that defines homomorphic requirements for EventCountable::sum()
 * with operator+(EventCountable, EventCountable).
 *
 * @tparam EventCountable
 */
template <typename EventCountable>
struct HomomorphicEventCountableSumWithAccumulateRequirements
{
    /**
     * @brief The size of index range.
     *
     * @return constexpr size_t
     */
    static constexpr size_t size()
    {
        return COMBINATIONS.size();
    }

    /**
     * @brief Get the EventCountable object A for given index.
     *
     * @tparam Idx
     * @return constexpr auto
     */
    template <size_t Idx>
    constexpr EventCountable a() const
    {
        return COMBINATIONS.template a<Idx>();
    }

    /**
     * @brief Get the EventCountable object B for given index.
     *
     * @tparam Idx
     * @return constexpr auto
     */
    template <size_t Idx>
    constexpr EventCountable b() const
    {
        return COMBINATIONS.template b<Idx>();
    }

    /**
     * @brief Compute sum of all event counts.
     *
     * @tparam size_t Ignored parameter
     * @param events
     * @return constexpr size_t
     */
    template <size_t>
    constexpr size_t morph(const EventCountable events) const
    {
        return events.sum();
    }

    /**
     * @brief Compute accumulate-before-sum expression.
     *
     * @param a
     * @param b
     * @return EventCountable
     */
    constexpr EventCountable beforeMorphOp(const EventCountable a, const EventCountable b) const
    {
        return a + b;
    }

    /**
     * @brief Compute accumulate-after-sum expression.
     *
     * @param a
     * @param b
     * @return constexpr size_t
     */
    constexpr size_t afterMorphOp(const size_t a, const size_t b) const
    {
        return a + b;
    }

private:
    static constexpr auto COMBINATIONS = Countable::EventCountableDoubleValueCombination<
        EventCountable, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{};
};

/**
 * @brief A structure that defines homomorphic requirements for EventCountable::someFailed()
 * with operator+(EventCountable, EventCountable).
 *
 * @tparam EventCountable
 */
template <typename EventCountable>
struct HomomorphicEventCountableSomeFailedWithAccumulateRequirements
{
    /**
     * @brief The size of index range.
     *
     * @return constexpr size_t
     */
    static constexpr size_t size()
    {
        return COMBINATIONS.size();
    }

    /**
     * @brief Get the EventCountable object A for given index.
     *
     * @tparam Idx
     * @return constexpr EventCountable
     */
    template <size_t Idx>
    constexpr EventCountable a() const
    {
        return COMBINATIONS.template a<Idx>();
    }

    /**
     * @brief Get the EventCountable object B for given index.
     *
     * @tparam Idx
     * @return constexpr EventCountable
     */
    template <size_t Idx>
    constexpr EventCountable b() const
    {
        return COMBINATIONS.template b<Idx>();
    }

    /**
     * @brief Check if some events failed.
     *
     * @tparam size_t Ignored parameter
     * @param events
     * @return constexpr bool
     */
    template <size_t>
    constexpr bool morph(const EventCountable events) const
    {
        return events.someFailed();
    }

    /**
     * @brief Compute accumulate-before-some-failed expression.
     *
     * @param a
     * @param b
     * @return EventCountable
     */
    constexpr EventCountable beforeMorphOp(const EventCountable a, const EventCountable b) const
    {
        return a + b;
    }

    /**
     * @brief Compute accumulate-after-some-failed expression.
     *
     * @param a
     * @param b
     * @return EventCountable
     */
    constexpr bool afterMorphOp(const bool a, const bool b) const
    {
        return a || b;
    }

private:
    static constexpr auto COMBINATIONS = Countable::EventCountableDoubleValueCombination<
        EventCountable, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{};
};
