/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>

#include "properties/classes/commutative.hpp"

#include "pbdt/test-context.hpp"

#include "suite-runner.hpp"

void expectationContext()
{
    using REQ = CommutativeExpectationContextRequirements<
        pbdt::test_context::detail::ExpectationContext, pbdt::test_context::detail::EventCountable>;
    const CommutativeRunTimeValueValidation<typename REQ::ToComparable, REQ> acceptable;
    acceptable.run();
}

int main(int argc, const char* const* argv)
{
    suite(
        std::array{
            "expectation-context",
        },
        std::array{
            expectationContext,
        }
    )
        .run(argc, argv);

    return EXIT_SUCCESS;
}
