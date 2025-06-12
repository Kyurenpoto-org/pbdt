/*
 * SPDX - FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#ifndef EXSTD_MODULE

#include <tuple>
#include <type_traits>
#include <variant>

#endif

#include "exstd/callable-traits.hpp"
#include "exstd/type-container.hpp"
#include "exstd/type-traits.hpp"

namespace exstd
{
    template <typename Func, typename Arg>
    constexpr auto applyOrInvoke(Func&& func, Arg&& arg)
    {
        if constexpr (TupleForm<Arg>)
        {
            return std::apply(std::forward<Func>(func), std::forward<Arg>(arg));
        }
        else
        {
            return std::invoke(std::forward<Func>(func), std::forward<Arg>(arg));
        }
    }

    template <typename FuncsTuple, typename ArgsTuple>
    constexpr auto compositeApply(FuncsTuple&& funcs, ArgsTuple&& args)
    {
        constexpr size_t N = std::tuple_size_v<std::remove_cvref_t<FuncsTuple>>;

        if constexpr (N == 0)
        {
            return args;
        }
        else
        {
            using ReturnsType = ReplacedContainerType<
                TypeIndexedContainerType<TypeMappedContainerType<FuncsTuple, CallableReturnType>>, std::variant>;

            ReturnsType result = std::variant_alternative_t<0, ReturnsType>{
                .value = std::apply(std::get<0>(funcs), args),
            };

            while (std::visit(
                       [](auto result)
                       {
                           return result.idx();
                       },
                       result
                   ) + 1
                   < N)
            {
                result = std::visit(
                    [&funcs](auto result)
                    {
                        constexpr size_t Idx = result.idx();

                        if constexpr (Idx + 1 < N)
                        {
                            return std::variant_alternative_t<Idx + 1, ReturnsType>{
                                .value = exstd::applyOrInvoke(std::get<Idx + 1>(funcs), result.value)
                            };
                        }
                        else
                        {
                            return result;
                        }
                    },
                    result
                );
            }

            return std::get<N - 1>(result).value;
        }
    }
}
