/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <array>
#include <ranges>
#include <variant>

#include "generators/multi-index.hpp"
#include "generators/sequence-from-indice.hpp"

namespace Expandable
{
    template <std::ranges::input_range Range>
    struct ExpectationContextSequence
    {
        consteval ExpectationContextSequence(Range indice) :
            ExpectationContextSequence{
                Util::SequenceFromIndice<Range>{
                    indice,
                },
            }
        {
        }

        template <template <size_t> typename ExpectationContext, size_t N>
        consteval auto value() const
        {
            return generate<ExpectationContext>(std::make_index_sequence<N>{});
        }

    private:
        consteval ExpectationContextSequence(Util::SequenceFromIndice<Range> sequence) :
            sequence{
                sequence,
            }
        {
        }

        template <template <size_t> typename ExpectationContext, size_t... INDICE>
        consteval std::array<std::variant<ExpectationContext<INDICE + 1>...>, sizeof...(INDICE)>
        generate(std::index_sequence<INDICE...>) const
        {
            return sequence.template value<std::variant<ExpectationContext<INDICE + 1>...>, sizeof...(INDICE)>(
                [](const size_t idx)
                {
                    return ExpectationContext<0>::expect(idx == 0, "");
                },
                []<typename T>(const T& x, const size_t idx)
                {
                    return std::visit(
                        [&idx](const auto& x) -> T
                        {
                            if constexpr (std::is_same_v<
                                              std::decay_t<decltype(x)>, ExpectationContext<sizeof...(INDICE)>>)
                            {
                                throw "Unreachable: ExpectationContext<N> encountered in ExpectationContextSequence "
                                      "generation";
                            }
                            else
                            {
                                return x.expect(idx == 0, "");
                            }
                        },
                        x
                    );
                }
            );
        }

        Util::SequenceFromIndice<Range> sequence;
    };

    template <std::ranges::input_range Range>
    ExpectationContextSequence(Range) -> ExpectationContextSequence<Range>;

    /**
     * @brief Generates combinations of three ExpectationContext values for each index.
     *
     * @details Each triplet selected from same ExpectationContext sequence.
     *
     * @tparam ExpectationContext
     * @tparam Ns
     */
    template <template <size_t> typename ExpectationContext, size_t... Ns>
    struct ExpectationContextTripleValueCombination
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
         * @brief The first value of ExpectationContext triplet for the given index.
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
         * @brief The second value of ExpectationContext triplet for the given index.
         *
         * @tparam IDX
         * @return constexpr ExpectationContext
         */
        template <size_t IDX>
        constexpr auto b() const
        {
            return std::get<ExpectationContext<INDICE[IDX][1] + 1>>(SEQUENCE[INDICE[IDX][1]]);
        }

        /**
         * @brief The third value of ExpectationContext triplet for the given index.
         *
         * @tparam IDX
         * @return constexpr ExpectationContext
         */
        template <size_t IDX>
        constexpr auto c() const
        {
            return std::get<ExpectationContext<INDICE[IDX][2] + 1>>(SEQUENCE[INDICE[IDX][2]]);
        }

    private:
        static constexpr size_t COMBINATION_INDEX_LIMIT = sizeof...(Ns);
        static constexpr size_t OP_VARIETY_LIMIT = 2;

        static constexpr auto INDICE = std::array{
            (Util::MultiIndex<
                 Ns, COMBINATION_INDEX_LIMIT, COMBINATION_INDEX_LIMIT, COMBINATION_INDEX_LIMIT, OP_VARIETY_LIMIT>{}
                 .value())...
        };
        static constexpr auto SEQUENCE = ExpectationContextSequence{
            INDICE
            | std::views::transform(
                [](const auto& x)
                {
                    return x[3];
                }
            )
        }.template value<ExpectationContext, COMBINATION_INDEX_LIMIT>();
    };
};
