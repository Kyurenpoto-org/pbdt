/*
 * SPDX - FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "properties/idempotent.hpp"

#include "pbdt/bdd.hpp"

#include "for-injection.hpp"

int main()
{
    const AcceptableRawContext<CompletableRawThenContext<Then, Expect>> acceptable;
    acceptable.accept(IdempotentValidator<Then, ToFlatTuple>{});

    return EXIT_SUCCESS;
}
