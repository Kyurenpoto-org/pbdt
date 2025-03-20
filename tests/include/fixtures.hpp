/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <array>
#include <print>
#include <source_location>

namespace
{
    void dynamic_assert(const bool expr, const std::source_location& location = std::source_location::current())
    {
        if (!expr)
        {
            std::println("{0}({1},{2})", location.file_name(), location.line(), location.column());
            std::exit(EXIT_FAILURE);
        }
    }
}

// https://gist.github.com/KoneLinx/d3601597248bed423daf1a7cf7bd9533
namespace
{
    constexpr uint32_t hash(uint32_t x)
    {
        constexpr std::array<uint32_t, 16> r{
            0xdf15236c, 0x16d16793, 0x3a697614, 0xe0fe08e4, 0xa3a53275, 0xccc10ff9, 0xb92fae55, 0xecf491de,
            0x36e86773, 0x0ed24a6a, 0xd7153d80, 0x84adf386, 0x17110e76, 0x6d411a6a, 0xcbd41fed, 0x4b1d6b30,
        };
        uint32_t result = x ^ r[x & 0xf];
        result ^= std::rotl(x, 020) ^ r[(x >> 010) & 0xF];
        result ^= std::rotl(x, 010) ^ r[(x >> 020) & 0xF];
        result ^= std::rotr(x, 010) ^ r[(x >> 030) & 0xF];

        return result;
    }

    template <size_t N>
    constexpr uint32_t hash(char const (&str)[N])
    {
        uint32_t result = 0;
        for (uint32_t i = 0; i < N; ++i)
        {
            result ^= static_cast<uint32_t>(str[i]) << (i % 4 * 8);
        }

        return hash(result);
    }

    template <size_t N>
    constexpr uint32_t
    compileTimeRandomImpl(char const (&file)[N], const std::source_location& location = std::source_location::current())
    {
        return hash(hash(__TIME__) ^ hash(file) ^ hash(location.line()) ^ hash(location.column()));
    }

#define COMPILE_TIME_RANDOM() compileTimeRandomImpl(__FILE__)
}
