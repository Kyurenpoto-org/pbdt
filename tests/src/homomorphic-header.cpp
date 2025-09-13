/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>

#include "properties/classes/homomorphic.hpp"

#include "pbdt/bdd.hpp"
#include "pbdt/test-context.hpp"

#include "suite-runner.hpp"
#include "wrapped-deps/for-bdd.hpp"

void runnableScenarioWithThenComponent()
{
    const HomomorphicValueValidation<HomomorphicRunnableScenarioWithThenRequirements<Expect, RunnableScenario, Then>>
        acceptable;
    acceptable.run();
}

void eventCountableSumWithAccumulate()
{
    const HomomorphicValueValidation<
        HomomorphicEventCountableSumWithAccumulateRequirements<pbdt::test_context::detail::EventCountable>>
        acceptable;
    acceptable.run();
}

void eventCountableSomeFailedWithAccumulate()
{
    const HomomorphicValueValidation<
        HomomorphicEventCountableSomeFailedWithAccumulateRequirements<pbdt::test_context::detail::EventCountable>>
        acceptable;
    acceptable.run();
}

void expectationContextCountAndReport()
{
    const HomomorphicRunTimeValueValidation<HomomorphicExpectationContextRequirements<
        pbdt::test_context::detail::ExpectationContext, pbdt::test_context::detail::EventCountable>>
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
            "expectation-context-count-and-report",
        },
        std::array{
            runnableScenarioWithThenComponent,
            eventCountableSumWithAccumulate,
            eventCountableSomeFailedWithAccumulate,
            expectationContextCountAndReport,
        }
    )
        .run(argc, argv);

    return EXIT_SUCCESS;
}
