/*
 * SPDX - FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <algorithm>
#include <array>
#include <ranges>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>

#ifndef PBDT_MODULE

#include "exstd/callable-traits.hpp"
#include "exstd/functional.hpp"
#include "exstd/ranges.hpp"

#endif

#include "pbdt/test-context.hpp"

namespace pbdt::bdd
{
    namespace detail
    {
        template <typename... Elements>
        struct ComponentContext;

        template <typename... Elements>
            requires(sizeof...(Elements) > 0)
        struct ComponentContext<Elements...>
        {
            constexpr ComponentContext(
                const std::array<std::string_view, sizeof...(Elements)> steps, const std::tuple<Elements...> elements
            ) :
                steps(steps),
                elements(elements)
            {
            }

            template <typename Element>
            constexpr ComponentContext<Elements..., std::decay_t<Element>>
            expand(const std::string_view step, Element&& element) const
            {
                std::array<std::string_view, sizeof...(Elements) + 1> newSteps;
                std::copy(steps.begin(), steps.end(), newSteps.begin());
                newSteps[sizeof...(Elements)] = step;

                return {
                    newSteps,
                    std::tuple_cat(elements, std::tuple{ element }),
                };
            }

            template <typename T>
                requires std::is_constructible_v<T, std::tuple<Elements...>>
            constexpr T complete() const
            {
                return T{ elements };
            }

            constexpr std::string description(const std::string_view name) const
            {
                std::string result = "[" + std::string(name) + "] " + steps[0] + "\n";
                for (size_t i = 1; i < steps.size(); ++i)
                {
                    result += "[AND] " + steps[i] + "\n";
                }

                return result;
            }

        private:
            std::array<std::string_view, sizeof...(Elements)> steps;
            std::tuple<Elements...> elements;
        };

        template <typename Element>
        struct ComponentContext<Element>
        {
            template <typename T>
                requires std::is_constructible_v<Element, T>
            constexpr ComponentContext(const std::string_view step, T&& element) :
                step(step),
                element(std::forward<T>(element))
            {
            }

            template <typename NextElement>
            constexpr auto expand(const std::string_view nextStep, NextElement&& nextElement) const
            {
                return ComponentContext<Element, std::decay_t<NextElement>>{
                    std::array{ step, nextStep },
                    std::tuple{ element, nextElement },
                };
            }

            constexpr Element complete() const
            {
                return element;
            }

            constexpr std::string description(const std::string_view name) const
            {
                return "[" + std::string(name) + "] " + std::string(step) + "\n";
            }

        private:
            std::string_view step;
            Element element;
        };

        template <typename Target>
        concept CallableTarget = exstd::Callable<std::decay_t<Target>>;

        template <typename Ret, typename ArgsTuple>
        concept IsomorphicCallableComponent =
            (std::is_same_v<Ret, ArgsTuple> || std::is_same_v<std::tuple<Ret>, ArgsTuple>);

        template <typename... Targets>
        struct GivenContext;

        template <typename, typename, typename>
        struct TargetCompletion;

        template <typename... Targets, typename Ret, typename... Args>
        struct TargetCompletion<std::tuple<Targets...>, Ret, std::tuple<Args...>> : std::tuple<Targets...>
        {
            constexpr Ret operator()(Args... args) const
            {
                return exstd::compositeApply(
                    std::tuple<Targets...>{ *this }, std::tuple{ std::forward<std::remove_reference_t<Args>>(args)... }
                );
            }
        };

        template <typename... Targets>
            requires(sizeof...(Targets) > 0)
        struct GivenContext<Targets...>
        {
            using ArgumentsType = exstd::CallableArgumentsType<std::tuple_element_t<0, std::tuple<Targets...>>>;
            using ReturnType =
                exstd::CallableReturnType<std::tuple_element_t<sizeof...(Targets) - 1, std::tuple<Targets...>>>;

            constexpr GivenContext(ComponentContext<Targets...> core) :
                core(core)
            {
            }

            template <typename Target>
                requires CallableTarget<Target>
                      && IsomorphicCallableComponent<ReturnType, exstd::CallableArgumentsType<std::decay_t<Target>>>
            constexpr GivenContext<Targets..., std::decay_t<Target>>
            andGiven(const std::string_view step, Target&& target) const
            {
                return { core.expand(step, std::forward<Target>(target)) };
            }

            using Completion = TargetCompletion<std::tuple<Targets...>, ReturnType, ArgumentsType>;

            constexpr Completion complete() const
            {
                return core.template complete<Completion>();
            }

            constexpr std::string description() const
            {
                return core.description("GIVEN");
            }

        private:
            ComponentContext<Targets...> core;
        };

        template <typename Target>
        struct GivenContext<Target>
        {
            using ArgumentsType = exstd::CallableArgumentsType<Target>;
            using ReturnType = exstd::CallableReturnType<Target>;

            template <typename T>
                requires std::is_constructible_v<Target, T>
            constexpr GivenContext(const std::string_view step, T&& target) :
                core(step, std::forward<T>(target))
            {
            }

            template <typename NextTarget>
                requires CallableTarget<NextTarget>
                      && IsomorphicCallableComponent<ReturnType, exstd::CallableArgumentsType<std::decay_t<NextTarget>>>
            constexpr GivenContext<Target, std::decay_t<NextTarget>>
            andGiven(const std::string_view step, NextTarget&& target) const
            {
                return { core.expand(step, std::forward<NextTarget>(target)) };
            }

            constexpr Target complete() const
            {
                return core.complete();
            }

            constexpr std::string description() const
            {
                return core.description("GIVEN");
            }

        private:
            ComponentContext<Target> core;
        };

        template <typename Domain>
        concept RangeDomain = std::ranges::range<std::decay_t<Domain>>;

        template <typename... Domains>
        struct WhenContext;

        template <typename>
        struct DomainCompletion;

        template <typename... Domains>
        struct DomainCompletion<std::tuple<Domains...>> :
            public std::ranges::view_interface<DomainCompletion<std::tuple<Domains...>>>
        {
            using BaseType = decltype(exstd::flattenCartesianProduct(std::declval<Domains>()...));

            constexpr DomainCompletion(const std::tuple<Domains...> domains) :
                base(std::apply(
                    [](Domains... domains)
                    {
                        return exstd::flattenCartesianProduct(std::move(domains)...);
                    },
                    domains
                ))
            {
            }

            constexpr auto begin() const
            {
                return base.begin();
            }

            constexpr auto end() const
            {
                return base.end();
            }

        private:
            BaseType base;
        };

        template <typename... Domains>
            requires(sizeof...(Domains) > 0)
        struct WhenContext<Domains...>
        {
            using RangeType = decltype(exstd::flattenCartesianProduct(std::declval<Domains>()...));

            constexpr WhenContext(ComponentContext<Domains...> core) :
                core(core)
            {
            }

            template <typename Domain>
                requires RangeDomain<Domain>
            constexpr WhenContext<Domains..., decltype(exstd::toContainer(std::declval<Domain>()))>
            andWhen(const std::string_view step, Domain&& domain) const
            {
                return { core.expand(step, exstd::toContainer(std::forward<Domain>(domain))) };
            }

            using Completion = DomainCompletion<std::tuple<Domains...>>;

            constexpr Completion complete() const
            {
                return core.template complete<Completion>();
            }

            constexpr std::string description() const
            {
                return core.description("WHEN");
            }

        private:
            ComponentContext<Domains...> core;
        };

        template <typename Domain>
        struct WhenContext<Domain>
        {
            using RangeType = Domain;

            template <typename T>
                requires std::is_constructible_v<Domain, T>
            constexpr WhenContext(const std::string_view step, T&& domain) :
                core(step, std::forward<T>(domain))
            {
            }

            template <typename NextDomain>
                requires RangeDomain<NextDomain>
            constexpr WhenContext<Domain, decltype(exstd::toContainer(std::declval<NextDomain>()))>
            andWhen(const std::string_view step, NextDomain&& domain) const
            {
                return { core.expand(step, exstd::toContainer(std::forward<NextDomain>(domain))) };
            }

            constexpr Domain complete() const
            {
                return core.complete();
            }

            constexpr std::string description() const
            {
                return core.description("WHEN");
            }

        private:
            ComponentContext<Domain> core;
        };

        template <typename Prop>
        concept CallableProperty =
            exstd::Callable<std::decay_t<Prop>>
            && std::tuple_size_v<exstd::CallableArgumentsType<std::decay_t<Prop>>> == 2
            && std::is_same_v<exstd::CallableReturnType<std::decay_t<Prop>>, test_context::detail::TestContext>;

        template <typename... Funcs>
        using CommonArgumentsType = std::common_type_t<exstd::CallableArgumentsType<Funcs>...>;

        template <typename... Funcs>
        concept HasCommonArguments = requires { typename CommonArgumentsType<Funcs...>; };

        template <typename... Props>
        struct ThenContext;

        template <typename, typename, typename>
        struct PropertyCompletion;

        template <typename... Props, typename Sample, typename Result>
        struct PropertyCompletion<std::tuple<Props...>, Sample, Result> : std::tuple<Props...>
        {
            constexpr test_context::detail::TestContext operator()(Sample sample, Result result) const
            {
                return std::apply(
                    [&sample, &result](const Props&... props)
                    {
                        return (props(sample, result) + ...);
                    },
                    std::tuple<Props...>{ *this }
                );
            }
        };

        template <typename... Props>
            requires(sizeof...(Props) > 0)
        struct ThenContext<Props...>
        {
            using SampleType = std::tuple_element_t<0, CommonArgumentsType<Props...>>;
            using ResultType = std::tuple_element_t<1, CommonArgumentsType<Props...>>;

            constexpr ThenContext(ComponentContext<Props...> core) :
                core(core)
            {
            }

            template <typename Prop>
                requires CallableProperty<Prop> && HasCommonArguments<Props..., std::decay_t<Prop>>
            constexpr ThenContext<Props..., std::decay_t<Prop>>
            andThen(const std::string_view step, Prop&& property) const
            {
                return { core.expand(step, static_cast<std::decay_t<Prop>>(property)) };
            }

            using Completion = PropertyCompletion<std::tuple<Props...>, SampleType, ResultType>;

            constexpr Completion complete() const
            {
                return core.template complete<Completion>();
            }

            constexpr std::string description() const
            {
                return core.description("THEN");
            }

        private:
            ComponentContext<Props...> core;
        };

        template <typename Prop>
        struct ThenContext<Prop>
        {
            using SampleType = std::tuple_element_t<0, exstd::CallableArgumentsType<Prop>>;
            using ResultType = std::tuple_element_t<1, exstd::CallableArgumentsType<Prop>>;

            template <typename T>
                requires std::is_constructible_v<Prop, T>
            constexpr ThenContext(const std::string_view step, T&& property) :
                core(step, std::forward<T>(property))
            {
            }

            template <typename NextProp>
                requires CallableProperty<NextProp> && HasCommonArguments<Prop, std::decay_t<NextProp>>
            constexpr ThenContext<Prop, std::decay_t<NextProp>>
            andThen(const std::string_view step, NextProp&& property) const
            {
                return { core.expand(step, static_cast<std::decay_t<NextProp>>(property)) };
            }

            constexpr Prop complete() const
            {
                return core.complete();
            }

            constexpr std::string description() const
            {
                return core.description("THEN");
            }

        private:
            ComponentContext<Prop> core;
        };
    }

    template <typename Target>
        requires detail::CallableTarget<Target>
    constexpr detail::GivenContext<std::decay_t<Target>> given(const std::string_view step, Target&& target)
    {
        return {
            step,
            std::forward<Target>(target),
        };
    }

    template <typename Domain>
        requires detail::RangeDomain<Domain>
    constexpr detail::WhenContext<decltype(exstd::toContainer(std::declval<Domain>()))>
    when(const std::string_view step, Domain&& domain)
    {
        return {
            step,
            exstd::toContainer(std::forward<Domain>(domain)),
        };
    }

    template <typename Prop>
        requires detail::CallableProperty<Prop>
    constexpr detail::ThenContext<std::decay_t<Prop>> then(const std::string_view step, Prop&& property)
    {
        return {
            step,
            std::forward<Prop>(property),
        };
    }

    namespace detail
    {
        template <typename Given, typename When, typename Then>
        struct Scenario
        {
            constexpr auto test() const
            {
                // return test_context::propertyContext(given.complete(), then.complete(), when.complete());
            }
        };

        template <typename Given>
        concept ValidGivenComponent = requires(Given given) {
            typename Given::ArgumentsType;
            typename Given::ReturnType;
            { given.complete() };
        };

        template <typename When>
        concept ValidWhenComponent = requires(When when) {
            typename When::RangeType;
            { when.complete() };
        };

        template <typename Then>
        concept ValidThenComponent = requires(Then then) {
            typename Then::SampleType;
            typename Then::ResultType;
            { then.complete() };
        };

        template <typename Given, typename When, typename Then>
        struct ScenarioBuilder
        {
            template <typename NewGiven, typename NewWhen, typename NewThen>
                requires std::is_constructible_v<Given, NewGiven> && std::is_constructible_v<When, NewWhen>
                          && std::is_constructible_v<Then, NewThen>
            constexpr ScenarioBuilder(NewGiven&& given, NewWhen&& when, NewThen&& then) :
                given(std::forward<NewGiven>(given)),
                when(std::forward<NewWhen>(when)),
                then(std::forward<NewThen>(then))
            {
            }

            template <typename NewGiven>
                requires std::is_same_v<Given, nullptr_t> && ValidGivenComponent<NewGiven>
            constexpr ScenarioBuilder<NewGiven, When, Then> withGiven(NewGiven&& newGiven) const
            {
                return {
                    std::forward<NewGiven>(newGiven),
                    when,
                    then,
                };
            }

            template <typename NewWhen>
                requires std::is_same_v<When, nullptr_t> && ValidWhenComponent<NewWhen>
            constexpr ScenarioBuilder<Given, NewWhen, Then> withWhen(NewWhen&& newWhen) const
            {
                return {
                    given,
                    std::forward<NewWhen>(newWhen),
                    then,
                };
            }

            template <typename NewThen>
                requires std::is_same_v<Then, nullptr_t> && ValidThenComponent<NewThen>
            constexpr ScenarioBuilder<Given, When, NewThen> withThen(NewThen&& newThen) const
            {
                return {
                    given,
                    when,
                    std::forward<NewThen>(newThen),
                };
            }

            constexpr auto build(const std::string_view name) const
                requires(
                    !(std::is_same_v<Given, nullptr_t> || std::is_same_v<When, nullptr_t>
                      || std::is_same_v<Then, nullptr_t>)
                )
            {
            }

        private:
            Given given;
            When when;
            Then then;
        };
    }

    constexpr auto scenario(const std::string_view name)
    {
    }

    constexpr detail::ScenarioBuilder<std::nullptr_t, std::nullptr_t, std::nullptr_t> builder{
        nullptr,
        nullptr,
        nullptr,
    };
}

namespace exstd::detail
{
    template <typename... Domains>
    struct CompileTimeViewExtent<pbdt::bdd::detail::DomainCompletion<std::tuple<Domains...>>> :
        CompileTimeViewExtent<typename pbdt::bdd::detail::DomainCompletion<std::tuple<Domains...>>::BaseType>
    {
    };
}
