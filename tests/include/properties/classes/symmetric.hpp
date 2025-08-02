/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include "util.hpp"

template <typename SymmetricRequirements>
struct SymmetricRuntimeValueValidation :
    RunTimeValueValidationBase<SymmetricRuntimeValueValidation<SymmetricRequirements>>
{
    static constexpr size_t size()
    {
        return SymmetricRequirements::size();
    }

    template <size_t Idx>
    auto a() const
    {
        return []()
        {
            return requirements.template x<Idx>();
        };
    }

    template <size_t Idx>
    auto b() const
    {
        return []()
        {
            return requirements.inverse(requirements.morph(requirements.template x<Idx>()));
        };
    }

private:
    static constexpr SymmetricRequirements requirements{};
};

#include "generators/values/event-countable-combination.hpp"

template <typename EventCountable, typename EventCountLogInfo>
struct SymmetricEventCountLogInfoForwardRequirements
{
    static constexpr size_t size()
    {
        return VALUES.size();
    }

    template <size_t Idx>
    auto x() const
    {
        return eventCountLogInfo.toComparable(VALUES.template a<Idx>());
    }

    std::string morph(const auto value) const
    {
        return eventCountLogInfo.serialize(value);
    }

    auto inverse(const std::string value) const
    {
        return eventCountLogInfo.deserialize(value);
    }

private:
    static constexpr auto VALUES = Countable::EventCountableDoubleValueCombination<
        EventCountable, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{};
    static constexpr EventCountLogInfo eventCountLogInfo{};
};

template <typename EventCountable, typename EventCountLogInfo>
struct SymmetricEventCountLogInfoBackwardRequirements
{
    static constexpr size_t size()
    {
        return VALUES.size();
    }

    template <size_t Idx>
    auto x() const
    {
        return eventCountLogInfo.serialize(eventCountLogInfo.toComparable(VALUES.template a<Idx>()));
    }

    auto morph(const std::string value) const
    {
        return eventCountLogInfo.deserialize(value);
    }

    std::string inverse(const auto value) const
    {
        return eventCountLogInfo.serialize(value);
    }

private:
    static constexpr auto VALUES = Countable::EventCountableDoubleValueCombination<
        EventCountable, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{};
    static constexpr EventCountLogInfo eventCountLogInfo{};
};
