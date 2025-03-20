/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <array>
#include <functional>
#include <ranges>
#include <tuple>

#include "fixtures.hpp"

namespace natural_components
{
    namespace target
    {
        constexpr int freeFuncTarget(int x)
        {
            return x + 0;
        }

        constexpr auto funcObjTarget = [](int x) -> int
        {
            return freeFuncTarget(x);
        };
    }

    namespace prop
    {
        constexpr auto freeFuncProp(int x, int result)
        {
            return pbdt::test_context::expect(x == result);
        }

        constexpr auto funcObjProp = [](int x, int result)
        {
            return freeFuncProp(x, result);
        };
    }

    namespace domain
    {
        constexpr std::array<int, 5> containerDomain{
            1, 2, 3, 4, 5,
        };

        // generator domain

        constexpr auto viewDomain = containerDomain
                                  | std::views::transform(
                                        [](int x)
                                        {
                                            return x;
                                        }
                                  );
    }

    namespace target2
    {
        constexpr int freeFuncTarget(int x, int y)
        {
            return (x + y) - (y + x);
        }
    }

    namespace prop2
    {
        constexpr auto freeFuncProp(std::tuple<int, int> args, int result)
        {
            return pbdt::test_context::expect(result == 0);
        }
    }

    namespace domain2
    {
        constexpr std::array<int, 5> containerDomain{
            1, 2, 3, 4, 5,
        };

        constexpr auto viewDomain = containerDomain
                                  | std::views::transform(
                                        [](int x)
                                        {
                                            return std::tuple{ x };
                                        }
                                  );
    }
}

namespace Examples
{
    constexpr auto combinations = std::tuple{
        std::tuple{
            &natural_components::target::freeFuncTarget,
            &natural_components::prop::freeFuncProp,
            natural_components::domain::containerDomain,
        },
        std::tuple{
            &natural_components::target::freeFuncTarget,
            &natural_components::prop::freeFuncProp,
            natural_components::domain::viewDomain,
        },
        std::tuple{
            &natural_components::target::freeFuncTarget,
            natural_components::prop::funcObjProp,
            natural_components::domain::containerDomain,
        },
        std::tuple{
            &natural_components::target::freeFuncTarget,
            natural_components::prop::funcObjProp,
            natural_components::domain::viewDomain,
        },
        std::tuple{
            natural_components::target::funcObjTarget,
            &natural_components::prop::freeFuncProp,
            natural_components::domain::containerDomain,
        },
        std::tuple{
            natural_components::target::funcObjTarget,
            &natural_components::prop::freeFuncProp,
            natural_components::domain::viewDomain,
        },
        std::tuple{
            natural_components::target::funcObjTarget,
            natural_components::prop::funcObjProp,
            natural_components::domain::containerDomain,
        },
        std::tuple{
            natural_components::target::funcObjTarget,
            natural_components::prop::funcObjProp,
            natural_components::domain::viewDomain,
        },
        std::tuple{
            &natural_components::target2::freeFuncTarget,
            &natural_components::prop2::freeFuncProp,
#if __cpp_lib_ranges_cartesian_product >= 202207L
            std::views::cartesian_product(
                natural_components::domain2::containerDomain, natural_components::domain2::containerDomain
            ),
#else
            []()
            {
                std::array<std::tuple<int, int>, 25> result{};
                for (size_t i = 0; i < 5; ++i)
                {
                    for (size_t j = 0; j < 5; ++j)
                    {
                        result[i * 5 + j] = { natural_components::domain2::containerDomain[i],
                                              natural_components::domain2::containerDomain[j] };
                    }
                }
                return result;
            }(),
#endif
        },
    };

    template <typename PropertyContext, size_t... Idxs>
    constexpr void assertCombinations(const std::index_sequence<Idxs...>)
    {
        (
            []()
            {
                constexpr auto compileTimeTarget = std::get<0>(std::get<Idxs>(combinations));
                constexpr auto compileTimeProp = std::get<1>(std::get<Idxs>(combinations));
                constexpr auto compileTimeDomain = std::get<2>(std::get<Idxs>(combinations));
                static_assert(
                    std::invoke(PropertyContext{}, compileTimeTarget, compileTimeProp, compileTimeDomain).passable()
                );

                const auto runTimeTarget = std::get<0>(std::get<Idxs>(combinations));
                const auto runTimeProp = std::get<1>(std::get<Idxs>(combinations));
                const auto runTimeDomain = std::get<2>(std::get<Idxs>(combinations));
                dynamic_assert(std::invoke(PropertyContext{}, runTimeTarget, runTimeProp, runTimeDomain).passable());
            }(),
            ...
        );
    }
}

template <typename PropertyContext>
void examples()
{
    Examples::assertCombinations<PropertyContext>(
        std::make_index_sequence<std::tuple_size_v<decltype(Examples::combinations)>>()
    );
}
