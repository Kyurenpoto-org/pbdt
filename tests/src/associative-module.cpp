/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>

#include "properties/classes/associative.hpp"

import exstd;
import pbdt;

#include "suite-runner.hpp"
#include "wrapped-deps/for-bdd.hpp"

void given()
{
    const AssociativeValueValidation<ToFlatTuple, AssociativeGivenRequirements<Given>> acceptable;
    acceptable.run();
}

void when()
{
    const AssociativeValueValidation<ToContainer, AssociativeWhenRequirements<When>> acceptable;
    acceptable.run();
}

void then()
{
    const AssociativeValueValidation<ToFlatTuple, AssociativeThenRequirements<Expect, Then>> acceptable;
    acceptable.run();
}

void expectationContext()
{
    using REQ = AssociativeExpectationContextRequirements<
        pbdt::test_context::detail::ExpectationContext, pbdt::test_context::detail::EventCountable>;
    const AssociativeRunTimeValueValidation<typename REQ::ToComparable, REQ> acceptable;
    acceptable.run();
}

int main(int argc, const char* const* argv)
{
    suite(
        std::array{
            "given",
            "when",
            "then",
            "expectation-context",
        },
        std::array{
            given,
            when,
            then,
            expectationContext,
        }
    )
        .run(argc, argv);

    return EXIT_SUCCESS;
}
