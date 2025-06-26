/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <type_traits>

#include "util.hpp"

template <typename MutualExclusiveRequirements>
struct MutualExclusiveTypeValidation : TypeValidationBase<MutualExclusiveTypeValidation<MutualExclusiveRequirements>>
{
    static constexpr size_t size()
    {
        return MutualExclusiveRequirements::size();
    }

    template <size_t Idx>
    using A = std::negation<std::conjunction<
        typename MutualExclusiveRequirements::template Constituent1<
            typename MutualExclusiveRequirements::template A<Idx>>,
        typename MutualExclusiveRequirements::template Constituent2<
            typename MutualExclusiveRequirements::template A<Idx>>>>;

    template <size_t Idx>
    using B = std::negation<std::conjunction<
        typename MutualExclusiveRequirements::template Constituent1<
            typename MutualExclusiveRequirements::template B<Idx>>,
        typename MutualExclusiveRequirements::template Constituent2<
            typename MutualExclusiveRequirements::template B<Idx>>>>;
};

#include "generators/types/callable-type.hpp"
#include "generators/types/container-type.hpp"

template <
    typename Result, template <typename> typename CallableTargetWrap, template <typename> typename RangeDomainWrap>
struct MutualExclusiveRangeDomainWithCallableTargetRequirements
{
    using TargetCombinations = typename CallableType::CallableTargetCombination<
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::type;

    using DomainCombinations = typename ContainerType::RangeDomainCombination<
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
    using B = std::tuple_element_t<Idx, DomainCombinations>;

    template <typename T>
    using Constituent1 = CallableTargetWrap<T>;

    template <typename T>
    using Constituent2 = RangeDomainWrap<T>;
};
