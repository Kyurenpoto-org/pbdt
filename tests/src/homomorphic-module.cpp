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
#include "wrapped-deps/for-test-context.hpp"

void runnableScenarioWithThenComponent()
{
    const HomomorphicValueValidation<HomomorphicRunnableScenarioWithThenRequirements<Expect, RunnableScenario, Then>>
        acceptable;
    acceptable.run();
}

void eventCountableSumWithAccumulate()
{
    const HomomorphicValueValidation<HomomorphicEventCountableSumWithAccumulateRequirements<EventCountableWrap>>
        acceptable;
    acceptable.run();
}

void eventCountableSomeFailedWithAccumulate()
{
    const HomomorphicValueValidation<HomomorphicEventCountableSomeFailedWithAccumulateRequirements<EventCountableWrap>>
        acceptable;
    acceptable.run();
}

int main(int argc, const char* const* argv)
{
    suite(
        std::array{
            "runnable-scenario-with-then-component",
            "event-countable-sum-with-accumulate",
            "event-countable-some-failed-with-accumulate",
        },
        std::array{
            runnableScenarioWithThenComponent,
            eventCountableSumWithAccumulate,
            eventCountableSomeFailedWithAccumulate,
        }
    )
        .run(argc, argv);

    return EXIT_SUCCESS;
}
