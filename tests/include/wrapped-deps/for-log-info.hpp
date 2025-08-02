/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <ranges>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace
{
    struct EventCountLogInfoWrap
    {
        struct ComparablePresentation
        {
            constexpr ComparablePresentation(const pbdt::test_context::detail::EventCountable eventCountable) :
                ComparablePresentation{
                    eventCountable.template each<pbdt::test_context::detail::EventCountable::EachName::PASSED>(),
                    eventCountable.template each<pbdt::test_context::detail::EventCountable::EachName::FAILED>(),
                    eventCountable.template each<pbdt::test_context::detail::EventCountable::EachName::SKIPPED>(),
                    eventCountable.sum(),
                }
            {
            }

            constexpr ComparablePresentation(
                const pbdt::test_context::detail::EventCountable::Each pass,
                const pbdt::test_context::detail::EventCountable::Each fail,
                const pbdt::test_context::detail::EventCountable::Each skip, const size_t total
            ) :
                pass{
                    pass,
                },
                fail{
                    fail,
                },
                skip{
                    skip,
                },
                total{
                    total,
                }
            {
            }

            bool operator==(const ComparablePresentation& other) const
            {
                return static_cast<std::string>(pass) == static_cast<std::string>(other.pass)
                    && static_cast<std::string>(fail) == static_cast<std::string>(other.fail)
                    && static_cast<std::string>(skip) == static_cast<std::string>(other.skip) && total == other.total;
            }

            constexpr operator pbdt::log_info::detail::EventCountLogInfo() const
            {
                return pbdt::log_info::detail::EventCountLogInfo{
                    pass,
                    fail,
                    skip,
                    total,
                };
            }

        private:
            pbdt::test_context::detail::EventCountable::Each pass;
            pbdt::test_context::detail::EventCountable::Each fail;
            pbdt::test_context::detail::EventCountable::Each skip;
            size_t total;
        };

        constexpr ComparablePresentation
        toComparable(const pbdt::test_context::detail::EventCountable eventCountable) const
        {
            return ComparablePresentation{
                eventCountable,
            };
        }

        std::string serialize(const ComparablePresentation presentation) const
        {
            return static_cast<std::string>(static_cast<pbdt::log_info::detail::EventCountLogInfo>(presentation));
        }

        ComparablePresentation deserialize(const std::string str) const
        {
            const auto x = splitStrView(
                std::string_view{
                    str,
                },
                std::string_view{
                    " | ",
                }
            );
            if (x.size() != 2)
            {
                throw std::runtime_error("Invalid format for EventCountLogInfo");
            }

            constexpr std::string_view GREEN = "\033[32m";
            constexpr std::string_view RED = "\033[31m";
            constexpr std::string_view YELLOW = "\033[33m";
            constexpr std::string_view PASSED_END = "%)\033[0m passed";
            constexpr std::string_view FAILED_END = "%)\033[0m failed";
            constexpr std::string_view SKIPPED_END = "%)\033[0m skipped";
            auto y = splitStrView(
                x[0], std::string_view{
                          ", ",
                      }
            );
            if (y.size() != 3 || !y[0].starts_with(GREEN) || !y[0].ends_with(PASSED_END) || !y[1].starts_with(RED)
                || !y[1].ends_with(FAILED_END) || !y[2].starts_with(YELLOW) || !y[2].ends_with(SKIPPED_END))
            {
                throw std::runtime_error("Invalid format for EventCountLogInfo");
            }
            y[0].remove_prefix(GREEN.size());
            y[0].remove_suffix(PASSED_END.size());
            y[1].remove_prefix(RED.size());
            y[1].remove_suffix(FAILED_END.size());
            y[2].remove_prefix(YELLOW.size());
            y[2].remove_suffix(SKIPPED_END.size());

            return ComparablePresentation{
                parseToEach(y[0]),
                parseToEach(y[1]),
                parseToEach(y[2]),
                parseToInteger(x[1]),
            };
        }

    private:
        std::vector<std::string_view> splitStrView(const std::string_view str, const std::string_view delim) const
        {
            return std::views::split(str, delim)
                 | std::views::transform(
                       [](auto x)
                       {
                           return std::string_view{ x };
                       }
                 )
                 | std::ranges::to<std::vector<std::string_view>>();
        }

        size_t parseToInteger(const std::string_view str) const
        {
            int value;
            auto [ptr, ec] = std::from_chars(str.data() + str.find_first_not_of(" "), str.data() + str.size(), value);
            if (ec != std::errc())
            {
                throw std::runtime_error("Invalid format for integer");
            }

            return static_cast<size_t>(value);
        }

        pbdt::test_context::detail::EventCountable::Each parseToEach(const std::string_view str) const
        {
            const auto x = splitStrView(
                str, std::string_view{
                         "(",
                     }
            );
            if (x.size() != 2)
            {
                throw std::runtime_error("Invalid format for Each");
            }

            return {
                parseToInteger(x[0]),
                parseToInteger(x[1]),
            };
        }
    };
}
