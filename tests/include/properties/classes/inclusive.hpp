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
struct InclusiveStringPiecesValidation :
    RunTimePropositionValidationBase<InclusiveStringPiecesValidation<InclusiveRequirements>>
{
    static constexpr size_t size()
    {
        return InclusiveRequirements::size();
    }

    template <typename Comparable>
    struct InclusiveComparison
    {
        InclusiveComparison(const Comparable comparable, const std::array<std::string, 4> var) :
            comparable{
                comparable,
            },
            var{
                var,
            }
        {
        }

        bool operator()() const
        {
            return comparable.contains(var);
        }

    private:
        Comparable comparable;
        std::array<std::string, 4> var;
    };

    template <typename Antecedent, typename Consequent>
    struct Inclusive
    {
        Inclusive(const Antecedent antecedent, const Consequent consequent) :
            antecedent{ antecedent },
            consequent{ consequent }
        {
        }

        InclusiveComparison<Inclusive> comparison(const std::array<std::string, 4> var) const
        {
            return InclusiveComparison<Inclusive>{
                *this,
                var,
            };
        }

        bool contains(const std::array<std::string, 4>& var) const
        {
            return !antecedent(var) || consequent(var);
        }

    private:
        Antecedent antecedent;
        Consequent consequent;
    };

    template <size_t Idx>
    auto truth() const
    {
        return comparisonFactory<Idx>().comparison(requirements.template origin<Idx>());
    }

    template <size_t Idx>
    auto falsity() const
    {
        return comparisonFactory<Idx>().comparison(requirements.template complement<Idx>());
    }

private:
    template <size_t Idx>
    auto comparisonFactory() const
    {
        return Inclusive<typename InclusiveRequirements::Antecedent, typename InclusiveRequirements::Consequent>{
            requirements.template antecedent<Idx>(),
            requirements.template consequent<Idx>(),
        };
    }

    static constexpr InclusiveRequirements requirements{};
};

#include "generators/values/event-countable-combination.hpp"

/**
 * @brief A structure that defines inclusive requirements for EventCountable.
 *
 * @tparam EventCountable
 */
template <typename EventCountable>
struct InclusiveStringifiedEventCountableRequirements
{
    static constexpr size_t size()
    {
        return VALUES.size();
    }

    template <size_t Idx>
    std::array<std::string, 4> origin() const
    {
        return split(VALUES.template a<Idx>());
    }

    template <size_t Idx>
    std::array<std::string, 4> complement() const
    {
        return split(VALUES.template a<Idx>().pass().fail().skip());
    }

    struct Antecedent
    {
        Antecedent(const std::string sup) :
            sup{ sup }
        {
        }

        bool operator()(std::array<std::string, 4> pieces) const
        {
            return sup.contains(pieces[0]) && sup.contains(pieces[1]) && sup.contains(pieces[2])
                && sup.contains(pieces[3]);
        }

    private:
        std::string sup;
    };

    template <size_t Idx>
    auto antecedent() const
    {
        return Antecedent{
            VALUES.template a<Idx>(),
        };
    }

    struct Consequent
    {
        Consequent(const std::array<std::string, 3> subA, const std::array<std::string, 3> subB) :
            subA{
                subA,
            },
            subB{
                subB,
            }
        {
        }

        bool operator()(const std::array<std::string, 4> pieces) const
        {
            return implement(subA, pieces) && implement(subB, pieces);
        }

    private:
        bool implement(const std::array<std::string, 3>& sub, const std::array<std::string, 4>& pieces) const
        {
            return sub[0].contains(pieces[0]) && sub[1].contains(pieces[1]) && sub[2].contains(pieces[2])
                && !sub[0].contains(pieces[3]) && !sub[1].contains(pieces[3]) && !sub[2].contains(pieces[3]);
        }

        const std::array<std::string, 3> subA;
        const std::array<std::string, 3> subB;
    };

    template <size_t Idx>
    auto consequent() const
    {
        const auto base = VALUES.template a<Idx>();
        const auto a = DROPS.template a<Idx>();
        const auto b = DROPS.template b<Idx>();

        return Consequent{
            std::array{
                static_cast<std::string>(base + a[0]),
                static_cast<std::string>(base + a[1]),
                static_cast<std::string>(base + a[2]),
            },
            std::array{
                static_cast<std::string>(base + b[0]),
                static_cast<std::string>(base + b[1]),
                static_cast<std::string>(base + b[2]),
            },
        };
    }

private:
    std::array<std::string, 4> split(std::string str) const
    {
        const auto x = std::views::split(str, "|") | std::ranges::to<std::vector<std::string>>();
        const auto y = std::views::split(x[0], ",") | std::ranges::to<std::vector<std::string>>();

        return {
            y[0],
            y[1],
            y[2],
            x[1],
        };
    }

    static constexpr auto VALUES = Countable::EventCountableDoubleValueCombination<
        EventCountable, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{};

    static constexpr auto DROPS = Countable::EventCountableSingleDropCombination<
        EventCountable, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{};
};
