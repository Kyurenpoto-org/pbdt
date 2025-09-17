/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>

#include "properties/classes/commutative.hpp"

import exstd;
import pbdt;

#include "suite-runner.hpp"

void eventCountable()
{
    const CommutativeRunTimeValueValidation<
        CommutativeEventCountableRequirements<pbdt::test_context::detail::EventCountable>>
        acceptable;
    acceptable.run();
}

void expectationContext()
{
    const CommutativeRunTimeValueValidation<CommutativeExpectationContextRequirements<
        pbdt::test_context::detail::ExpectationContext, pbdt::test_context::detail::EventCountable>>
        acceptable;
    acceptable.run();
}

int main(int argc, const char* const* argv)
{
    suite(
        std::array{
            "event-countable",
            "expectation-context",
        },
        std::array{
            eventCountable,
            expectationContext,
        }
    )
        .run(argc, argv);

    return EXIT_SUCCESS;
}
