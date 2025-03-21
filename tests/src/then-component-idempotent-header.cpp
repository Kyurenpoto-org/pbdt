/*
 * SPDX - FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "then-component-idempotent-common.hpp"

#include "pbdt/bdd.hpp"

namespace pbdt::bdd::detail
{
    template <typename... Props, typename Prop>
    constexpr auto operator+(ThenContext<Props...>&& context, Prop&& prop)
    {
        return context.andThen("", std::forward<Prop>(prop));
    }
}

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
    idempotent<Expect, ToFlatTuple, Then>();

    return EXIT_SUCCESS;
}
