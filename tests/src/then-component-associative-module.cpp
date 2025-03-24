/*
 * SPDX - FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
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

    template <typename... Props, typename Sample, typename Result>
    constexpr std::tuple<Props...>
    toTuple(const pbdt::bdd::detail::PropertyCompletion<std::tuple<Props...>, Sample, Result> completion)
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
    template <typename... Props, typename Prop>
    constexpr auto operator+(ThenContext<Props...>&& context, Prop&& prop)
    {
        return context.andThen("", std::forward<Prop>(prop));
    }
}

struct Expect
{
    constexpr auto
    operator()(const bool expression, const std::source_location& location = std::source_location::current())
    {
        return pbdt::test_context::expect(expression, location);
    }
};

struct ToFlatTuple
{
    template <typename T>
    constexpr auto operator()(T&& t) const
    {
        return flatTuple(toTuple(t));
    }
};

struct Then
{
    template <typename Prop>
    constexpr auto operator()(Prop&& prop) const
    {
        return pbdt::bdd::then("", std::forward<Prop>(prop));
    }
};

int main()
{
    const AcceptableRawContext<TwoWayCompletableRawThenContext<Then, Expect>> acceptable;
    acceptable.accept(AssociativeValidator<ToFlatTuple>{});

    return EXIT_SUCCESS;
}
