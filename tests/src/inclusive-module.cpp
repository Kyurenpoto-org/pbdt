/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>

#include "properties/constraints/inclusive.hpp"

import exstd;
import pbdt;

#include "suite-runner.hpp"
#include "wrapped-deps/for-constraints.hpp"

void callableTarget()
{
    const InclusiveTypeValidation<InclusiveCallableTargetRequirements<Callable, CallableTarget>> acceptable;
}

int main(int argc, const char* const* argv)
{
    suite(
        std::array{
            "callable-target",
        },
        std::array{
            callableTarget,
        }
    )
        .run(argc, argv);

    return EXIT_SUCCESS;
}
