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
    const AcceptableRawContext<CompletableRawGivenContext<Given>> acceptable;
    acceptable.accept(IdempotentValidator<Given, ToFlatTuple>{});
}

void when()
{
    const AcceptableRawContext<CompletableRawWhenContext<When>> acceptable;
    acceptable.accept(IdempotentValidator<When, ToContainer>{});
}

void then()
{
    const AcceptableRawContext<CompletableRawThenContext<Then, Expect>> acceptable;
    acceptable.accept(IdempotentValidator<Then, ToFlatTuple>{});
}

int main(int argc, const char* const* argv)
{
    suite(std::array{ "given", "when", "then" }, std::array{ given, when, then }).run(argc, argv);

    return EXIT_SUCCESS;
}
