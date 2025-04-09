/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>

#include "properties/associative.hpp"

#include "pbdt/bdd.hpp"

#include "for-injection.hpp"
#include "suite-runner.hpp"

void given()
{
    const AcceptableRawContext<TwoWayCompletableRawGivenContext<Given>> acceptable;
    acceptable.accept(AssociativeValidator<ToFlatTuple>{});
}

void when()
{
    const AcceptableRawContext<TwoWayCompletableRawWhenContext<When>> acceptable;
    acceptable.accept(AssociativeValidator<ToContainer>{});
}

void then()
{
    const AcceptableRawContext<TwoWayCompletableRawThenContext<Then, Expect>> acceptable;
    acceptable.accept(AssociativeValidator<ToFlatTuple>{});
}

int main(int argc, const char* const* argv)
{
    suite(std::array{ "given", "when", "then" }, std::array{ given, when, then }).run(argc, argv);

    return EXIT_SUCCESS;
}
