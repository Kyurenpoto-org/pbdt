/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>

#include "properties/classes/projective.hpp"

import exstd;
import pbdt;

#include "for-injection.hpp"
#include "suite-runner.hpp"

void runnableScenarioWithGivenComponent()
{
    const LeftProjectiveValidation<
        ProjectiveRunnableScenarioWithGivenRequirements<ToContainer, Expect, RunnableScenario, Given>>
        acceptableL;
    acceptableL.run();
}

void runnableScenarioWithWhenComponent()
{
    const LeftProjectiveValidation<
        ProjectiveRunnableScenarioWithWhenRequirements<ToContainer, Expect, RunnableScenario, When>>
        acceptableL;
    acceptableL.run();

    const RightProjectiveValidation<
        ProjectiveRunnableScenarioWithWhenRequirements<ToContainer, Expect, RunnableScenario, When>>
        acceptableR;
    acceptableR.run();
}

int main(int argc, const char* const* argv)
{
    suite(
        std::array{
            "runnable-scenario-with-given-component",
            "runnable-scenario-with-when-component",
        },
        std::array{
            runnableScenarioWithGivenComponent,
            runnableScenarioWithWhenComponent,
        }
    )
        .run(argc, argv);

    return EXIT_SUCCESS;
}
