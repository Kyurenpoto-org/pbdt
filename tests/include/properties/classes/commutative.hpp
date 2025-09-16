/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include "properties/util.hpp"

/**
 * @brief A value validation structure that checks commutative properties in run-time.
 *
 * @details Commutative property: a op b == b op a.
 *
 * @tparam CommutativeRequirements
 *
 * @see RunTimeValueValidationBase
 */
template <typename CommutativeRequirements>
struct CommutativeRunTimeValueValidation :
    RunTimeValueValidationBase<CommutativeRunTimeValueValidation<CommutativeRequirements>>
{
    /**
     * @brief The size of index range.
     *
     * @details Same as CommutativeRequirements::size().
     *
     * @return constexpr size_t
     */
    static constexpr size_t size()
    {
        return CommutativeRequirements::size();
    }

    /**
     * @brief Construct the forward callable for the given index.
     *
     * @tparam IDX
     * @return constexpr auto The callable that same as () -> decltype(toComparable(closedOp(a, b))).
     */
    template <size_t Idx>
    auto a() const
    {
        return []()
        {
            return requirements.toComparable(
                requirements.closedOp(requirements.template a<Idx>(), requirements.template b<Idx>())
            );
        };
    }

    /**
     * @brief Construct the backward callable for the given index.
     *
     * @tparam IDX
     * @return constexpr auto The callable that same as () -> decltype(toComparable(closedOp(a, b))).
     */
    template <size_t Idx>
    auto b() const
    {
        return []()
        {
            return requirements.toComparable(
                requirements.closedOp(requirements.template b<Idx>(), requirements.template a<Idx>())
            );
        };
    }

private:
    static constexpr CommutativeRequirements requirements{};
};

#include "generators/values/event-countable-combination.hpp"
#include "generators/values/expectation-context-combination.hpp"

/**
 * @brief A structure that defines commutative requirements for EventCountable::operator std::string() with
 * operator+(EventCountable, EventCountable).
 *
 * @tparam EventCountable
 */
template <typename EventCountable>
struct CommutativeEventCountableRequirements
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
     * @tparam IDX
     * @return EventCountable
     */
    template <size_t IDX>
    auto a() const
    {
        return COMBINATIONS.template a<IDX>();
    }

    /**
     * @brief Get the EventCountable object B for given index.
     *
     * @tparam IDX
     * @return EventCountable
     */
    template <size_t IDX>
    auto b() const
    {
        return COMBINATIONS.template b<IDX>();
    }

    /**
     * @brief Compute accumulate expression.
     *
     * @param a
     * @param b
     * @return EventCountable
     */
    EventCountable closedOp(const EventCountable a, const EventCountable b) const
    {
        return a + b;
    }

    /**
     * @brief Convert result to comparable type.
     *
     * @param events
     * @return std::string
     */
    std::string toComparable(const EventCountable events) const
    {
        return events;
    }

private:
    static constexpr auto COMBINATIONS = Countable::EventCountableTripleValueCombination<
        EventCountable, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{};
};

/**
 * @brief A structure that defines commutative requirements for ExpectationContext<N>::countedEvents() with
 * operator+(ExpectationContext<N>, ExpectationContext<M>).
 *
 * @tparam ExpectationContext
 * @tparam EventCountable
 */
template <template <size_t> typename ExpectationContext, typename EventCountable>
struct CommutativeExpectationContextRequirements
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
     * @brief Get the ExpectationContext object A for given index.
     *
     * @tparam IDX
     * @return ExpectationContext<N> N is arbitrary size
     */
    template <size_t IDX>
    auto a() const
    {
        return COMBINATIONS.template a<IDX>();
    }

    /**
     * @brief Get the ExpectationContext object B for given index.
     *
     * @tparam IDX
     * @return ExpectationContext<N> N is arbitrary size
     */
    template <size_t IDX>
    auto b() const
    {
        return COMBINATIONS.template b<IDX>();
    }

    /**
     * @brief Compute accumulate expression.
     *
     * @tparam N
     * @tparam M
     * @param a
     * @param b
     * @return ExpectationContext<N + M> N and M is size of each of A and B
     */
    template <size_t N, size_t M>
    ExpectationContext<N + M> closedOp(const ExpectationContext<N> a, const ExpectationContext<M> b) const
    {
        return a + b;
    }

    /**
     * @brief Convert result to comparable type.
     *
     * @tparam N
     * @param context
     * @return std::array<std::string, 2>
     */
    template <size_t N>
    std::string toComparable(const ExpectationContext<N> context) const
    {
        return context.template countedEvents<EventCountable>();
    }

private:
    static constexpr auto COMBINATIONS = Expandable::ExpectationContextTripleValueCombination<
        ExpectationContext, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{};
};
