/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <type_traits>

#include "util.hpp"

/**
 * @brief A type validation structure that checks inclusive properties.
 *
 * @tparam InclusiveRequirements
 */
template <typename InclusiveRequirements>
struct InclusiveTypeValidation : CompileTimePropositionValidationBase<InclusiveTypeValidation<InclusiveRequirements>>
{
    static constexpr size_t size()
    {
        return InclusiveRequirements::size();
    }

    template <size_t Idx>
    using Truth = std::disjunction<
        std::negation<
            typename InclusiveRequirements::template Includer<typename InclusiveRequirements::template Origin<Idx>>>,
        typename InclusiveRequirements::template BeIncluded<typename InclusiveRequirements::template Origin<Idx>>>;

    template <size_t Idx>
    constexpr auto truth() const
    {
        return []()
        {
            return Truth<Idx>::value;
        };
    }

    template <size_t Idx>
    using Falsity = std::disjunction<
        typename InclusiveRequirements::template Includer<typename InclusiveRequirements::template Complement<Idx>>,
        std::negation<typename InclusiveRequirements::template BeIncluded<
            typename InclusiveRequirements::template Complement<Idx>>>>;

    template <size_t Idx>
    constexpr auto falsity() const
    {
        return []()
        {
            return Falsity<Idx>::value;
        };
    }
};

#include "generators/types/callable-type.hpp"

/**
 * @brief A structure that defines inclusive requirements for callable-target concept.
 *
 * @tparam CallableWrap
 * @tparam CallableTargetWrap
 */
template <template <typename> typename CallableWrap, template <typename> typename CallableTargetWrap>
struct InclusiveCallableTargetRequirements
{
    using TargetCombinations = typename CallableType::CallableTargetCombination<
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::type;

    using NonTargetCombinations = typename CallableType::NonCallableTargetCombination<
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::type;

    static constexpr size_t size()
    {
        return std::tuple_size_v<TargetCombinations>;
    }

    template <size_t Idx>
    using Origin = std::tuple_element_t<Idx, TargetCombinations>;

    template <size_t Idx>
    using Complement = std::tuple_element_t<Idx, NonTargetCombinations>;

    template <typename T>
    using Includer = CallableWrap<T>;

    template <typename T>
    using BeIncluded = CallableTargetWrap<T>;
};

/**
 * @brief A structure that defines inclusive requirements for two concepts: callable-target and callable-property.
 *
 * @tparam Result
 * @tparam CallableTargetWrap
 * @tparam CallablePropWrap
 */
template <
    typename Result, template <typename> typename CallableTargetWrap, template <typename> typename CallablePropWrap>
struct InclusiveCallablePropertyRequirements
{
    using PropCombinations = typename CallableType::CallablePropCombination<
        Result, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::type;

    using NonPropCombinations = typename CallableType::NonCallablePropCombination<
        Result, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::type;

    static constexpr size_t size()
    {
        return std::tuple_size_v<PropCombinations>;
    }

    template <size_t Idx>
    using Origin = std::tuple_element_t<Idx, PropCombinations>;

    template <size_t Idx>
    using Complement = std::tuple_element_t<Idx, NonPropCombinations>;

    template <typename T>
    using Includer = CallableTargetWrap<T>;

    template <typename T>
    using BeIncluded = CallablePropWrap<T>;
};
