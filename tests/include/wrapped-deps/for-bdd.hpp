/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <source_location>
#include <tuple>

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
    template <typename... Targets, typename Target>
    constexpr auto operator+(GivenContext<Targets...>&& context, Target&& target)
    {
        return context.andGiven( //"",
            std::forward<Target>(target)
        );
    }

    template <typename... Domains, typename Domain>
    constexpr auto operator+(WhenContext<Domains...>&& context, Domain&& domain)
    {
        return context.andWhen( //"",
            std::forward<Domain>(domain)
        );
    }

    template <typename... Props, typename Prop>
    constexpr auto operator+(ThenContext<Props...>&& context, Prop&& prop)
    {
        return context.andThen( //"",
            std::forward<Prop>(prop)
        );
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

struct ToContainer
{
    template <typename T>
    constexpr auto operator()(T&& t) const
    {
        return exstd::toContainer(std::forward<T>(t));
    }
};

struct Given
{
    template <typename Target>
    constexpr auto operator()(Target&& target) const
    {
        return pbdt::bdd::given( //"",
            std::forward<Target>(target)
        );
    }
};

struct When
{
    template <typename Domain>
    constexpr auto operator()(Domain&& domain) const
    {
        return pbdt::bdd::when( //"",
            std::forward<Domain>(domain)
        );
    }
};

struct Then
{
    template <typename Prop>
    constexpr auto operator()(Prop&& prop) const
    {
        return pbdt::bdd::then( //"",
            std::forward<Prop>(prop)
        );
    }
};

struct RunnableScenario
{
    template <typename Target, typename Prop, typename Domain>
    constexpr auto operator()(Target&& target, Prop&& prop, Domain&& domain) const
    {
        return pbdt::bdd::scenario(
            std::forward<Target>(target), std::forward<Prop>(prop), std::forward<Domain>(domain)
        );
    }
};

struct Expect
{
    constexpr auto
    operator()(const bool expression, const std::source_location& location = std::source_location::current())
    {
        return pbdt::test_context::expect(expression, location);
    }
};

struct ApplyOrInvoke
{
    template <typename Func, typename Arg>
    constexpr auto operator()(Func&& func, Arg&& arg) const
    {
        return exstd::applyOrInvoke(std::forward<Func>(func), std::forward<Arg>(arg));
    }
};
