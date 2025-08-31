/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>

#include "properties/classes/projective.hpp"

import exstd;
import pbdt;

#include "suite-runner.hpp"
#include "wrapped-deps/for-bdd.hpp"

void runnableScenarioWithGivenComponent()
{
    const LeftProjectiveValueValidation<
        ProjectiveRunnableScenarioWithGivenRequirements<ToContainer, Expect, RunnableScenario, Given>>
        acceptableL;
    acceptableL.run();
}

void runnableScenarioWithWhenComponent()
{
    const LeftProjectiveValueValidation<
        ProjectiveRunnableScenarioWithWhenRequirements<ToContainer, Expect, RunnableScenario, When>>
        acceptableL;
    acceptableL.run();

    const RightProjectiveValueValidation<
        ProjectiveRunnableScenarioWithWhenRequirements<ToContainer, Expect, RunnableScenario, When>>
        acceptableR;
    acceptableR.run();
}

void eventCountableEach()
{
    const DoubleProjectiveRuntimeValueValidation<
        ProjectiveEventCountableEachRequirements<pbdt::test_context::detail::EventCountable>>
        acceptable;
    acceptable.run();
}

int main(int argc, const char* const* argv)
{
    suite(
        std::array{
            "runnable-scenario-with-given-component",
            "runnable-scenario-with-when-component",
            "event-countable-each",
        },
        std::array{
            runnableScenarioWithGivenComponent,
            runnableScenarioWithWhenComponent,
            eventCountableEach,
        }
    )
        .run(argc, argv);

    return EXIT_SUCCESS;
}
