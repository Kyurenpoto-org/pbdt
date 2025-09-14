/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <array>
#include <ranges>
#include <variant>

namespace Expandable
{
    template <std::ranges::input_range Range>
    struct ExpectationContextSequence
    {
        consteval ExpectationContextSequence(Range indice) :
            indice(indice)
        {
        }

        template <template <size_t> typename ExpectationContext, size_t N>
        consteval auto value() const
        {
            return generate<ExpectationContext>(std::make_index_sequence<N>{});
        }

    private:
        template <template <size_t> typename ExpectationContext, size_t... INDICE>
        consteval std::array<std::variant<ExpectationContext<INDICE + 1>...>, sizeof...(INDICE)>
        generate(std::index_sequence<INDICE...>) const
        {
            constexpr size_t N = sizeof...(INDICE);
            std::vector<std::variant<ExpectationContext<INDICE + 1>...>> vec;
            vec.reserve(N);
            vec.push_back(ExpectationContext<0>::expect(indice[0] == 0, ""));

            for (size_t i = 1; i < N; ++i)
            {
                vec.push_back(
                    std::visit(
                        [&](const auto& x) -> std::variant<ExpectationContext<INDICE + 1>...>
                        {
                            if constexpr (std::is_same_v<std::decay_t<decltype(x)>, ExpectationContext<N>>)
                            {
                                throw "Unreachable: ExpectationContext<N> encountered in ExpectationContextSequence "
                                      "generation";
                            }
                            else
                            {
                                return x.expect(indice[i] == 0, "");
                            }
                        },
                        vec[i - 1]
                    )
                );
            }

            return { vec[INDICE]... };
        }

        Range indice;
    };

    template <std::ranges::input_range Range>
    ExpectationContextSequence(Range) -> ExpectationContextSequence<Range>;

    /**
     * @brief Generates combinations of two ExpectationContext values for each index.
     *
     * @details Each pair selected from same ExpectationContext sequence.
     *
     * @tparam ExpectationContext
     * @tparam Ns
     */
    template <template <size_t> typename ExpectationContext, size_t... Ns>
    struct ExpectationContextDoubleValueCombination
    {
        /**
         * @brief The size of index range.
         *
         * @return constexpr size_t
         */
        static constexpr size_t size()
        {
            return COMBINATION_INDEX_LIMIT;
        }

        /**
         * @brief The first value of ExpectationContext pair for the given index.
         *
         * @tparam IDX
         * @return constexpr ExpectationContext
         */
        template <size_t IDX>
        constexpr auto a() const
        {
            return std::get<ExpectationContext<INDICE[IDX][0] + 1>>(SEQUENCE[INDICE[IDX][0]]);
        }

        /**
         * @brief The second value of ExpectationContext pair for the given index.
         *
         * @tparam IDX
         * @return constexpr ExpectationContext
         */
        template <size_t IDX>
        constexpr auto b() const
        {
            return std::get<ExpectationContext<INDICE[IDX][1] + 1>>(SEQUENCE[INDICE[IDX][1]]);
        }

    private:
        static constexpr size_t COMBINATION_INDEX_LIMIT = sizeof...(Ns);
        static constexpr size_t OP_VARIETY_LIMIT = 2;

        static constexpr auto INDICE = std::array{
            (Util::MultiIndex<Ns, COMBINATION_INDEX_LIMIT, COMBINATION_INDEX_LIMIT, OP_VARIETY_LIMIT>{}.value())...
        };
        static constexpr auto SEQUENCE = ExpectationContextSequence{
            INDICE
            | std::views::transform(
                [](const auto& x)
                {
                    return x[2];
                }
            )
        }.template value<ExpectationContext, COMBINATION_INDEX_LIMIT>();
    };
};
