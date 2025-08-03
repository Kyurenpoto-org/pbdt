/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <variant>

namespace
{
    struct EventCountableWrap
    {
        struct PassOp
        {
            constexpr pbdt::test_context::detail::EventCountable
            operator()(const pbdt::test_context::detail::EventCountable x) const
            {
                return x.pass();
            }
        };

        struct FailOp
        {
            constexpr pbdt::test_context::detail::EventCountable
            operator()(const pbdt::test_context::detail::EventCountable x) const
            {
                return x.fail();
            }
        };

        struct SkipOp
        {
            constexpr pbdt::test_context::detail::EventCountable
            operator()(const pbdt::test_context::detail::EventCountable x) const
            {
                return x.skip();
            }
        };

        struct OpContainer
        {
            constexpr pbdt::test_context::detail::EventCountable
            operator()(const pbdt::test_context::detail::EventCountable x) const
            {
                return std::visit(
                    [x](const auto op)
                    {
                        return op(x);
                    },
                    op
                );
            }

            std::variant<PassOp, FailOp, SkipOp> op;
        };

        template <size_t N>
            requires(N < 3)
        static constexpr OpContainer op()
        {
            if constexpr (N == 0)
            {
                return OpContainer{
                    .op = PassOp{},
                };
            }
            else if constexpr (N == 1)
            {
                return OpContainer{
                    .op = FailOp{},
                };
            }
            else
            {
                return OpContainer{
                    .op = SkipOp{},
                };
            }
        }

        template <size_t N>
            requires(N < 3)
        static constexpr pbdt::test_context::detail::EventCountable::Each
        each(pbdt::test_context::detail::EventCountable eventCountable)
        {
            if constexpr (N == 0)
            {
                return eventCountable.each<pbdt::test_context::detail::EventCountable::EachName::PASSED>();
            }
            else if constexpr (N == 1)
            {
                return eventCountable.each<pbdt::test_context::detail::EventCountable::EachName::FAILED>();
            }
            else
            {
                return eventCountable.each<pbdt::test_context::detail::EventCountable::EachName::SKIPPED>();
            }
        }

        constexpr pbdt::test_context::detail::EventCountable prototype() const
        {
            return pbdt::test_context::detail::EventCountable::prototype();
        }
    };
}
