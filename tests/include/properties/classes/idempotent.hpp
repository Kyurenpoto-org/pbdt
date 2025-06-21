/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "util.hpp"

template <typename ToComparable, typename IdempotentRequirements>
struct IdempotentValueValidation : ValueValidationBase<IdempotentValueValidation<ToComparable, IdempotentRequirements>>
{
    static constexpr size_t size()
    {
        return IdempotentRequirements::size();
    }

    template <size_t Idx>
    constexpr auto a() const
    {
        return []()
        {
            return toComparable(complete(std::make_index_sequence<Idx + 1>()));
        };
    }

    template <size_t Idx>
    constexpr auto b() const
    {
        return []()
        {
            return toComparable(requirements.closedOp(complete(std::make_index_sequence<Idx + 1>())));
        };
    }

private:
    template <size_t... Idxs>
    static constexpr auto complete(const std::index_sequence<Idxs...>)
    {
        return requirements.closedOp(requirements.template element<Idxs>()...);
    }

    static constexpr IdempotentRequirements requirements{};
    static constexpr ToComparable toComparable{};
};

#include "generators/values/composable-callable.hpp"
#include "generators/values/foldable-callable.hpp"
#include "generators/values/productable-container.hpp"

template <typename Given>
struct IdempotentGivenRequirements
{
    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(RAW_CONTEXT)>;
    }

    template <size_t Idx>
    constexpr auto element() const
    {
        return std::get<Idx>(RAW_CONTEXT);
    }

    template <typename T, typename... Ts>
    constexpr auto closedOp(T&& t, Ts&&... ts) const
    {
        return (given(std::forward<T>(t)) + ... + std::forward<Ts>(ts)).complete();
    }

private:
    static constexpr auto RAW_CONTEXT = Composable::ComposableCombination<
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value;
    static constexpr Given given{};
};

template <typename When>
struct IdempotentWhenRequirements
{
    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(RAW_CONTEXT)>;
    }

    template <size_t Idx>
    constexpr auto element() const
    {
        return std::get<Idx>(RAW_CONTEXT)();
    }

    template <typename T, typename... Ts>
    constexpr auto closedOp(T&& t, Ts&&... ts) const
    {
        return (when(std::forward<T>(t)) + ... + std::forward<Ts>(ts)).complete();
    }

private:
    static constexpr auto RAW_CONTEXT = Productable::ProductableCombination<
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value;
    static constexpr When when{};
};

template <typename Then, typename Expect>
struct IdempotentThenRequirements
{
    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(RAW_CONTEXT)>;
    }

    template <size_t Idx>
    constexpr auto element() const
    {
        return std::get<Idx>(RAW_CONTEXT);
    }

    template <typename T, typename... Ts>
    constexpr auto closedOp(T&& t, Ts&&... ts) const
    {
        return (then(std::forward<T>(t)) + ... + std::forward<Ts>(ts)).complete();
    }

private:
    static constexpr auto RAW_CONTEXT = Foldable::FoldableCombination<
        Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value;
    static constexpr Then then{};
};
