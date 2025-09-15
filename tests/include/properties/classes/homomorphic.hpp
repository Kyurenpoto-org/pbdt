/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <tuple>

#include "properties/util.hpp"

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
     * @tparam IDX
     * @return constexpr auto The callable that same as () -> decltype(morph(beforeMorphOp(a, b))).
     */
    template <size_t IDX>
    constexpr auto a() const
    {
        return []()
        {
            return requirements.template morph<IDX>(
                requirements.beforeMorphOp(requirements.template a<IDX>(), requirements.template b<IDX>())
            );
        };
    }

    /**
     * @brief Construct the operand-after-morph callable for the given index.
     *
     * @tparam IDX
     * @return constexpr auto The callable that same as () -> decltype(afterMorphOp(morph(a), morph(b))).
     */
    template <size_t IDX>
    constexpr auto b() const
    {
        return []()
        {
            return requirements.afterMorphOp(
                requirements.template morph<IDX>(requirements.template a<IDX>()),
                requirements.template morph<IDX>(requirements.template b<IDX>())
            );
        };
    }

private:
    static constexpr HomomorphicRequirements requirements{};
};

/**
 * @brief A value validation structure that checks homomorphic requirements in run-time.
 *
 * @details Homomorphic property: morph(a op b) == morph(a) op' morph(b).
 *
 * @tparam HomomorphicRequirements
 *
 * @see RunTimeValueValidationBase
 */
template <typename HomomorphicRequirements>
struct HomomorphicRunTimeValueValidation :
    RunTimeValueValidationBase<HomomorphicRunTimeValueValidation<HomomorphicRequirements>>
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
     * @tparam IDX
     * @return auto The callable that same as () -> decltype(morph(beforeMorphOp(a, b))).
     */
    template <size_t IDX>
    auto a() const
    {
        return []()
        {
            return requirements.template morph<IDX>(
                requirements.beforeMorphOp(requirements.template a<IDX>(), requirements.template b<IDX>())
            );
        };
    }

    /**
     * @brief Construct the operand-after-morph callable for the given index.
     *
     * @tparam IDX
     * @return auto The callable that same as () -> decltype(afterMorphOp(morph(a), morph(b))).
     */
    template <size_t IDX>
    auto b() const
    {
        return []()
        {
            return requirements.afterMorphOp(
                requirements.template morph<IDX>(requirements.template a<IDX>()),
                requirements.template morph<IDX>(requirements.template b<IDX>())
            );
        };
    }

private:
    static constexpr HomomorphicRequirements requirements{};
};

#include "generators/values/event-countable-combination.hpp"
#include "generators/values/expectation-context-combination.hpp"
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
     * @tparam IDX
     * @return constexpr auto The callable that same as () -> decltype(propA()).
     */
    template <size_t IDX>
    constexpr auto a() const
    {
        return std::get<IDX>(COMBINATIONS).template propA<Expect>();
    }

    /**
     * @brief Get the property object B for the given index.
     *
     * @tparam IDX
     * @return constexpr auto The callable that same as () -> decltype(propA()).
     */
    template <size_t IDX>
    constexpr auto b() const
    {
        return std::get<IDX>(COMBINATIONS).template propB<Expect>();
    }

    /**
     * @brief Construct the RunnableScenario object from property object and check passable for given index.
     *
     * @tparam IDX
     * @tparam Prop
     * @return constexpr bool
     */
    template <size_t IDX, typename Prop>
    constexpr bool morph(Prop&& prop) const
    {
        constexpr auto combination = std::get<IDX>(COMBINATIONS);

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
 * @brief A structure that defines homomorphic requirements for EventCountable::sum() and EventCountable::someFailed()
 * with operator+(EventCountable, EventCountable).
 *
 * @tparam EventCountable
 */
template <typename EventCountable>
struct HomomorphicEventCountableRequirements
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
     * @return constexpr EventCountable
     */
    template <size_t IDX>
    constexpr EventCountable a() const
    {
        return COMBINATIONS.template a<IDX>();
    }

    /**
     * @brief Get the EventCountable object B for given index.
     *
     * @tparam IDX
     * @return constexpr EventCountable
     */
    template <size_t IDX>
    constexpr EventCountable b() const
    {
        return COMBINATIONS.template b<IDX>();
    }

    /**
     * @brief Compute sum of all event counts & Check if some events failed.
     *
     * @tparam size_t Ignored parameter
     * @param events
     * @return constexpr std::tuple<size_t, bool>
     */
    template <size_t>
    constexpr std::tuple<size_t, bool> morph(const EventCountable events) const
    {
        return {
            events.sum(),
            events.someFailed(),
        };
    }

    /**
     * @brief Compute accumulate-before-sum-and-some-failed expression.
     *
     * @param a
     * @param b
     * @return constexpr EventCountable
     */
    constexpr EventCountable beforeMorphOp(const EventCountable a, const EventCountable b) const
    {
        return a + b;
    }

    /**
     * @brief Compute accumulate-after-sum-and-some-failed expression.
     *
     * @param a
     * @param b
     * @return constepxr std::tuple<size_t, bool>
     */
    constexpr std::tuple<size_t, bool>
    afterMorphOp(const std::tuple<size_t, bool> a, const std::tuple<size_t, bool> b) const
    {
        return {
            std::get<0>(a) + std::get<0>(b),
            std::get<1>(a) || std::get<1>(b),
        };
    }

private:
    static constexpr auto COMBINATIONS = Countable::EventCountableDoubleValueCombination<
        EventCountable, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{};
};

/**
 * @brief A structure that defines homomorphic requirements for ExpectationContext<N>::countedEvents() and
 * ExpectationContext<N>::failureReport() with operator+(ExpectationContext<N>, ExpectationContext<M>).
 *
 * @tparam ExpectationContext
 * @tparam EventCountable
 */
template <template <size_t> typename ExpectationContext, typename EventCountable>
struct HomomorphicExpectationContextRequirements
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
     * @brief Stringify each of counted events and failure report.
     *
     * @tparam size_t Ignored parameter
     * @tparam N
     * @param context
     * @return std::array<std::string, 2>
     */
    template <size_t, size_t N>
    std::array<std::string, 2> morph(const ExpectationContext<N> context) const
    {
        return {
            static_cast<std::string>(context.template countedEvents<EventCountable>()),
            static_cast<std::string>(context.template failureReport<EventCountable>()),
        };
    }

    /**
     * @brief Compute accumulate-before-count-and-report expression.
     *
     * @param a
     * @param b
     * @return ExpectationContext<N + M> N and M is size of each of A and B
     */
    template <size_t N, size_t M>
    ExpectationContext<N + M> beforeMorphOp(const ExpectationContext<N> a, const ExpectationContext<M> b) const
    {
        return a + b;
    }

    /**
     * @brief Compute accumulate-after-count-and-report expression.
     *
     * @param a
     * @param b
     * @return std::array<std::string, 2>
     */
    std::array<std::string, 2>
    afterMorphOp(const std::array<std::string, 2> a, const std::array<std::string, 2> b) const
    {
        return {
            a[0] + b[0],
            a[1] + b[1],
        };
    }

private:
    static constexpr auto COMBINATIONS = Expandable::ExpectationContextTripleValueCombination<
        ExpectationContext, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{};
};
