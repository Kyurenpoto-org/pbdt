/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <regex>
#include <string>
#include <tuple>

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
            std::regex regex{ R"(\s*(\d+)\s*\(\s*(\d+)%\s*\)\s*passed,)"
                              R"(\s*(\d+)\s*\(\s*(\d+)%\s*\)\s*failed,)"
                              R"(\s*(\d+)\s*\(\s*(\d+)%\s*\)\s*skipped\s*\|\s*(\d+))" };
            std::smatch match;
            if (std::regex_match(str, match, regex))
            {
                return ComparablePresentation{
                    pbdt::test_context::detail::EventCountable::Each{
                        std::stoul(match[1]),
                        std::stoul(match[2]),
                    },
                    pbdt::test_context::detail::EventCountable::Each{
                        std::stoul(match[3]),
                        std::stoul(match[4]),
                    },
                    pbdt::test_context::detail::EventCountable::Each{
                        std::stoul(match[5]),
                        std::stoul(match[6]),
                    },
                    std::stoul(match[7]),
                };
            }
            else
            {
                throw std::runtime_error("Invalid format for EventCountLogInfo");
            }
        }
    };
}
