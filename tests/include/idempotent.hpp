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
            [&]()
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
