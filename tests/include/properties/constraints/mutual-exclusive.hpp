/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <type_traits>

#include "util.hpp"

/**
 * @brief A type validation structure that checks mutual exclusive properties.
 *
 * @tparam MutualExclusiveRequirements
 */
template <typename MutualExclusiveRequirements>
struct MutualExclusiveTypeValidation : TypeValidationBase<MutualExclusiveTypeValidation<MutualExclusiveRequirements>>
{
    static constexpr size_t size()
    {
        return MutualExclusiveRequirements::size();
    }

    template <size_t Idx>
    using A = std::conjunction<
        typename MutualExclusiveRequirements::template ConstituentA<
            typename MutualExclusiveRequirements::template A<Idx>>,
        std::negation<typename MutualExclusiveRequirements::template ConstituentB<
            typename MutualExclusiveRequirements::template A<Idx>>>>;

    template <size_t Idx>
    using B = std::conjunction<
        std::negation<typename MutualExclusiveRequirements::template ConstituentA<
            typename MutualExclusiveRequirements::template B<Idx>>>,
        typename MutualExclusiveRequirements::template ConstituentB<
            typename MutualExclusiveRequirements::template B<Idx>>>;
};

#include "generators/types/callable-type.hpp"
#include "generators/types/container-type.hpp"

/**
 * @brief A structure that defines mutual exclusive requirements for two concepts: callable-target and range-domain.
 *
 * @tparam Result
 * @tparam CallableTargetWrap
 * @tparam RangeDomainWrap
 */
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
    using ConstituentA = CallableTargetWrap<T>;

    template <typename T>
    using ConstituentB = RangeDomainWrap<T>;
};
