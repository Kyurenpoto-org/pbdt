/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <tuple>

#include "fixtures.hpp"

namespace Examples
{
    constexpr auto combinations = std::tuple{
        std::tuple{
            natural_components::target::freeFuncTarget,
            natural_components::prop::freeFuncProp,
            natural_components::domain::containerDomain,
        },
        std::tuple{
            natural_components::target::freeFuncTarget,
            natural_components::prop::freeFuncProp,
            natural_components::domain::viewDomain,
        },
        std::tuple{
            natural_components::target::freeFuncTarget,
            natural_components::prop::funcObjProp,
            natural_components::domain::containerDomain,
        },
        std::tuple{
            natural_components::target::freeFuncTarget,
            natural_components::prop::funcObjProp,
            natural_components::domain::viewDomain,
        },
        std::tuple{
            natural_components::target::funcObjTarget,
            natural_components::prop::freeFuncProp,
            natural_components::domain::containerDomain,
        },
        std::tuple{
            natural_components::target::funcObjTarget,
            natural_components::prop::freeFuncProp,
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
            natural_components::target2::freeFuncTarget,
            natural_components::prop2::freeFuncProp,
#if __cpp_lib_ranges_cartesian_product >= 202207L
            std::views::cartesian_product(
                natural_components::domain2::containerDomain, natural_components::domain2::containerDomain
            ),
#else
            [&]()
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
