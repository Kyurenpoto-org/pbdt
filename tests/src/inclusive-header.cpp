/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>

#include "properties/constraints/inclusive.hpp"

#include "exstd/callable-traits.hpp"
#include "pbdt/constraints.hpp"

#include "suite-runner.hpp"
#include "wrapped-deps/for-constraints.hpp"

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

int main(int argc, const char* const* argv)
{
    suite(
        std::array{
            "callable-target",
            "callable-property",
        },
        std::array{
            callableTarget,
            callableProperty,
        }
    )
        .run(argc, argv);

    return EXIT_SUCCESS;
}
