/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <type_traits>

#include "util.hpp"

/**
 * @brief A string validation structure that checks inclusive properties.
 *
 * @tparam InclusiveRequirements
 *
 * @see RunTimePropositionValidationBase
 * @see TypeProposition
 */
template <typename InclusiveRequirements>
struct InclusiveStringValidation : RunTimePropositionValidationBase<InclusiveStringValidation<InclusiveRequirements>>
{
    static constexpr size_t size()
    {
        return InclusiveRequirements::size();
    }

    template <typename Comparable>
    struct InclusiveComparison
    {
        InclusiveComparison(Comparable comparable, std::string_view partial) :
            comparable{
                comparable,
            },
            partial{
                partial,
            }
        {
        }

        bool operator()() const
        {
            return comparable.contains(partial);
        }

    private:
        Comparable comparable;
        std::string_view partial;
    };

    struct Inclusive
    {
        Inclusive(std::string_view includer, std::string_view beIncluded) :
            includer{
                includer,
            },
            beIncluded{
                beIncluded,
            }
        {
        }

        InclusiveComparison<Inclusive> comparison(std::string_view partial) const
        {
            return InclusiveComparison<Inclusive>{
                *this,
                partial,
            };
        }

        bool contains(std::string_view partial) const
        {
            return !includer.contains(partial) || beIncluded.contains(partial);
        }

    private:
        std::string_view includer;
        std::string_view beIncluded;
    };

    template <size_t Idx>
    constexpr auto truth() const
    {
        return Inclusive{
            requirements.template includer<Idx>(),
            requirements.template beIncluded<Idx>(),
        }
            .comparison(requirements.template origin<Idx>());
    }

    template <size_t Idx>
    constexpr auto falsity() const
    {
        return Inclusive{
            requirements.template includer<Idx>(),
            requirements.template beIncluded<Idx>(),
        }
            .comparison(requirements.template complement<Idx>());
    }

private:
    static constexpr InclusiveRequirements requirements{};
};

#include "generators/values/event-countable-combination.hpp"

/**
 * @brief A structure that defines inclusive requirements for EventCountable.
 *
 * @tparam EventCountable
 */
template <typename EventCountable>
struct InclusiveEventCountableStringifyRequirements
{
    static constexpr size_t size()
    {
        return VALUES.size();
    }

    template <size_t Idx>
    std::string origin() const
    {
        return VALUES.template a<Idx>().template each<EventCountable::EachName::PASSED>();
    }

    template <size_t Idx>
    std::string complement() const
    {
        return VALUES.template a<Idx>().pass().template each<EventCountable::EachName::PASSED>();
    }

    template <size_t Idx>
    std::string includer() const
    {
        return VALUES.template a<Idx>();
    }

    template <size_t Idx>
    std::string beIncluded() const
    {
        return VALUES.template a<Idx>().template each<EventCountable::EachName::PASSED>();
    }

private:
    static constexpr auto VALUES = Countable::EventCountableDoubleValueCombination<
        EventCountable, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{};
};
