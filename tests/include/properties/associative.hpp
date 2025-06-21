/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "util.hpp"

template <typename ToComparable, typename AssociativeRequirements>
struct AssociativeValidation : ValueValidationBase<AssociativeValidation<ToComparable, AssociativeRequirements>>
{
    static constexpr size_t size()
    {
        return AssociativeRequirements::size();
    }

    template <size_t Idx>
    constexpr auto a() const
    {
        return []()
        {
            return toComparable(requirements.closedOp(
                requirements.closedOp(requirements.template a<Idx>(), requirements.template b<Idx>()),
                requirements.template c<Idx>()
            ));
        };
    }

    template <size_t Idx>
    constexpr auto b() const
    {
        return []()
        {
            return toComparable(requirements.closedOp(
                requirements.template a<Idx>(),
                requirements.closedOp(requirements.template b<Idx>(), requirements.template c<Idx>())
            ));
        };
    }

private:
    static constexpr AssociativeRequirements requirements{};
    static constexpr ToComparable toComparable{};
};

#include "generators/composable-callable.hpp"
#include "generators/foldable-callable.hpp"
#include "generators/productable-container.hpp"

template <typename Given>
struct AssociativeGivenRequirements
{
    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(COMBINATIONS)> - 3 + 1;
    }

    template <size_t Idx>
    constexpr auto a() const
    {
        return std::get<Idx>(COMBINATIONS);
    }

    template <size_t Idx>
    constexpr auto b() const
    {
        return std::get<Idx + 1>(COMBINATIONS);
    }

    template <size_t Idx>
    constexpr auto c() const
    {
        return std::get<Idx + 2>(COMBINATIONS);
    }

    template <typename A, typename B>
    constexpr auto closedOp(A&& a, B&& b) const
    {
        return (given(std::forward<A>(a)) + std::forward<B>(b)).complete();
    }

private:
    static constexpr auto COMBINATIONS = Composable::ComposableCombination<
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value;
    static constexpr Given given{};
};

template <typename When>
struct AssociativeWhenRequirements
{
    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(COMBINATIONS)> - 3 + 1;
    }

    template <size_t Idx>
    constexpr auto a() const
    {
        return std::get<Idx>(COMBINATIONS)();
    }

    template <size_t Idx>
    constexpr auto b() const
    {
        return std::get<Idx + 1>(COMBINATIONS)();
    }

    template <size_t Idx>
    constexpr auto c() const
    {
        return std::get<Idx + 2>(COMBINATIONS)();
    }

    template <typename A, typename B>
    constexpr auto closedOp(A&& a, B&& b) const
    {
        return (when(std::forward<A>(a)) + std::forward<B>(b)).complete();
    }

private:
    static constexpr auto COMBINATIONS = Productable::ProductableCombination<
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM()>::value;
    static constexpr When when{};
};

template <typename Expect, typename Then>
struct AssociativeThenRequirements
{
    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(COMBINATIONS)> - 3 + 1;
    }

    template <size_t Idx>
    constexpr auto a() const
    {
        return std::get<Idx>(COMBINATIONS);
    }

    template <size_t Idx>
    constexpr auto b() const
    {
        return std::get<Idx + 1>(COMBINATIONS);
    }

    template <size_t Idx>
    constexpr auto c() const
    {
        return std::get<Idx + 2>(COMBINATIONS);
    }

    template <typename A, typename B>
    constexpr auto closedOp(A&& a, B&& b) const
    {
        return (then(std::forward<A>(a)) + std::forward<B>(b)).complete();
    }

private:
    static constexpr auto COMBINATIONS = Foldable::FoldableCombination<
        Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value;
    static constexpr Then then{};
};
