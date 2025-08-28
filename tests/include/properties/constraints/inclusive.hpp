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
 *
 * @see CompileTimePropositionValidationBase
 * @see TypeProposition
 */
template <typename InclusiveRequirements>
struct InclusiveTypeValidation : CompileTimePropositionValidationBase<InclusiveTypeValidation<InclusiveRequirements>>
{
    static constexpr size_t size()
    {
        return InclusiveRequirements::size();
    }

    template <typename T>
    struct Inclusive :
        TypeProposition<std::disjunction<
            std::negation<typename InclusiveRequirements::template Includer<T>>,
            typename InclusiveRequirements::template BeIncluded<T>>>
    {
    };

    template <size_t Idx>
    constexpr auto truth() const
    {
        return Inclusive<typename InclusiveRequirements::template Origin<Idx>>{};
    }

    template <size_t Idx>
    constexpr auto falsity() const
    {
        return Inclusive<typename InclusiveRequirements::template Complement<Idx>>{};
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
