/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <array>
#include <ranges>
#include <tuple>

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
            return generate<ExpectationContext>(std::tuple<>{}, std::make_index_sequence<N>{});
        }

    private:
        template <template <size_t> typename ExpectationContext, typename Tuple, size_t CURRENT, size_t... REST>
        consteval auto generate(const Tuple tup, std::index_sequence<CURRENT, REST...>) const
        {
            if constexpr (sizeof...(REST) == 0)
            {
                return catNext(tup);
            }
            else if constexpr (CURRENT == 0)
            {
                return generate<ExpectationContext>(
                    std::tuple{
                        ExpectationContext<0>::expect(indice[0] == 0, ""),
                    },
                    std::index_sequence<REST...>{}
                );
            }
            else
            {
                return generate<ExpectationContext>(catNext(tup), std::index_sequence<REST...>{});
            }
        }

        template <template <size_t> typename ExpectationContext, size_t... INDICE>
        consteval std::tuple<ExpectationContext<INDICE>..., ExpectationContext<sizeof...(INDICE) + 1>>
        catNext(const std::tuple<ExpectationContext<INDICE>...> tup) const
        {
            return std::tuple_cat(
                tup, std::tuple{
                         std::get<sizeof...(INDICE) - 1>(tup).expect(indice[sizeof...(INDICE)] == 0, ""),
                     }
            );
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
            return std::get<INDICE[IDX][0]>(SEQUENCE);
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
            return std::get<INDICE[IDX][1]>(SEQUENCE);
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
