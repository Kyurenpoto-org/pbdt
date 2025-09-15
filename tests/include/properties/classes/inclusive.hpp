/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <string_view>
#include <type_traits>

#include "properties/util.hpp"

/**
 * @brief A string validation structure that checks inclusive properties.
 *
 * @details Inclusive property: If A implies B, then not A or B must be true.
 *
 * @tparam InclusiveRequirements
 *
 * @see RunTimePropositionValidationBase
 */
template <typename InclusiveRequirements>
struct InclusiveStringPiecesValidation :
    RunTimePropositionValidationBase<InclusiveStringPiecesValidation<InclusiveRequirements>>
{
    /**
     * @brief The size of index range.
     *
     * @details Same as InclusiveRequirements::size().
     *
     * @return constexpr size_t
     */
    static constexpr size_t size()
    {
        return InclusiveRequirements::size();
    }

    /**
     * @brief Construct the truth set proposition for the given index.
     *
     * @tparam Idx
     * @return auto The callable that same as () -> bool.
     */
    template <size_t Idx>
    auto truth() const
    {
        return []()
        {
            return !requirements.template antecedent<Idx>()(requirements.template origin<Idx>())
                || requirements.template consequent<Idx>()(requirements.template origin<Idx>());
        };
    }

    /**
     * @brief Construct the falsity set proposition for the given index.
     *
     * @tparam Idx
     * @return auto The callable that same as () -> bool.
     */
    template <size_t Idx>
    auto falsity() const
    {
        return []()
        {
            return !requirements.template antecedent<Idx>()(requirements.template complement<Idx>())
                || requirements.template consequent<Idx>()(requirements.template complement<Idx>());
        };
    }

private:
    static constexpr InclusiveRequirements requirements{};
};

#include "generators/values/event-countable-combination.hpp"

/**
 * @brief A structure that defines inclusive requirements for EventCountable::operator std::string().
 *
 * @tparam EventCountable
 *
 * @see EventCountable::operator std::string()
 */
template <typename EventCountable>
struct InclusiveEventCountableRequirements
{
    /**
     * @brief The size of index range.
     *
     * @return constexpr size_t
     */
    static constexpr size_t size()
    {
        return VALUES.size();
    }

    /**
     * @brief Construct the origin string pieces for the given index.
     *
     * @tparam Idx
     * @return std::array<std::string, 4>
     */
    template <size_t Idx>
    std::array<std::string, 3> origin() const
    {
        return split(VALUES.template a<Idx>());
    }

    /**
     * @brief Construct the complement string pieces for the given index.
     *
     * @tparam Idx
     * @return std::array<std::string, 4>
     */
    template <size_t Idx>
    std::array<std::string, 3> complement() const
    {
        return split(VALUES.template a<Idx>().pass().fail().skip());
    }

    /**
     * @brief Construct the antecedent callable for the given index.
     *
     * @tparam Idx
     * @return Callable that same as (const std::array<std::string, 3>&) -> bool
     */
    template <size_t Idx>
    auto antecedent() const
    {
        return [sup = split(VALUES.template a<Idx>())](const std::array<std::string, 3>& pieces)
        {
            return sup == pieces;
        };
    }

    /**
     * @brief Construct the consequent callable for the given index.
     *
     * @tparam Idx
     * @return Callable that same as (const std::array<std::string, 3>&) -> bool
     */
    template <size_t Idx>
    auto consequent() const
    {
        const auto base = VALUES.template a<Idx>();
        const auto a = DROPS.template a<Idx>();
        const auto b = DROPS.template b<Idx>();

        return [subA =
                    std::array{
                        split(base + a[0])[0],
                        split(base + a[1])[1],
                        split(base + a[2])[2],
                    },
                subB =
                    std::array{
                        split(base + b[0])[0],
                        split(base + b[1])[1],
                        split(base + b[2])[2],
                    }](const std::array<std::string, 3>& pieces)
        {
            return subA == pieces && subB == pieces;
        };
    }

private:
    std::array<std::string, 3> split(const std::string str) const
    {
        auto splitToVec = [](const std::string& str, const std::string_view pattern)
        {
            return std::views::split(str, pattern)
                 | std::views::transform(
                       [](const auto& x)
                       {
                           return std::ranges::to<std::string>(x);
                       }
                 )
                 | std::ranges::to<std::vector<std::string>>();
        };

        const auto x = splitToVec(str, "|");
        const auto y = splitToVec(x[0], ",");

        return {
            y[0],
            y[1],
            y[2],
        };
    }

    static constexpr auto VALUES = Countable::EventCountableDoubleValueCombination<
        EventCountable, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{};

    static constexpr auto DROPS = Countable::EventCountableSingleDropCombination<
        EventCountable, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{};
};
