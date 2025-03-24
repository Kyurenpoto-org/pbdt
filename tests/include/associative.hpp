/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "util.hpp"

template <typename ToComparable>
struct AssociativeValidator
{
    static constexpr bool associative(const auto& l2r, const auto& r2l)
    {
        return toComparable(l2r) == toComparable(r2l);
    }

    void operator()(const auto l2r, const auto r2l) const
    {
        constexpr auto compileTimeL2R = l2r();
        constexpr auto compileTimeR2L = r2l();
        static_assert(associative(compileTimeL2R, compileTimeR2L));

        const auto runTimeL2R = l2r();
        const auto runTimeR2L = r2l();
        dynamic_assert(associative(runTimeL2R, runTimeR2L));
    }

private:
    static constexpr ToComparable toComparable{};
};

template <typename TwoWayCompletableRawContext>
struct AcceptableRawContext
{
    template <typename Visitor>
    void accept(Visitor&& visitor) const
    {
        acceptImpl<TwoWayCompletableRawContext::size() - 1>(std::forward<Visitor>(visitor));
    }

private:
    template <size_t Idx, typename Visitor>
    void acceptImpl(Visitor&& visitor) const
    {
        visitor(
            []()
            {
                return completable.template l2rComplete<Idx>();
            },
            []()
            {
                return completable.template r2lComplete<Idx>();
            }
        );

        if constexpr (Idx > 0)
        {
            acceptImpl<Idx - 1>(std::forward<Visitor>(visitor));
        }
    }

    static constexpr TwoWayCompletableRawContext completable{};
};

#include "composable-callable.hpp"
#include "foldable-callable.hpp"
#include "productable-container.hpp"

template <typename Given>
struct TwoWayCompletableRawGivenContext
{
    static constexpr auto RAW_CONTEXT = std::tuple{
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
        Composable::ComposableCombination<
            COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
            COMPILE_TIME_RANDOM()>::value,
    };
    static constexpr Given given{};

    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(RAW_CONTEXT)>;
    }

    template <size_t Idx>
    constexpr auto l2rComplete() const
    {
        constexpr auto combination = std::get<Idx>(RAW_CONTEXT);

        constexpr auto a = std::get<0>(combination);
        constexpr auto b = std::get<1>(combination);
        constexpr auto c = std::get<2>(combination);

        return (given((given(a) + b).complete()) + c).complete();
    }

    template <size_t Idx>
    constexpr auto r2lComplete() const
    {
        constexpr auto combination = std::get<Idx>(RAW_CONTEXT);

        constexpr auto a = std::get<0>(combination);
        constexpr auto b = std::get<1>(combination);
        constexpr auto c = std::get<2>(combination);

        return (given(a) + (given(b) + c).complete()).complete();
    }
};

template <typename When>
struct TwoWayCompletableRawWhenContext
{
    static constexpr auto RAW_CONTEXT = std::tuple{
        Productable::ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Productable::ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Productable::ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Productable::ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Productable::ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Productable::ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Productable::ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Productable::ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Productable::ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Productable::ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Productable::ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Productable::ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Productable::ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Productable::ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Productable::ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Productable::ProductableCombination<COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
    };
    static constexpr When when{};

    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(RAW_CONTEXT)>;
    }

    template <size_t Idx>
    constexpr auto l2rComplete() const
    {
        constexpr auto combination = std::get<Idx>(RAW_CONTEXT);

        constexpr auto a = std::get<0>(combination);
        constexpr auto b = std::get<1>(combination);
        constexpr auto c = std::get<2>(combination);

        return (when((when(a()) + b()).complete()) + c()).complete();
    }

    template <size_t Idx>
    constexpr auto r2lComplete() const
    {
        constexpr auto combination = std::get<Idx>(RAW_CONTEXT);

        constexpr auto a = std::get<0>(combination);
        constexpr auto b = std::get<1>(combination);
        constexpr auto c = std::get<2>(combination);

        return (when(a()) + (when(b()) + c()).complete()).complete();
    }
};

template <typename Then, typename Expect>
struct TwoWayCompletableRawThenContext
{
    static constexpr auto RAW_CONTEXT = std::tuple{
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
        Foldable::FoldableCombination<
            Expect, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value,
    };
    static constexpr Then then{};

    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(RAW_CONTEXT)>;
    }

    template <size_t Idx>
    constexpr auto l2rComplete() const
    {
        constexpr auto combination = std::get<Idx>(RAW_CONTEXT);

        constexpr auto a = std::get<0>(combination);
        constexpr auto b = std::get<1>(combination);
        constexpr auto c = std::get<2>(combination);

        return (then((then(a) + b).complete()) + c).complete();
    }

    template <size_t Idx>
    constexpr auto r2lComplete() const
    {
        constexpr auto combination = std::get<Idx>(RAW_CONTEXT);

        constexpr auto a = std::get<0>(combination);
        constexpr auto b = std::get<1>(combination);
        constexpr auto c = std::get<2>(combination);

        return (then(a) + (then(b) + c).complete()).complete();
    }
};
