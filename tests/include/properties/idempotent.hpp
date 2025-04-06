/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "util.hpp"

template <typename Component, typename ToComparable>
struct IdempotentValidator
{
    static constexpr bool idempotent(const auto& completed)
    {
        return toComparable(completed) == toComparable(component(completed).complete());
    }

    void operator()(const auto complete) const
    {
        constexpr auto compileTimeCompleted = complete();
        static_assert(idempotent(compileTimeCompleted));

        const auto runTimeCompleted = complete();
        dynamic_assert(idempotent(runTimeCompleted));
    }

private:
    static constexpr ToComparable toComparable{};
    static constexpr Component component{};
};

template <typename CompletableRawContext>
struct AcceptableRawContext
{
    template <typename Visitor>
    void accept(Visitor&& visitor) const
    {
        acceptImpl<CompletableRawContext::size() - 1>(std::forward<Visitor>(visitor));
    }

private:
    template <size_t Measure, typename Visitor>
    void acceptImpl(Visitor&& visitor) const
    {
        visitor(
            []()
            {
                return completable.complete(std::make_index_sequence<Measure + 1>());
            }
        );

        if constexpr (Measure > 0)
        {
            acceptImpl<Measure - 1>(std::forward<Visitor>(visitor));
        }
    }

    static constexpr CompletableRawContext completable{};
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
};
