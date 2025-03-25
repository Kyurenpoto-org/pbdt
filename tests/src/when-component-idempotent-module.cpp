/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "idempotent.hpp"

import exstd;
import pbdt;

#include "for-injection.hpp"

int main()
{
    const AcceptableRawContext<CompletableRawWhenContext<When>> acceptable;
    acceptable.accept(IdempotentValidator<When, ToContainer>{});

    return EXIT_SUCCESS;
}
