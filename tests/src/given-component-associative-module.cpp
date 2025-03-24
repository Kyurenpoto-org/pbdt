/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "associative.hpp"

import pbdt;

namespace
{
    template <typename T>
    constexpr std::tuple<T> toTuple(const T t)
    {
        return { t };
    }

    template <typename... Targets, typename Ret, typename Args>
    constexpr std::tuple<Targets...>
    toTuple(const pbdt::bdd::detail::TargetCompletion<std::tuple<Targets...>, Ret, Args> completion)
    {
        return { completion };
    }

    constexpr auto flatTuple(auto&& tup)
    {
        return std::apply(
            [](const auto&... args)
            {
                return std::tuple_cat(toTuple(args)...);
            },
            tup
        );
    }
}

namespace pbdt::bdd::detail
{
    template <typename... Targets, typename Target>
    constexpr auto operator+(GivenContext<Targets...>&& context, Target&& target)
    {
        return context.andGiven("", std::forward<Target>(target));
    }
}

struct ToFlatTuple
{
    template <typename T>
    constexpr auto operator()(T&& t) const
    {
        return flatTuple(toTuple(t));
    }
};

struct Given
{
    template <typename Target>
    constexpr auto operator()(Target&& target) const
    {
        return pbdt::bdd::given("", std::forward<Target>(target));
    }
};

int main()
{
    const AcceptableRawContext<TwoWayCompletableRawGivenContext<Given>> acceptable;
    acceptable.accept(AssociativeValidator<ToFlatTuple>{});

    return EXIT_SUCCESS;
}
