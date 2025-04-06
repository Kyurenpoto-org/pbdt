/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "properties/associative.hpp"

import exstd;
import pbdt;

#include "for-injection.hpp"

int main()
{
    const AcceptableRawContext<TwoWayCompletableRawGivenContext<Given>> acceptable;
    acceptable.accept(AssociativeValidator<ToFlatTuple>{});

    return EXIT_SUCCESS;
}
