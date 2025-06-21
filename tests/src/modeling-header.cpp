/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>

#include "properties/classes/modeling.hpp"

#include "pbdt/bdd.hpp"

#include "for-injection.hpp"
#include "suite-runner.hpp"

void runnableScenario()
{
    const ModelingValueValidation<ModelingRunnableScenarioRequirements<Expect, ApplyOrInvoke, RunnableScenario>>
        acceptable;
    acceptable.run();
}

int main(int argc, const char* const* argv)
{
    suite(
        std::array{
            "runnable-scenario",
        },
        std::array{
            runnableScenario,
        }
    )
        .run(argc, argv);

    return EXIT_SUCCESS;
}
