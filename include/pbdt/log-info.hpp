/*
 * SPDX - FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#ifndef PBDT_MODULE

#include <format>
#include <string>
#include <string_view>
#include <variant>

#include "pbdt/test-context.hpp"

#endif

namespace pbdt::log_info
{
    namespace detail
    {
        template <auto>
        struct NonTypePlaceHolder
        {
        };

        struct ColoredString
        {
            enum class Color
            {
                NONE,
                PASS,
                FAIL,
                SKIP,
            };

            template <Color COLOR>
            ColoredString(const std::string str, NonTypePlaceHolder<COLOR>) :
                colored{
                    FormatableString<COLOR>{
                        str,
                    },
                }
            {
            }

            operator std::string() const
            {
                return std::visit(
                    [](const auto colored)
                    {
                        return FormatableString<Color::NONE>(static_cast<std::string>(colored));
                    },
                    colored
                );
            }

        private:
            template <Color>
            struct ColorFormatable;

            template <>
            struct ColorFormatable<Color::NONE>
            {
                std::string format(const std::string str) const
                {
                    return std::format("{}{}", COLOR, str);
                }

            private:
                static constexpr std::string_view COLOR = "\033[0m";
            };

            template <>
            struct ColorFormatable<Color::PASS>
            {
                std::string format(const std::string str) const
                {
                    return std::format("{}{}", COLOR, str);
                }

            private:
                static constexpr std::string_view COLOR = "\033[32m";
            };

            template <>
            struct ColorFormatable<Color::FAIL>
            {
                std::string format(const std::string str) const
                {
                    return std::format("{}{}", COLOR, str);
                }

            private:
                static constexpr std::string_view COLOR = "\033[31m";
            };

            template <>
            struct ColorFormatable<Color::SKIP>
            {
                std::string format(const std::string str) const
                {
                    return std::format("{}{}", COLOR, str);
                }

            private:
                static constexpr std::string_view COLOR = "\033[33m";
            };

            template <Color COLOR>
            struct FormatableString
            {
                FormatableString(const std::string str) :
                    str{
                        str,
                    }
                {
                }

                operator std::string() const
                {
                    return formatable.format(str);
                }

            private:
                std::string str;
                ColorFormatable<COLOR> formatable{};
            };

            std::variant<FormatableString<Color::PASS>, FormatableString<Color::FAIL>, FormatableString<Color::SKIP>>
                colored;
        };

        struct EventCountLogInfo
        {
            constexpr EventCountLogInfo(
                const pbdt::test_context::detail::EventCountable::Each passed,
                const pbdt::test_context::detail::EventCountable::Each failed,
                const pbdt::test_context::detail::EventCountable::Each skipped, const size_t total
            ) :
                EventCountLogInfo{
                    ColoredEach<ColoredString::Color::PASS>{
                        passed,
                    },
                    ColoredEach<ColoredString::Color::FAIL>{
                        failed,
                    },
                    ColoredEach<ColoredString::Color::SKIP>{
                        skipped,
                    },
                    total,
                }
            {
            }

            operator std::string() const
            {
                return std::format(
                    "{}, {}, {} | {}", static_cast<std::string>(passed), static_cast<std::string>(failed),
                    static_cast<std::string>(skipped), total
                );
            }

        private:
            template <ColoredString::Color>
            struct EachFormatable;

            template <>
            struct EachFormatable<ColoredString::Color::PASS>
            {
                std::string format(const pbdt::test_context::detail::EventCountable::Each& each) const
                {
                    return std::format(FORMAT, static_cast<std::string>(each));
                }

            private:
                static constexpr std::string_view FORMAT = "{} passed";
            };

            template <>
            struct EachFormatable<ColoredString::Color::FAIL>
            {
                std::string format(const pbdt::test_context::detail::EventCountable::Each& each) const
                {
                    return std::format(FORMAT, static_cast<std::string>(each));
                }

            private:
                static constexpr std::string_view FORMAT = "{} failed";
            };

            template <>
            struct EachFormatable<ColoredString::Color::SKIP>
            {
                std::string format(const pbdt::test_context::detail::EventCountable::Each& each) const
                {
                    return std::format(FORMAT, static_cast<std::string>(each));
                }

            private:
                static constexpr std::string_view FORMAT = "{} skipped";
            };

            template <ColoredString::Color COLOR>
            struct ColoredEach
            {
                constexpr ColoredEach(const pbdt::test_context::detail::EventCountable::Each each) :
                    each{
                        each,
                    }
                {
                }

                operator std::string() const
                {
                    return ColoredString{
                        formatable.format(each),
                        NonTypePlaceHolder<ColoredString::Color::PASS>{},
                    };
                }

            private:
                pbdt::test_context::detail::EventCountable::Each each;
                EachFormatable<COLOR> formatable{};
            };

            constexpr EventCountLogInfo(
                const ColoredEach<ColoredString::Color::PASS> passed,
                const ColoredEach<ColoredString::Color::FAIL> failed,
                const ColoredEach<ColoredString::Color::SKIP> skipped, const size_t total
            ) :
                passed(passed),
                failed(failed),
                skipped(skipped),
                total(total)
            {
            }

            ColoredEach<ColoredString::Color::PASS> passed;
            ColoredEach<ColoredString::Color::FAIL> failed;
            ColoredEach<ColoredString::Color::SKIP> skipped;
            size_t total;
        };

        template <typename Param, typename Result>
        struct FailedTestLogInfo
        {
            Param param;
            Result result;
        };

        template <typename... SampleLogs>
        struct FailedTestSamplesLogInfo
        {
            std::tuple<SampleLogs...> failedSamples;
        };

        struct SampledTestLogInfo
        {
            constexpr SampledTestLogInfo(const pbdt::test_context::detail::SampledTestContext context) :
                SampledTestLogInfo{
                    EventCountLogInfo{
                        context.eachAssertions<pbdt::test_context::detail::EventCountable::EachName::PASSED>(),
                        context.eachAssertions<pbdt::test_context::detail::EventCountable::EachName::FAILED>(),
                        context.eachAssertions<pbdt::test_context::detail::EventCountable::EachName::SKIPPED>(),
                        context.sumOfAssertions(),
                    },
                    EventCountLogInfo{
                        context.eachSampleTests<pbdt::test_context::detail::EventCountable::EachName::PASSED>(),
                        context.eachSampleTests<pbdt::test_context::detail::EventCountable::EachName::FAILED>(),
                        context.eachSampleTests<pbdt::test_context::detail::EventCountable::EachName::SKIPPED>(),
                        context.sumOfSampleTests(),
                    },
                }
            {
            }

            constexpr SampledTestLogInfo(const EventCountLogInfo assertions, const EventCountLogInfo sampleTests) :
                assertions(assertions),
                sampleTests(sampleTests)
            {
            }

            operator std::string() const
            {
                return std::format(
                    "Assertion: {}\nDomain: {}\n", static_cast<std::string>(assertions),
                    static_cast<std::string>(sampleTests)
                );
            }

        private:
            EventCountLogInfo assertions;
            EventCountLogInfo sampleTests;
        };
    }
}
