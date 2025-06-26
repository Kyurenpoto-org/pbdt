/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>

#include "properties/constraints/mutual-exclusive.hpp"

import exstd;
import pbdt;

#include "suite-runner.hpp"
#include "wrapped-deps/for-constraints.hpp"

void rangeDomainWithCallableTarget()
{
    const MutualExclusiveTypeValidation<MutualExclusiveRangeDomainWithCallableTargetRequirements<
        pbdt::test_context::detail::TestContext, CallableTarget, RangeDomain>>
        acceptable;
    acceptable.run();
}

int main(int argc, const char* const* argv)
{
    suite(
        std::array{
            "range-domain-with-callable-target",
        },
        std::array{
            rangeDomainWithCallableTarget,
        }
    )
        .run(argc, argv);

    return EXIT_SUCCESS;
}
