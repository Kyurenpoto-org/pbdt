/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "properties/idempotent.hpp"

#include "pbdt/bdd.hpp"

#include "for-injection.hpp"

int main()
{
    const AcceptableRawContext<CompletableRawGivenContext<Given>> acceptable;
    acceptable.accept(IdempotentValidator<Given, ToFlatTuple>{});

    return EXIT_SUCCESS;
}
