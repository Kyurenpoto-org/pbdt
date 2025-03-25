/*
 * SPDX - FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "associative.hpp"

import exstd;
import pbdt;

#include "for-injection.hpp"

int main()
{
    const AcceptableRawContext<TwoWayCompletableRawThenContext<Then, Expect>> acceptable;
    acceptable.accept(AssociativeValidator<ToFlatTuple>{});

    return EXIT_SUCCESS;
}
