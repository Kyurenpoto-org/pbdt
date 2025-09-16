/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include "properties/util.hpp"

/**
 * @brief A value validation structure that checks associative properties.
 *
 * @details Associative property: (a op b) op c == a op (b op c).
 *
 * @tparam ToComparable
 * @tparam AssociativeRequirements
 *
 * @see TwoWayValueValidationBase
 */
template <typename ToComparable, typename AssociativeRequirements>
struct AssociativeValueValidation :
    TwoWayValueValidationBase<AssociativeValueValidation<ToComparable, AssociativeRequirements>>
{
    /**
     * @brief The size of index range.
     *
     * @details Same as AssociativeRequirements::size().
     *
     * @return constexpr size_t
     */
    static constexpr size_t size()
    {
        return AssociativeRequirements::size();
    }

    /**
     * @brief Construct the left-to-right callable for the given index.
     *
     * @tparam IDX
     * @return constexpr auto The callable that same as () -> decltype(toComparable(closedOp(closedOp(a, b), c))).
     */
    template <size_t Idx>
    constexpr auto a() const
    {
        return []()
        {
            return toComparable(requirements.closedOp(
                requirements.closedOp(requirements.template a<Idx>(), requirements.template b<Idx>()),
                requirements.template c<Idx>()
            ));
        };
    }

    /**
     * @brief Construct the right-to-left callable for the given index.
     *
     * @tparam IDX
     * @return constexpr auto The callable that same as () -> decltype(toComparable(closedOp(a, closedOp(b, c)))).
     */
    template <size_t Idx>
    constexpr auto b() const
    {
        return []()
        {
            return toComparable(requirements.closedOp(
                requirements.template a<Idx>(),
                requirements.closedOp(requirements.template b<Idx>(), requirements.template c<Idx>())
            ));
        };
    }

private:
    static constexpr AssociativeRequirements requirements{};
    static constexpr ToComparable toComparable{};
};

/**
 * @brief A value validation structure that checks associative properties in run-time.
 *
 * @details Associative property: (a op b) op c == a op (b op c).
 *
 * @tparam AssociativeRequirements
 *
 * @see RunTimeValueValidationBase
 */
template <typename AssociativeRequirements>
struct AssociativeRunTimeValueValidation :
    RunTimeValueValidationBase<AssociativeRunTimeValueValidation<AssociativeRequirements>>
{
    /**
     * @brief The size of index range.
     *
     * @details Same as AssociativeRequirements::size().
     *
     * @return constexpr size_t
     */
    static constexpr size_t size()
    {
        return AssociativeRequirements::size();
    }

    /**
     * @brief Construct the left-to-right callable for the given index.
     *
     * @tparam IDX
     * @return constexpr auto The callable that same as () -> decltype(toComparable(closedOp(closedOp(a, b), c))).
     */
    template <size_t Idx>
    auto a() const
    {
        return []()
        {
            return requirements.toComparable(requirements.closedOp(
                requirements.closedOp(requirements.template a<Idx>(), requirements.template b<Idx>()),
                requirements.template c<Idx>()
            ));
        };
    }

    /**
     * @brief Construct the right-to-left callable for the given index.
     *
     * @tparam IDX
     * @return constexpr auto The callable that same as () -> decltype(toComparable(closedOp(a, closedOp(b, c)))).
     */
    template <size_t Idx>
    auto b() const
    {
        return []()
        {
            return requirements.toComparable(requirements.closedOp(
                requirements.template a<Idx>(),
                requirements.closedOp(requirements.template b<Idx>(), requirements.template c<Idx>())
            ));
        };
    }

private:
    static constexpr AssociativeRequirements requirements{};
};

#include "generators/values/composable-callable.hpp"
#include "generators/values/event-countable-combination.hpp"
#include "generators/values/expectation-context-combination.hpp"
#include "generators/values/foldable-callable.hpp"
#include "generators/values/productable-container.hpp"

/**
 * @brief A structure that defines associative requirements for a given class.
 *
 * @tparam Given
 */
template <typename Given>
struct AssociativeGivenRequirements
{
    /**
     * @brief The size of index range.
     *
     * @return constexpr size_t
     */
    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(COMBINATIONS)> - 3 + 1;
    }

    /**
     * @brief Get the callable object A for given index.
     *
     * @tparam IDX
     * @return constexpr auto The callable
     */
    template <size_t Idx>
    constexpr auto a() const
    {
        return std::get<Idx>(COMBINATIONS);
    }

    /**
     * @brief Get the callable object B for given index.
     *
     * @tparam IDX
     * @return constexpr auto The callable
     */
    template <size_t Idx>
    constexpr auto b() const
    {
        return std::get<Idx + 1>(COMBINATIONS);
    }

    /**
     * @brief Get the callable object C for given index.
     *
     * @tparam IDX
     * @return constexpr auto The callable
     */
    template <size_t Idx>
    constexpr auto c() const
    {
        return std::get<Idx + 2>(COMBINATIONS);
    }

    /**
     * @brief Compute accumulate expression.
     *
     * @tparam A
     * @tparam B
     * @param a
     * @param b
     * @return constexpr auto The callable that same as B::operator()(A::operator()(...))
     */
    template <typename A, typename B>
    constexpr auto closedOp(A&& a, B&& b) const
    {
        return (given(std::forward<A>(a)) + std::forward<B>(b)).complete();
    }

private:
    static constexpr auto COMBINATIONS = Composable::ComposableCombination<
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value;
    static constexpr Given given{};
};

/**
 * @brief A structure that defines associative requirements for a when class.
 *
 * @tparam When
 */
template <typename When>
struct AssociativeWhenRequirements
{
    /**
     * @brief The size of index range.
     *
     * @return constexpr size_t
     */
    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(COMBINATIONS)> - 3 + 1;
    }

    /**
     * @brief Get the range object A for given index.
     *
     * @tparam IDX
     * @return constexpr auto The range
     */
    template <size_t Idx>
    constexpr auto a() const
    {
        return std::get<Idx>(COMBINATIONS)();
    }

    /**
     * @brief Get the range object B for given index.
     *
     * @tparam IDX
     * @return constexpr auto The range
     */
    template <size_t Idx>
    constexpr auto b() const
    {
        return std::get<Idx + 1>(COMBINATIONS)();
    }

    /**
     * @brief Get the range object C for given index.
     *
     * @tparam IDX
     * @return constexpr auto The range
     */
    template <size_t Idx>
    constexpr auto c() const
    {
        return std::get<Idx + 2>(COMBINATIONS)();
    }

    /**
     * @brief Compute accumulate expression.
     *
     * @tparam A
     * @tparam B
     * @param a
     * @param b
     * @return constexpr auto The range that same as array<T, N>
     */
    template <typename A, typename B>
    constexpr auto closedOp(A&& a, B&& b) const
    {
        return (when(std::forward<A>(a)) + std::forward<B>(b)).complete();
    }

private:
    static constexpr auto COMBINATIONS = Productable::ProductableCombination<
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM()>::value;
    static constexpr When when{};
};

/**
 * @brief A structure that defines associative requirements for a then class.
 *
 * @tparam Expect
 * @tparam Then
 */
template <typename Expect, typename Then>
struct AssociativeThenRequirements
{
    /**
     * @brief The size of index range.
     *
     * @return constexpr size_t
     */
    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(COMBINATIONS)> - 3 + 1;
    }

    /**
     * @brief Get the callable object A for given index.
     *
     * @tparam IDX
     * @return constexpr auto The callable that same as () -> TestContext
     */
    template <size_t Idx>
    constexpr auto a() const
    {
        return std::get<Idx>(COMBINATIONS);
    }

    /**
     * @brief Get the callable object B for given index.
     *
     * @tparam IDX
     * @return constexpr auto The callable that same as () -> TestContext
     */
    template <size_t Idx>
    constexpr auto b() const
    {
        return std::get<Idx + 1>(COMBINATIONS);
    }

    /**
     * @brief Get the callable object C for given index.
     *
     * @tparam IDX
     * @return constexpr auto The callable that same as () -> TestContext
     */
    template <size_t Idx>
    constexpr auto c() const
    {
        return std::get<Idx + 2>(COMBINATIONS);
    }

    /**
     * @brief Compute accumulate expression.
     *
     * @tparam A
     * @tparam B
     * @param a
     * @param b
     * @return constexpr auto The callable that same as () -> TestContext
     */
    template <typename A, typename B>
    constexpr auto closedOp(A&& a, B&& b) const
    {
        return (then(std::forward<A>(a)) + std::forward<B>(b)).complete();
    }

private:
    static constexpr auto COMBINATIONS = Foldable::FoldableCombination<
        Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value;
    static constexpr Then then{};
};

/**
 * @brief A structure that defines associative requirements for EventCountable::operator std::string() with
 * operator+(EventCountable EventCountable).
 *
 * @tparam EventCountable
 */
template <typename EventCountable>
struct AssociativeEventCountableRequirements
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
     * @brief Get the EventCountable object C for given index.
     *
     * @tparam IDX
     * @return EventCountable
     */
    template <size_t IDX>
    auto c() const
    {
        return COMBINATIONS.template c<IDX>();
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

    std::string toComparable(const EventCountable context) const
    {
        return context;
    }

private:
    static constexpr auto COMBINATIONS = Countable::EventCountableTripleValueCombination<
        EventCountable, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{};
};

/**
 * @brief A structure that defines associative requirements for ExpectationContext<N>::countedEvents() and
 * ExpectationContext<N>::failureReport() with operator+(ExpectationContext<N>, ExpectationContext<M>).
 *
 * @tparam ExpectationContext
 * @tparam EventCountable
 */
template <template <size_t> typename ExpectationContext, typename EventCountable>
struct AssociativeExpectationContextRequirements
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
     * @brief Get the ExpectationContext object C for given index.
     *
     * @tparam IDX
     * @return ExpectationContext<N> N is arbitrary size
     */
    template <size_t IDX>
    auto c() const
    {
        return COMBINATIONS.template c<IDX>();
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

    template <size_t N>
    std::array<std::string, 2> toComparable(const ExpectationContext<N> context) const
    {
        return {
            static_cast<std::string>(context.template countedEvents<EventCountable>()),
            static_cast<std::string>(context.template failureReport<EventCountable>()),
        };
    }

private:
    static constexpr auto COMBINATIONS = Expandable::ExpectationContextTripleValueCombination<
        ExpectationContext, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{};
};
