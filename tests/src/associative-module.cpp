/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>

#include "properties/classes/associative.hpp"

import exstd;
import pbdt;

#include "for-injection.hpp"
#include "suite-runner.hpp"

void given()
{
    const AssociativeValidation<ToFlatTuple, AssociativeGivenRequirements<Given>> acceptable;
    acceptable.run();
}

void when()
{
    const AssociativeValidation<ToContainer, AssociativeWhenRequirements<When>> acceptable;
    acceptable.run();
}

void then()
{
    const AssociativeValidation<ToFlatTuple, AssociativeThenRequirements<Expect, Then>> acceptable;
    acceptable.run();
}

int main(int argc, const char* const* argv)
{
    suite(
        std::array{
            "given",
            "when",
            "then",
        },
        std::array{
            given,
            when,
            then,
        }
    )
        .run(argc, argv);

    return EXIT_SUCCESS;
}
