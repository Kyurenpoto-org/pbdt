/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "properties/model.hpp"

#include "pbdt/bdd.hpp"

#include "for-injection.hpp"

int main()
{
    const AcceptableCombination<TwoWayRunnableScenarioCombination<Expect, ApplyOrInvoke, RunnableScenario>> acceptable;
    acceptable.accept(ModelValidator{});

    return EXIT_SUCCESS;
}
