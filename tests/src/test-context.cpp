/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <print>

#include "prop-pp/test-context.hpp"

#include "fixtures.hpp"

#define ASSERT_COMBINATION(target, prop, domain)                                                                       \
    {                                                                                                                  \
        constexpr auto compileTimeTarget = target;                                                                     \
        constexpr auto compileTimeProp = prop;                                                                         \
        constexpr auto compileTimeDomain = domain;                                                                     \
        static_assert(                                                                                                 \
            prop_pp::test_context::propertyContext(compileTimeTarget, compileTimeProp, compileTimeDomain).passable()   \
        );                                                                                                             \
                                                                                                                       \
        const auto runTimeTarget = target;                                                                             \
        const auto runTimeProp = prop;                                                                                 \
        const auto runTimeDomain = domain;                                                                             \
        dynamic_assert(prop_pp::test_context::propertyContext(runTimeTarget, runTimeProp, runTimeDomain).passable());  \
    }

void examples()
{
    ASSERT_COMBINATION(
        natural_components::target::freeFuncTarget, natural_components::prop::freeFuncProp,
        natural_components::domain::containerDomain
    );
    ASSERT_COMBINATION(
        natural_components::target::freeFuncTarget, natural_components::prop::freeFuncProp,
        natural_components::domain::viewDomain
    );
    ASSERT_COMBINATION(
        natural_components::target::freeFuncTarget, natural_components::prop::funcObjProp,
        natural_components::domain::containerDomain
    );
    ASSERT_COMBINATION(
        natural_components::target::freeFuncTarget, natural_components::prop::funcObjProp,
        natural_components::domain::viewDomain
    );
    ASSERT_COMBINATION(
        natural_components::target::funcObjTarget, natural_components::prop::freeFuncProp,
        natural_components::domain::containerDomain
    );
    ASSERT_COMBINATION(
        natural_components::target::funcObjTarget, natural_components::prop::freeFuncProp,
        natural_components::domain::viewDomain
    );
    ASSERT_COMBINATION(
        natural_components::target::funcObjTarget, natural_components::prop::funcObjProp,
        natural_components::domain::containerDomain
    );
    ASSERT_COMBINATION(
        natural_components::target::funcObjTarget, natural_components::prop::funcObjProp,
        natural_components::domain::viewDomain
    );

#if __cpp_lib_ranges_cartesian_product >= 202207L
    ASSERT_COMBINATION(
        natural_components::target2::freeFuncTarget, natural_components::prop2::freeFuncProp,
        std::views::cartesian_product(
            natural_components::domain2::containerDomain, natural_components::domain2::containerDomain
        )
    );
#else
    constexpr auto product = [&]()
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
    }();
    ASSERT_COMBINATION(natural_components::target2::freeFuncTarget, natural_components::prop2::freeFuncProp, product);
#endif
}

int main()
{
    examples();

    return EXIT_SUCCESS;
}
