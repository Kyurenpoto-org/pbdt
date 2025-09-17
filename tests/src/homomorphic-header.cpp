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

void eventCountable()
{
    const HomomorphicValueValidation<HomomorphicEventCountableRequirements<pbdt::test_context::detail::EventCountable>>
        acceptable;
    acceptable.run();
}

void expectationContext()
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
            "event-countable",
            "expectation-context",
        },
        std::array{
            runnableScenarioWithThenComponent,
            eventCountable,
            expectationContext,
        }
    )
        .run(argc, argv);

    return EXIT_SUCCESS;
}
