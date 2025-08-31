/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>

#include "properties/classes/inclusive.hpp"
#include "properties/constraints/inclusive.hpp"

import exstd;
import pbdt;

#include "suite-runner.hpp"
#include "wrapped-deps/for-constraints.hpp"
#include "wrapped-deps/for-test-context.hpp"

void callableTarget()
{
    const InclusiveTypeValidation<InclusiveCallableTargetRequirements<Callable, CallableTarget>> acceptable;
    acceptable.run();
}

void callableProperty()
{
    const InclusiveTypeValidation<InclusiveCallablePropertyRequirements<
        pbdt::test_context::detail::TestContext, CallableTarget, CallableProperty>>
        acceptable;
    acceptable.run();
}

void eventCountable()
{
    const InclusiveStringValidation<InclusiveEventCountableStringifyRequirements<EventCountableWrap>> acceptable;
    acceptable.run();
}

int main(int argc, const char* const* argv)
{
    suite(
        std::array{
            "callable-target",
            "callable-property",
            "event-countable-stringify",
        },
        std::array{
            callableTarget,
            callableProperty,
            eventCountable,
        }
    )
        .run(argc, argv);

    return EXIT_SUCCESS;
}
