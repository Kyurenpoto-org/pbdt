/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <type_traits>

#include "util.hpp"

template <typename InclusiveRequirements>
struct InclusiveTypeValidation : TypeValidationBase<InclusiveTypeValidation<InclusiveRequirements>>
{
    static constexpr size_t size()
    {
        return InclusiveRequirements::size();
    }

    template <size_t Idx>
    using A = std::disjunction<
        std::negation<typename InclusiveRequirements::template Includer<InclusiveRequirements::template A<Idx>>>,
        typename InclusiveRequirements::template BeIncluded<InclusiveRequirements::template A<Idx>>>;

    template <size_t Idx>
    using B = std::disjunction<
        typename InclusiveRequirements::template Includer<InclusiveRequirements::template B<Idx>>,
        std::negation<typename InclusiveRequirements::template BeIncluded<InclusiveRequirements::template B<Idx>>>>;
};

#include "generators/types/callable-type.hpp"
#include "generators/types/container-type.hpp"

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
    using A = std::tuple_element_t<Idx, TargetCombinations>;

    template <size_t Idx>
    using B = std::tuple_element_t<Idx, NonTargetCombinations>;

    template <typename T>
    using Includer = CallableWrap<T>;

    template <typename T>
    using BeIncluded = CallableTargetWrap<T>;
};

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
    using A = std::tuple_element_t<Idx, PropCombinations>;

    template <size_t Idx>
    using B = std::tuple_element_t<Idx, NonPropCombinations>;

    template <typename T>
    using Includer = CallableTargetWrap<T>;

    template <typename T>
    using BeIncluded = CallablePropWrap<T>;
};
