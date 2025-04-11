/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>

#include "properties/associative.hpp"

import exstd;
import pbdt;

#include "for-injection.hpp"
#include "suite-runner.hpp"

void given()
{
    const AcceptableRawContext<ToFlatTuple, TwoWayCompletableRawGivenContext<Given>> acceptable;
    acceptable.accept(AssociativeValidator{});
}

void when()
{
    const AcceptableRawContext<ToContainer, TwoWayCompletableRawWhenContext<When>> acceptable;
    acceptable.accept(AssociativeValidator{});
}

void then()
{
    const AcceptableRawContext<ToFlatTuple, TwoWayCompletableRawThenContext<Expect, Then>> acceptable;
    acceptable.accept(AssociativeValidator{});
}

int main(int argc, const char* const* argv)
{
    suite(std::array{ "given", "when", "then" }, std::array{ given, when, then }).run(argc, argv);

    return EXIT_SUCCESS;
}
