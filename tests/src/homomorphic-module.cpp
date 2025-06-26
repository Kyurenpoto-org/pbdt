/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>

#include "properties/classes/homomorphic.hpp"

import exstd;
import pbdt;

#include "suite-runner.hpp"
#include "wrapped-deps/for-bdd.hpp"

void runnableScenarioWithThenComponent()
{
    const HomomorphicValueValidation<HomomorphicRunnableScenarioWithThenRequirements<Expect, RunnableScenario, Then>>
        acceptable;
    acceptable.run();
}

int main(int argc, const char* const* argv)
{
    suite(
        std::array{
            "runnable-scenario-with-then-component",
        },
        std::array{
            runnableScenarioWithThenComponent,
        }
    )
        .run(argc, argv);

    return EXIT_SUCCESS;
}
