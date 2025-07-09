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
                    ColorFormatable<COLOR>{
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
                        return ColorFormatable<Color::NONE>(colored);
                    },
                    colored
                );
            }

        private:
            struct FormatableString
            {
                FormatableString(const std::string str) :
                    str{
                        str,
                    }
                {
                }

                std::string format(const std::string_view color) const
                {
                    return std::format("{}{}", color, str);
                }

            private:
                std::string str;
            };

            template <typename Derived>
            struct ColorFormatableBase
            {
                ColorFormatableBase(const std::string str) :
                    ColorFormatableBase{
                        FormatableString{
                            str,
                        },
                    }
                {
                }

                ColorFormatableBase(const FormatableString str) :
                    str{
                        str,
                    }
                {
                }

                operator std::string() const
                {
                    return str.format(color());
                }

            protected:
                constexpr std::string_view color() const;

            private:
                FormatableString str;
            };

            template <Color>
            struct ColorFormatable;

            template <>
            struct ColorFormatable<Color::NONE> : public ColorFormatableBase<ColorFormatable<Color::NONE>>
            {
                ColorFormatable(const std::string str) :
                    ColorFormatableBase<ColorFormatable<Color::NONE>>{
                        str,
                    }
                {
                }

            protected:
                constexpr std::string_view color() const
                {
                    return COLOR;
                }

            private:
                static constexpr std::string_view COLOR = "\033[0m";
            };

            template <>
            struct ColorFormatable<Color::PASS> : public ColorFormatableBase<ColorFormatable<Color::PASS>>
            {
                ColorFormatable(const std::string str) :
                    ColorFormatableBase<ColorFormatable<Color::PASS>>{
                        str,
                    }
                {
                }

            protected:
                constexpr std::string_view color() const
                {
                    return COLOR;
                }

            private:
                static constexpr std::string_view COLOR = "\033[32m";
            };

            template <>
            struct ColorFormatable<Color::FAIL> : public ColorFormatableBase<ColorFormatable<Color::FAIL>>
            {
                ColorFormatable(const std::string str) :
                    ColorFormatableBase<ColorFormatable<Color::FAIL>>{
                        str,
                    }
                {
                }

            protected:
                constexpr std::string_view color() const
                {
                    return COLOR;
                }

            private:
                static constexpr std::string_view COLOR = "\033[31m";
            };

            template <>
            struct ColorFormatable<Color::SKIP> : public ColorFormatableBase<ColorFormatable<Color::SKIP>>
            {
                ColorFormatable(const std::string str) :
                    ColorFormatableBase<ColorFormatable<Color::SKIP>>{
                        str,
                    }
                {
                }

            protected:
                constexpr std::string_view color() const
                {
                    return COLOR;
                }

            private:
                static constexpr std::string_view COLOR = "\033[33m";
            };

            std::variant<ColorFormatable<Color::PASS>, ColorFormatable<Color::FAIL>, ColorFormatable<Color::SKIP>>
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
                    ColoredAmountWithRate<ColoredString::Color::PASS>{
                        AmountWithRate{
                            passed,
                        },
                    },
                    ColoredAmountWithRate<ColoredString::Color::FAIL>{
                        AmountWithRate{
                            failed,
                        },
                    },
                    ColoredAmountWithRate<ColoredString::Color::SKIP>{
                        AmountWithRate{
                            skipped,
                        },
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
            struct AmountWithRate
            {
                constexpr AmountWithRate(const pbdt::test_context::detail::EventCountable::Each each) :
                    each(each)
                {
                }

                operator std::string() const
                {
                    return std::format("{:>5} ({:>3}%)", each.amount, each.rate);
                }

            private:
                pbdt::test_context::detail::EventCountable::Each each;
            };

            template <typename Derived>
            struct ColoredAmountWithRateBase
            {
                constexpr ColoredAmountWithRateBase(const AmountWithRate amountWithRate) :
                    amountWithRate{
                        amountWithRate,
                    }
                {
                }

                operator std::string() const
                {
                    return ColoredString{
                        format(amountWithRate),
                        NonTypePlaceHolder<ColoredString::Color::PASS>{},
                    };
                }

            protected:
                std::string format(const AmountWithRate&) const;

            private:
                AmountWithRate amountWithRate;
            };

            template <ColoredString::Color>
            struct ColoredAmountWithRate;

            template <>
            struct ColoredAmountWithRate<ColoredString::Color::PASS> :
                public ColoredAmountWithRateBase<ColoredAmountWithRate<ColoredString::Color::PASS>>
            {
                constexpr ColoredAmountWithRate(const AmountWithRate amountWithRate) :
                    ColoredAmountWithRateBase<ColoredAmountWithRate<ColoredString::Color::PASS>>{
                        amountWithRate,
                    }
                {
                }

            protected:
                std::string format(const AmountWithRate& amountWithRate) const
                {
                    return std::format(FORMAT, static_cast<std::string>(amountWithRate));
                }

            private:
                static constexpr std::string_view FORMAT = "{} passed";
            };

            template <>
            struct ColoredAmountWithRate<ColoredString::Color::FAIL> :
                public ColoredAmountWithRateBase<ColoredAmountWithRate<ColoredString::Color::FAIL>>
            {
                constexpr ColoredAmountWithRate(const AmountWithRate amountWithRate) :
                    ColoredAmountWithRateBase<ColoredAmountWithRate<ColoredString::Color::FAIL>>{
                        amountWithRate,
                    }
                {
                }

            protected:
                std::string format(const AmountWithRate& amountWithRate) const
                {
                    return std::format(FORMAT, static_cast<std::string>(amountWithRate));
                }

            private:
                static constexpr std::string_view FORMAT = "{} failed";
            };

            template <>
            struct ColoredAmountWithRate<ColoredString::Color::SKIP> :
                public ColoredAmountWithRateBase<ColoredAmountWithRate<ColoredString::Color::SKIP>>
            {
                constexpr ColoredAmountWithRate(const AmountWithRate amountWithRate) :
                    ColoredAmountWithRateBase<ColoredAmountWithRate<ColoredString::Color::SKIP>>{
                        amountWithRate,
                    }
                {
                }

            protected:
                std::string format(const AmountWithRate& amountWithRate) const
                {
                    return std::format(FORMAT, static_cast<std::string>(amountWithRate));
                }

            private:
                static constexpr std::string_view FORMAT = "{} skipped";
            };

            constexpr EventCountLogInfo(
                const ColoredAmountWithRate<ColoredString::Color::PASS> passed,
                const ColoredAmountWithRate<ColoredString::Color::FAIL> failed,
                const ColoredAmountWithRate<ColoredString::Color::SKIP> skipped, const size_t total
            ) :
                passed(passed),
                failed(failed),
                skipped(skipped),
                total(total)
            {
            }

            ColoredAmountWithRate<ColoredString::Color::PASS> passed;
            ColoredAmountWithRate<ColoredString::Color::FAIL> failed;
            ColoredAmountWithRate<ColoredString::Color::SKIP> skipped;
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
