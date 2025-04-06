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
    const AcceptableRawContext<CompletableRawWhenContext<When>> acceptable;
    acceptable.accept(IdempotentValidator<When, ToContainer>{});

    return EXIT_SUCCESS;
}
