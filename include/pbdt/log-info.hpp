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

#endif

namespace pbdt::log_info::detail
{
    template <typename T>
    concept LogInfo = requires(T x) {
        { x } -> std::convertible_to<std::string>;
    };

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
        std::variant<NonTypePlaceHolder<Color::PASS>, NonTypePlaceHolder<Color::FAIL>, NonTypePlaceHolder<Color::SKIP>>
            placeHolder;
    };

    template <ColoredString::Color>
    struct ColorFormatable;

    template <>
    struct ColorFormatable<ColoredString::Color::NONE>
    {
        std::string format(const std::string_view str) const
        {
            return std::format("{}{}", COLOR, str);
        }

    private:
        static constexpr std::string_view COLOR = "\033[0m";
    };

    template <>
    struct ColorFormatable<ColoredString::Color::PASS>
    {
        std::string format(const std::string_view str) const
        {
            return std::format("{}{}", COLOR, str);
        }

    private:
        static constexpr std::string_view COLOR = "\033[32m";
    };

    template <>
    struct ColorFormatable<ColoredString::Color::FAIL>
    {
        std::string format(const std::string_view str) const
        {
            return std::format("{}{}", COLOR, str);
        }

    private:
        static constexpr std::string_view COLOR = "\033[31m";
    };

    template <>
    struct ColorFormatable<ColoredString::Color::SKIP>
    {
        std::string format(const std::string_view str) const
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
            return colorFormatable.format(str) + defaultFormatable.format("");
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
        std::string format(const std::string_view str) const
        {
            return std::format(FORMAT, str);
        }

    private:
        static constexpr std::string_view FORMAT = "{} passed";
    };

    template <>
    struct EachFormatable<ColoredString::Color::FAIL>
    {
        std::string format(const std::string_view str) const
        {
            return std::format(FORMAT, str);
        }

    private:
        static constexpr std::string_view FORMAT = "{} failed";
    };

    template <>
    struct EachFormatable<ColoredString::Color::SKIP>
    {
        std::string format(const std::string_view str) const
        {
            return std::format(FORMAT, str);
        }

    private:
        static constexpr std::string_view FORMAT = "{} skipped";
    };

    template <LogInfo EachInfo, ColoredString::Color COLOR>
    struct ColoredEach
    {
        constexpr ColoredEach(const EachInfo each) :
            each{
                each,
            }
        {
        }

        operator std::string() const
        {
            const std::string colored = ColoredString{
                each,
                NonTypePlaceHolder<COLOR>{},
            };

            return formatable.format(colored);
        }

    private:
        EachInfo each;
        EachFormatable<COLOR> formatable{};
    };

    template <LogInfo EachInfo>
    struct EventCountLogInfo
    {
        constexpr EventCountLogInfo(
            const EachInfo passed, const EachInfo failed, const EachInfo skipped, const size_t total
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
                coloredEach<ColoredString::Color::FAIL>(failed), coloredEach<ColoredString::Color::SKIP>(skipped), total
            );
        }

    private:
        template <ColoredString::Color COLOR>
        std::string coloredEach(const EachInfo& each) const
        {
            return ColoredEach<EachInfo, COLOR>{
                each,
            };
        }

        EachInfo passed;
        EachInfo failed;
        EachInfo skipped;
        size_t total;
    };
}
