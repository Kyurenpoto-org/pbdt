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
    const AcceptableRawContext<ToFlatTuple, CompletableRawGivenContext<Given>> acceptable;
    acceptable.accept(IdempotentValidator{});
}

void when()
{
    const AcceptableRawContext<ToContainer, CompletableRawWhenContext<When>> acceptable;
    acceptable.accept(IdempotentValidator{});
}

void then()
{
    const AcceptableRawContext<ToFlatTuple, CompletableRawThenContext<Then, Expect>> acceptable;
    acceptable.accept(IdempotentValidator{});
}

int main(int argc, const char* const* argv)
{
    suite(std::array{ "given", "when", "then" }, std::array{ given, when, then }).run(argc, argv);

    return EXIT_SUCCESS;
}
