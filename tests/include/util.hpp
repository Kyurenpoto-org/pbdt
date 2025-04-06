/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <array>
#include <iostream>
#include <source_location>

#if __cpp_lib_print >= 202207L
#include <print>
#endif

namespace
{
    void dynamic_assert(const bool expr, const std::source_location& location = std::source_location::current())
    {
        if (!expr)
        {
#if __cpp_lib_print >= 202207L
            std::println("{0}({1},{2})", location.file_name(), location.line(), location.column());
#else
            std::cerr << location.file_name() << "(" << location.line() << "," << location.column() << ")\n";
#endif
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
    constexpr uint32_t hash(const std::array<char, N> str)
    {
        uint32_t result = 0;
        for (uint32_t i = 0; i < N; ++i)
        {
            result ^= static_cast<uint32_t>(str[i]) << (i % 4 * 8);
        }

        return hash(result);
    }

    template <auto>
    struct CompileTimeHash;

    template <uint32_t N>
    struct CompileTimeHash<N>
    {
        static constexpr uint32_t value = hash(N);
    };

    template <size_t N, std::array<char, N> Str>
    struct CompileTimeHash<Str>
    {
        static constexpr uint32_t value = hash(Str);
    };

    template <std::size_t N>
    consteval auto stringToArray(const char (&str)[N])
    {
        std::array<char, N> arr{};
        for (std::size_t i = 0; i < N; ++i)
        {
            arr[i] = str[i];
        }
        return arr;
    }

    template <auto Time, auto File, auto Line, auto Column>
    struct CompileTimeRandomImpl
    {
        static constexpr uint32_t value = hash(
            CompileTimeHash<Time>::value ^ CompileTimeHash<File>::value ^ CompileTimeHash<Line>::value
            ^ CompileTimeHash<Column>::value
        );
    };

#define COMPILE_TIME_RANDOM()                                                                                          \
    CompileTimeRandomImpl<                                                                                             \
        stringToArray(__TIME__), stringToArray(__FILE__), std::source_location::current().line(),                      \
        std::source_location::current().column()>::value
}
