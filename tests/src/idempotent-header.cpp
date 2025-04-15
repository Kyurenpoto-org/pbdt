/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>

#include "properties/idempotent.hpp"

#include "pbdt/bdd.hpp"

#include "for-injection.hpp"
#include "suite-runner.hpp"

void given()
{
    const IdempotentValidation<ToFlatTuple, IdempotentGivenRequirements<Given>> acceptable;
    acceptable.run();
}

void when()
{
    const IdempotentValidation<ToContainer, IdempotentWhenRequirements<When>> acceptable;
    acceptable.run();
}

void then()
{
    const IdempotentValidation<ToFlatTuple, IdempotentThenRequirements<Then, Expect>> acceptable;
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
