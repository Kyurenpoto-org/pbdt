/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <functional>

#include "util.hpp"

template <typename IdempotentProperty>
struct IdempotentValidator
{
    void operator()(const auto complete) const
    {
        constexpr auto compileTimeCompleted = complete();
        static_assert(std::invoke(IdempotentProperty{}, compileTimeCompleted));

        const auto runTimeCompleted = complete();
        dynamic_assert(std::invoke(IdempotentProperty{}, runTimeCompleted));
    }
};

template <typename ToComparable, typename Component>
struct IdempotentProperty
{
    constexpr bool operator()(const auto& completed) const
    {
        return std::invoke(ToComparable{}, completed)
            == std::invoke(ToComparable{}, std::invoke(Component{}, completed).complete());
    }
};
