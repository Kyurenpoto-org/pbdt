/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <array>
#include <functional>
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

    /**
     * @brief Performs a compile-time value assertion.
     *
     * @tparam Idx the index to be asserted.
     */
    template <size_t Idx>
    struct IndexedCompileTimeValueAssertion
    {
        template <typename Validatable>
        void operator()(const Validatable& validatable) const
        {
            compileTimeValueAssert(validatable.template a<Idx>(), validatable.template b<Idx>());
        }

    private:
        void compileTimeValueAssert(const auto a, const auto b) const
        {
            constexpr auto compileTimeA = a();
            constexpr auto compileTimeB = b();
            static_assert(compileTimeA == compileTimeB);
        }
    };

    /**
     * @brief Performs a run-time value assertion.
     *
     * @tparam Idx the index to be asserted.
     */
    template <size_t Idx>
    struct IndexedRunTimeValueAssertion
    {
        template <typename Validatable>
        void operator()(const Validatable& validatable) const
        {
            runTimeValueAssert(validatable.template a<Idx>(), validatable.template b<Idx>());
        }

    private:
        void runTimeValueAssert(const auto a, const auto b) const
        {
            const auto runTimeA = a();
            const auto runTimeB = b();
            dynamic_assert(runTimeA == runTimeB);
        }
    };

    /**
     * @brief Performs a two-way assertion, checking both compile-time and run-time values.
     *
     * @tparam Idx the index to be asserted.
     */
    template <size_t Idx>
    struct IndexedTwoWayValueAssertion
    {
        template <typename Validatable>
        void operator()(const Validatable& validatable) const
        {
            std::invoke(IndexedCompileTimeValueAssertion<Idx>{}, validatable);
            std::invoke(IndexedRunTimeValueAssertion<Idx>{}, validatable);
        }
    };

    /**
     * @brief Performs a compile-time proposition assertion.
     *
     * @tparam Idx
     */
    template <size_t Idx>
    struct IndexedCompileTimePropositionAssertion
    {
        template <typename Validatable>
        void operator()(const Validatable& validatable) const
        {
            compileTimePropositionAssert(validatable.template truth<Idx>(), validatable.template falsity<Idx>());
        }

    private:
        void compileTimePropositionAssert(const auto truth, const auto falsity) const
        {
            constexpr bool compileTimeTruth = truth();
            constexpr bool compileTimeFalsity = falsity();
            static_assert(compileTimeTruth && compileTimeFalsity);
        }
    };

    /**
     * @brief Performs a run-time proposition assertion.
     *
     * @tparam Idx
     */
    template <size_t Idx>
    struct IndexedRunTimePropositionAssertion
    {
        template <typename Validatable>
        void operator()(const Validatable& validatable) const
        {
            dynamic_assert(validatable.template truth<Idx>() && validatable.template falsity<Idx>());
        }

        void runTimePropositionAssert(const auto truth, const auto falsity) const
        {
            const bool runTimeTruth = truth();
            const bool runTimeFalsity = falsity();
            dynamic_assert(runTimeTruth && runTimeFalsity);
        }
    };
}

namespace
{
    /**
     * @brief A base structure for iterative validation.
     *
     * @tparam Validatable The type to be asserted.
     */
    template <typename Validatable, template <size_t> typename IndexedAssertion>
    struct IterativeValidationBase
    {
        void run() const
        {
            runImpl<Validatable::size() - 1>();
        }

    private:
        template <size_t Idx>
        void runImpl() const
        {
            std::invoke(IndexedAssertion<Idx>{}, static_cast<const Validatable&>(*this));

            if constexpr (Idx > 0)
            {
                runImpl<Idx - 1>();
            }
        }
    };

    /**
     * @brief Validates run-time value requirements.
     *
     * @details Iterative validation with run-time value assertion.
     *
     * @tparam Validatable The type must provide size(), a<Idx>(), and b<Idx>() members.
     *
     * @see IterativeValidationBase
     * @see IndexedRunTimeValueAssertion
     */
    template <typename Validatable>
    struct RunTimeValueValidationBase : public IterativeValidationBase<Validatable, IndexedRunTimeValueAssertion>
    {
    };

    /**
     * @brief Validates both compile-time and run-time value requirements.
     *
     * @details Iterative validation with two-way assertion.
     *
     * @tparam Validatable The type must provide size(), a<Idx>(), and b<Idx>() members.
     *
     * @see IterativeValidationBase
     * @see IndexedTwoWayValueAssertion
     */
    template <typename Validatable>
    struct TwoWayValueValidationBase : public IterativeValidationBase<Validatable, IndexedTwoWayValueAssertion>
    {
    };

    /**
     * @brief Validates compile-time proposition requirements.
     *
     * @details Iterative validation with compile-time proposition assertion.
     *
     * @tparam Validatable The type must provide size(), truth<Idx>(), and falsity<Idx>() members.
     *
     * @see IterativeValidationBase
     * @see IndexedCompileTimePropositionAssertion
     */
    template <typename Validatable>
    struct CompileTimePropositionValidationBase :
        public IterativeValidationBase<Validatable, IndexedCompileTimePropositionAssertion>
    {
    };

    /**
     * @brief Validates run-time proposition requirements.
     *
     * @details Iterative validation with run-time proposition assertion.
     *
     * @tparam Validatable The type must provide size(), truth<Idx>(), and falsity<Idx>() members.
     *
     * @see IterativeValidationBase
     * @see IndexedRunTimePropositionAssertion
     */
    template <typename Validatable>
    struct RunTimePropositionValidationBase :
        public IterativeValidationBase<Validatable, IndexedRunTimePropositionAssertion>
    {
    };
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

    /**
     * @brief A macro that generates a compile-time random value based on the current time, file, line, and column.
     *
     * @return A compile-time random value.
     */
#define COMPILE_TIME_RANDOM()                                                                                          \
    CompileTimeRandomImpl<                                                                                             \
        stringToArray(__TIME__), stringToArray(__FILE__), std::source_location::current().line(),                      \
        std::source_location::current().column()>::value
}
