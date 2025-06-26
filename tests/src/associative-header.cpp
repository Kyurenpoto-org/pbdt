/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>

#include "properties/classes/associative.hpp"

#include "pbdt/bdd.hpp"

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
