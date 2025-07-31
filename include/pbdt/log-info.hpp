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
            ColoredString(const std::string str, NonTypePlaceHolder<COLOR> placeHolder) :
                str{
                    str,
                },
                placeHolder{
                    placeHolder,
                }
            {
            }

            operator std::string() const
            {
                return std::visit(
                    [this](const auto holder)
                    {
                        return format(holder);
                    },
                    placeHolder
                );
            }

        private:
            template <Color COLOR>
            std::string format(NonTypePlaceHolder<COLOR>) const;

            std::string str;
            std::variant<
                NonTypePlaceHolder<Color::PASS>, NonTypePlaceHolder<Color::FAIL>, NonTypePlaceHolder<Color::SKIP>>
                placeHolder;
        };

        template <ColoredString::Color>
        struct ColorFormatable;

        template <>
        struct ColorFormatable<ColoredString::Color::NONE>
        {
            std::string format(const std::string str) const
            {
                return std::format("{}{}", COLOR, str);
            }

        private:
            static constexpr std::string_view COLOR = "\033[0m";
        };

        template <>
        struct ColorFormatable<ColoredString::Color::PASS>
        {
            std::string format(const std::string str) const
            {
                return std::format("{}{}", COLOR, str);
            }

        private:
            static constexpr std::string_view COLOR = "\033[32m";
        };

        template <>
        struct ColorFormatable<ColoredString::Color::FAIL>
        {
            std::string format(const std::string str) const
            {
                return std::format("{}{}", COLOR, str);
            }

        private:
            static constexpr std::string_view COLOR = "\033[31m";
        };

        template <>
        struct ColorFormatable<ColoredString::Color::SKIP>
        {
            std::string format(const std::string str) const
            {
                return std::format("{}{}", COLOR, str);
            }

        private:
            static constexpr std::string_view COLOR = "\033[33m";
        };

        template <ColoredString::Color COLOR>
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
                return colorFormatable.format(str) + defaultFormatable.format(str);
            }

        private:
            std::string str;
            ColorFormatable<COLOR> colorFormatable{};
            ColorFormatable<ColoredString::Color::NONE> defaultFormatable{};
        };

        template <ColoredString::Color COLOR>
        std::string ColoredString::format(NonTypePlaceHolder<COLOR>) const
        {
            return FormatableString<COLOR>{
                str,
            };
        }

        template <ColoredString::Color>
        struct EachFormatable;

        template <>
        struct EachFormatable<ColoredString::Color::PASS>
        {
            std::string format(const std::string str) const
            {
                return std::format(FORMAT, str);
            }

        private:
            static constexpr std::string_view FORMAT = "{} passed";
        };

        template <>
        struct EachFormatable<ColoredString::Color::FAIL>
        {
            std::string format(const std::string str) const
            {
                return std::format(FORMAT, str);
            }

        private:
            static constexpr std::string_view FORMAT = "{} failed";
        };

        template <>
        struct EachFormatable<ColoredString::Color::SKIP>
        {
            std::string format(const std::string str) const
            {
                return std::format(FORMAT, str);
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
                return formatable.format(
                    ColoredString{
                        each,
                        NonTypePlaceHolder<COLOR>{},
                    }
                );
            }

        private:
            pbdt::test_context::detail::EventCountable::Each each;
            EachFormatable<COLOR> formatable{};
        };

        struct EventCountLogInfo
        {
            constexpr EventCountLogInfo(
                const pbdt::test_context::detail::EventCountable::Each passed,
                const pbdt::test_context::detail::EventCountable::Each failed,
                const pbdt::test_context::detail::EventCountable::Each skipped, const size_t total
            ) :
                passed(passed),
                failed(failed),
                skipped(skipped),
                total(total)
            {
            }

            operator std::string() const
            {
                return std::format(
                    "{}, {}, {} | {}", coloredEach<ColoredString::Color::PASS>(passed),
                    coloredEach<ColoredString::Color::FAIL>(failed), coloredEach<ColoredString::Color::SKIP>(skipped),
                    total
                );
            }

        private:
            template <ColoredString::Color COLOR>
            std::string coloredEach(const pbdt::test_context::detail::EventCountable::Each& each) const
            {
                return ColoredEach<COLOR>{
                    each,
                };
            }

            pbdt::test_context::detail::EventCountable::Each passed;
            pbdt::test_context::detail::EventCountable::Each failed;
            pbdt::test_context::detail::EventCountable::Each skipped;
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
