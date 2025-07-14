/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>

#include "properties/classes/modeling.hpp"

import exstd;
import pbdt;

#include "suite-runner.hpp"
#include "wrapped-deps/for-bdd.hpp"
#include "wrapped-deps/for-test-context.hpp"

void runnableScenario()
{
    const ModelingValueValidation<ModelingRunnableScenarioRequirements<Expect, ApplyOrInvoke, RunnableScenario>>
        acceptable;
    acceptable.run();
}

void eventCountable()
{
    const ModelingValueValidation<ModelingEventCountableSumRequirements<EventCountableWrap>> acceptable;
    acceptable.run();
}

int main(int argc, const char* const* argv)
{
    suite(
        std::array{
            "runnable-scenario",
            "event-countable",
        },
        std::array{
            runnableScenario,
            eventCountable,
        }
    )
        .run(argc, argv);

    return EXIT_SUCCESS;
}
