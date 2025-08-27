/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include "util.hpp"

template <typename ProjectiveRequirements>
struct LeftProjectiveValueValidation : TwoWayValueValidationBase<LeftProjectiveValueValidation<ProjectiveRequirements>>
{
    static constexpr size_t size()
    {
        return ProjectiveRequirements::size();
    }

    template <size_t Idx>
    constexpr auto a() const
    {
        return []()
        {
            return requirements.morph(
                requirements.template dependencies<Idx>(),
                requirements.closedOp(requirements.template a<Idx>(), requirements.template b<Idx>())
            );
        };
    }

    template <size_t Idx>
    constexpr auto b() const
    {
        return []()
        {
            return requirements.morph(requirements.template dependenciesForA<Idx>(), requirements.template a<Idx>());
        };
    }

private:
    static constexpr ProjectiveRequirements requirements{};
};

template <typename ProjectiveRequirements>
struct RightProjectiveValueValidation :
    TwoWayValueValidationBase<RightProjectiveValueValidation<ProjectiveRequirements>>
{
    static constexpr size_t size()
    {
        return ProjectiveRequirements::size();
    }

    template <size_t Idx>
    constexpr auto a() const
    {
        return []()
        {
            return requirements.morph(
                requirements.template dependencies<Idx>(),
                requirements.closedOp(requirements.template a<Idx>(), requirements.template b<Idx>())
            );
        };
    }

    template <size_t Idx>
    constexpr auto b() const
    {
        return []()
        {
            return requirements.morph(requirements.template dependenciesForB<Idx>(), requirements.template b<Idx>());
        };
    }

private:
    static constexpr ProjectiveRequirements requirements{};
};

template <typename ProjectiveRequirements>
struct DoubleProjectiveRuntimeValueValidation :
    RunTimeValueValidationBase<DoubleProjectiveRuntimeValueValidation<ProjectiveRequirements>>
{
    static constexpr size_t size()
    {
        return ProjectiveRequirements::size();
    }

    template <size_t Idx>
    auto a() const
    {
        return []()
        {
            return requirements.morph(requirements.template dependenciesA<Idx>(), requirements.template x<Idx>());
        };
    }

    template <size_t Idx>
    auto b() const
    {
        return []()
        {
            return requirements.morph(requirements.template dependenciesB<Idx>(), requirements.template x<Idx>());
        };
    }

private:
    static constexpr ProjectiveRequirements requirements{};
};

#include "generators/values/event-countable-combination.hpp"
#include "generators/values/runnable-double-combination.hpp"

template <typename ToContainer, typename Expect, typename RunnableScenario, typename Given>
struct ProjectiveRunnableScenarioWithGivenRequirements
{
    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(COMBINATIONS)>;
    }

    template <typename Prop, typename Domain>
    struct TargetDependencies
    {
        Prop prop;
        Domain domain;
    };

    template <size_t Idx>
    constexpr auto dependencies() const
    {
        constexpr auto combination = std::get<Idx>(COMBINATIONS);

        return TargetDependencies{
            .prop = combination.template prop<Expect>(),
            .domain = toContainer(combination.domain()),
        };
    }

    template <size_t Idx>
    constexpr auto dependenciesForA() const
    {
        constexpr auto combination = std::get<Idx>(COMBINATIONS);

        return TargetDependencies{
            .prop = combination.template propForTargetA<Expect>(),
            .domain = toContainer(combination.domain()),
        };
    }

    template <size_t Idx>
    constexpr auto a() const
    {
        return std::get<Idx>(COMBINATIONS).targetA();
    }

    template <size_t Idx>
    constexpr auto b() const
    {
        return std::get<Idx>(COMBINATIONS).targetB();
    }

    template <typename A, typename B>
    constexpr auto closedOp(A&& a, B&& b) const
    {
        return (given(std::forward<A>(a)) + std::forward<B>(b)).complete();
    }

    template <typename Dependencies, typename Target>
    constexpr auto morph(Dependencies&& dependencies, Target&& target) const
    {
        return runnableScenario(std::forward<Target>(target), dependencies.prop, dependencies.domain).run().passable();
    }

private:
    static constexpr auto COMBINATIONS = Runnable::RunnableDoubleTargetCombinationSamples<
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value;
    static constexpr ToContainer toContainer{};
    static constexpr RunnableScenario runnableScenario{};
    static constexpr Given given{};
};

template <typename ToContainer, typename Expect, typename RunnableScenario, typename When>
struct ProjectiveRunnableScenarioWithWhenRequirements
{
    static constexpr size_t size()
    {
        return std::tuple_size_v<decltype(COMBINATIONS)>;
    }

    template <typename Target, typename Prop>
    struct DomainDependencies
    {
        Target target;
        Prop prop;
    };

    template <size_t Idx>
    constexpr auto dependencies() const
    {
        constexpr auto combination = std::get<Idx>(COMBINATIONS);

        return DomainDependencies{
            .target = combination.target(),
            .prop = combination.template prop<Expect>(),
        };
    }

    template <size_t Idx>
    constexpr auto dependenciesForA() const
    {
        constexpr auto combination = std::get<Idx>(COMBINATIONS);

        return DomainDependencies{
            .target = combination.targetForDomainA(),
            .prop = combination.template propForDomainA<Expect>(),
        };
    }

    template <size_t Idx>
    constexpr auto dependenciesForB() const
    {
        constexpr auto combination = std::get<Idx>(COMBINATIONS);

        return DomainDependencies{
            .target = combination.targetForDomainB(),
            .prop = combination.template propForDomainB<Expect>(),
        };
    }

    template <size_t Idx>
    constexpr auto a() const
    {
        return std::get<Idx>(COMBINATIONS).domainA();
    }

    template <size_t Idx>
    constexpr auto b() const
    {
        return std::get<Idx>(COMBINATIONS).domainB();
    }

    template <typename A, typename B>
    constexpr auto closedOp(A&& a, B&& b) const
    {
        return (when(std::forward<A>(a)) + std::forward<B>(b)).complete();
    }

    template <typename Dependencies, typename Domain>
    constexpr auto morph(Dependencies&& dependencies, Domain&& domain) const
    {
        return runnableScenario(dependencies.target, dependencies.prop, std::forward<Domain>(domain)).run().passable();
    }

private:
    static constexpr auto COMBINATIONS = Runnable::RunnableDoubleDomainCombinationSamples<
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>::value;
    static constexpr ToContainer toContainer{};
    static constexpr RunnableScenario runnableScenario{};
    static constexpr When when{};
};

template <typename EventCountable>
struct ProjectiveEventCountableEachRequirements
{
    static constexpr size_t size()
    {
        return VALUES.size();
    }

    template <size_t Idx>
    constexpr auto dependenciesA() const
    {
        return DROPS.template a<Idx>();
    }

    template <size_t Idx>
    constexpr auto dependenciesB() const
    {
        return DROPS.template b<Idx>();
    }

    template <size_t Idx>
    constexpr auto x() const
    {
        return VALUES.template a<Idx>();
    }

    template <typename Dependencies, typename Value>
    auto morph(Dependencies&& dependencies, Value&& value) const
    {
        return std::array{
            static_cast<std::string>(EventCountable::template each<0>(dependencies[0] + value)),
            static_cast<std::string>(EventCountable::template each<1>(dependencies[1] + value)),
            static_cast<std::string>(EventCountable::template each<2>(dependencies[2] + value)),
        };
    }

private:
    static constexpr auto VALUES = Countable::EventCountableDoubleValueCombination<
        EventCountable, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{};

    static constexpr auto DROPS = Countable::EventCountableSingleDropCombination<
        EventCountable, COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(),
        COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM(), COMPILE_TIME_RANDOM()>{};
};
