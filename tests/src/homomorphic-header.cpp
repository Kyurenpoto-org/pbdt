/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>

#include "properties/homomorphic.hpp"

#include "pbdt/bdd.hpp"

#include "for-injection.hpp"
#include "suite-runner.hpp"

void runnableScenarioWithThenComponent()
{
    const HomomorphicValidation<HomomorphicRunnableScenarioWithThenRequirements<Expect, RunnableScenario, Then>>
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
