/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>
#include <charconv>
#include <ranges>
#include <variant>

#include "properties/classes/symmetric.hpp"

import pbdt;

#include "suite-runner.hpp"
#include "wrapped-deps/for-log-info.hpp"
#include "wrapped-deps/for-test-context.hpp"

void eventCountLogInfoForward()
{
    const SymmetricRuntimeValueValidation<
        SymmetricEventCountLogInfoForwardRequirements<EventCountableWrap, EventCountLogInfoWrap>>
        acceptable;
    acceptable.run();
}

void eventCountLogInfoBackward()
{
    const SymmetricRuntimeValueValidation<
        SymmetricEventCountLogInfoBackwardRequirements<EventCountableWrap, EventCountLogInfoWrap>>
        acceptable;
    acceptable.run();
}

int main(int argc, const char* const* argv)
{
    suite(
        std::array{
            "event-count-log-info-forward",
            "event-count-log-info-backward",
        },
        std::array{
            eventCountLogInfoForward,
            eventCountLogInfoBackward,
        }
    )
        .run(argc, argv);

    return EXIT_SUCCESS;
}
