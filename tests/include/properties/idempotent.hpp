/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "util.hpp"

template <typename ToComparable, typename CompletableRawContext>
struct IdempotentValidation : ValidationBase<IdempotentValidation<ToComparable, CompletableRawContext>>
{
    static constexpr size_t size()
    {
        return CompletableRawContext::size();
    }

    template <size_t Idx>
    constexpr auto a() const
    {
        return []()
        {
            return toComparable(completable.complete(std::make_index_sequence<Idx + 1>()));
        };
    }

    template <size_t Idx>
    constexpr auto b() const
    {
        return []()
        {
            return toComparable(completable.idempotentComplete(completable.complete(std::make_index_sequence<Idx + 1>())
            ));
        };
    }

private:
    static constexpr CompletableRawContext completable{};
    static constexpr ToComparable toComparable{};
};

#include "generators/composable-callable.hpp"
#include "generators/foldable-callable.hpp"
#include "generators/productable-container.hpp"

template <typename Given>
struct CompletableRawGivenContext
{
    static constexpr auto RAW_CONTEXT = Composable::ComposableCombination<
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value;
    static constexpr Given given{};

    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(RAW_CONTEXT)>;
    }

    template <size_t Idx, size_t... Idxs>
    constexpr auto complete(const std::index_sequence<Idx, Idxs...>) const
    {
        return (given(std::get<Idx>(RAW_CONTEXT)) + ... + std::get<Idxs>(RAW_CONTEXT)).complete();
    }

    template <typename Completed>
    constexpr auto idempotentComplete(Completed&& completed) const
    {
        return given(completed).complete();
    }
};

template <typename When>
struct CompletableRawWhenContext
{
    static constexpr auto RAW_CONTEXT = Productable::ProductableCombination<
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value;
    static constexpr When when{};

    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(RAW_CONTEXT)>;
    }

    template <size_t Idx, size_t... Idxs>
    constexpr auto complete(const std::index_sequence<Idx, Idxs...>) const
    {
        return (when(std::get<Idx>(RAW_CONTEXT)()) + ... + std::get<Idxs>(RAW_CONTEXT)()).complete();
    }

    template <typename Completed>
    constexpr auto idempotentComplete(Completed&& completed) const
    {
        return when(completed).complete();
    }
};

template <typename Then, typename Expect>
struct CompletableRawThenContext
{
    static constexpr auto RAW_CONTEXT = Foldable::FoldableCombination<
        Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value;
    static constexpr Then then{};

    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(RAW_CONTEXT)>;
    }

    template <size_t Idx, size_t... Idxs>
    constexpr auto complete(const std::index_sequence<Idx, Idxs...>) const
    {
        return (then(std::get<Idx>(RAW_CONTEXT)) + ... + std::get<Idxs>(RAW_CONTEXT)).complete();
    }

    template <typename Completed>
    constexpr auto idempotentComplete(Completed&& completed) const
    {
        return then(completed).complete();
    }
};
