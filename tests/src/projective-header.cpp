/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>

#include "properties/projective.hpp"

#include "pbdt/bdd.hpp"

#include "for-injection.hpp"
#include "suite-runner.hpp"

void runnableScenarioWithWhenComponent()
{
    const LeftProjectiveValidation<
        ProjectiveRunnableScenarioWithWhenRequirements<ToContainer, ApplyOrInvoke, Expect, RunnableScenario, When>>
        acceptableL;
    acceptableL.run();

    const RightProjectiveValidation<
        ProjectiveRunnableScenarioWithWhenRequirements<ToContainer, ApplyOrInvoke, Expect, RunnableScenario, When>>
        acceptableR;
    acceptableR.run();
}

int main(int argc, const char* const* argv)
{
    suite(std::array{ "runnable-scenario-with-when-component" }, std::array{ runnableScenarioWithWhenComponent })
        .run(argc, argv);

    return EXIT_SUCCESS;
}
