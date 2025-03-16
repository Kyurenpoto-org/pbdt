/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>
#include <span>
#include <string_view>
#include <tuple>

#include "prop-pp/impl/bdd.hpp"

#include "fixtures.hpp"

#define ASSERT_IDEMPOTENT(arg)                                                                                         \
    {                                                                                                                  \
        constexpr auto compileTimeSamples = arg;                                                                       \
        static_assert(                                                                                                 \
            exstd::toContainer(compileTimeSamples)                                                                     \
            == exstd::toContainer(prop_pp::bdd::when("", compileTimeSamples).complete())                               \
        );                                                                                                             \
                                                                                                                       \
        const auto runTimeSamples = arg;                                                                               \
        dynamic_assert(                                                                                                \
            exstd::toContainer(runTimeSamples)                                                                         \
            == exstd::toContainer(prop_pp::bdd::when("", runTimeSamples).complete())                                   \
        );                                                                                                             \
    }

void idempotent()
{
    ASSERT_IDEMPOTENT(prop_pp::bdd::when("", natural_components::domain::containerDomain).complete());
    ASSERT_IDEMPOTENT(prop_pp::bdd::when("", natural_components::domain::viewDomain).complete());
    ASSERT_IDEMPOTENT(prop_pp::bdd::when("", natural_components::domain2::containerDomain)
                          .andWhen("", natural_components::domain2::containerDomain)
                          .complete());
    ASSERT_IDEMPOTENT(prop_pp::bdd::when("", natural_components::domain2::viewDomain)
                          .andWhen("", natural_components::domain2::containerDomain)
                          .complete());
    ASSERT_IDEMPOTENT(prop_pp::bdd::when("", natural_components::domain2::containerDomain)
                          .andWhen("", natural_components::domain2::viewDomain)
                          .complete());
    ASSERT_IDEMPOTENT(prop_pp::bdd::when("", natural_components::domain2::viewDomain)
                          .andWhen("", natural_components::domain2::viewDomain)
                          .complete());
}

namespace Associative
{
    template <typename, typename>
    struct SequenceArray;

    template <size_t... Ns>
    struct SequenceArray<std::index_sequence<Ns...>, int>
    {
        static constexpr auto value = std::array<int, sizeof...(Ns)>{ Ns... };

        static constexpr auto rvalue()
        {
            return value;
        }
    };

    constexpr std::string_view charTable = "0123456789";

    template <size_t... Ns>
    struct SequenceArray<std::index_sequence<Ns...>, std::string_view>
    {
        static constexpr auto value = std::array{ (charTable.substr(Ns, 1))... };

        static constexpr auto rvalue()
        {
            return value;
        }
    };

    template <typename Seq, typename T>
    struct SequenceArraySpan
    {
        static constexpr auto value = std::span{ SequenceArray<Seq, T>::value };

        static constexpr auto rvalue()
        {
            return value;
        }
    };

    template <typename Seq, typename T>
    struct SequenceArrayOwningView
    {
        static constexpr auto rvalue()
        {
            return std::ranges::owning_view{ SequenceArray<Seq, T>::rvalue() };
        }
    };

    template <typename Seq, typename T>
    struct SequenceArrayRefView
    {
        static constexpr auto value = std::ranges::ref_view{ SequenceArray<Seq, T>::value };

        static constexpr auto rvalue()
        {
            return value;
        }
    };

    template <size_t, size_t, typename>
    struct ProductableContainer;

    template <size_t N, typename T>
    struct ProductableContainer<0, N, T>
    {
        static constexpr auto value()
        {
            return SequenceArray<std::make_index_sequence<N>(), T>::rvalue();
        }
    };

    template <size_t N, typename T>
    struct ProductableContainer<1, N, T>
    {
        static constexpr auto value()
        {
            return SequenceArraySpan<std::make_index_sequence<N>(), T>::rvalue();
        }
    };

    template <size_t N, typename T>
    struct ProductableContainer<2, N, T>
    {
        static constexpr auto value()
        {
            return SequenceArrayOwningView<std::make_index_sequence<N>(), T>::rvalue();
        }
    };

    template <size_t N, typename T>
    struct ProductableContainer<3, N, T>
    {
        static constexpr auto value()
        {
            return SequenceArrayRefView<std::make_index_sequence<N>(), T>::rvalue();
        }
    };

    constexpr auto productableCombinations = std::tuple{
        std::tuple{
            natural_components::domain::containerDomain,
            natural_components::domain::containerDomain,
            natural_components::domain::containerDomain,
        },
        std::tuple{
            natural_components::domain::containerDomain,
            natural_components::domain::containerDomain,
            natural_components::domain::viewDomain,
        },
        std::tuple{
            natural_components::domain::containerDomain,
            natural_components::domain::viewDomain,
            natural_components::domain::containerDomain,
        },
        std::tuple{
            natural_components::domain::containerDomain,
            natural_components::domain::viewDomain,
            natural_components::domain::viewDomain,
        },
        std::tuple{
            natural_components::domain::viewDomain,
            natural_components::domain::containerDomain,
            natural_components::domain::containerDomain,
        },
        std::tuple{
            natural_components::domain::viewDomain,
            natural_components::domain::containerDomain,
            natural_components::domain::viewDomain,
        },
        std::tuple{
            natural_components::domain::viewDomain,
            natural_components::domain::viewDomain,
            natural_components::domain::containerDomain,
        },
        std::tuple{
            natural_components::domain::viewDomain,
            natural_components::domain::viewDomain,
            natural_components::domain::viewDomain,
        },
    };

    template <size_t... Idxs>
    constexpr void assertCombinations(const std::index_sequence<Idxs...>)
    {
        (
            []()
            {
                constexpr auto combination = std::get<Idxs>(productableCombinations);

                constexpr auto a = std::get<0>(combination);
                constexpr auto b = std::get<1>(combination);
                constexpr auto c = std::get<2>(combination);

                constexpr auto compileTimeLhs = prop_pp::bdd::when("", a)
                                                    .andWhen("", prop_pp::bdd::when("", b).andWhen("", c).complete())
                                                    .complete();
                constexpr auto compileTimeRhs =
                    prop_pp::bdd::when("", prop_pp::bdd::when("", a).andWhen("", b).complete())
                        .andWhen("", c)
                        .complete();
                static_assert(exstd::toContainer(compileTimeLhs) == exstd::toContainer(compileTimeRhs));

                const auto runTimeLhs = prop_pp::bdd::when("", a)
                                            .andWhen("", prop_pp::bdd::when("", b).andWhen("", c).complete())
                                            .complete();
                const auto runTimeRhs = prop_pp::bdd::when("", prop_pp::bdd::when("", a).andWhen("", b).complete())
                                            .andWhen("", c)
                                            .complete();
                dynamic_assert(exstd::toContainer(runTimeLhs) == exstd::toContainer(runTimeRhs));
            }(),
            ...
        );
    }
}

void associative()
{
    Associative::assertCombinations(
        std::make_index_sequence<std::tuple_size_v<decltype(Associative::productableCombinations)>>()
    );
}

int main()
{
    // Set Category that has product between any two objects
    idempotent();
    associative();

    return EXIT_SUCCESS;
}
