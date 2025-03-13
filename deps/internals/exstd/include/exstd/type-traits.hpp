/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <tuple>
#include <type_traits>

namespace exstd
{
    namespace detail
    {
        template <typename>
        struct TupleFormImpl : std::false_type
        {
        };

        template <typename... Ts>
        struct TupleFormImpl<std::tuple<Ts...>> : std::true_type
        {
        };
    }

    template <typename T>
    concept TupleForm = detail::TupleFormImpl<std::remove_cvref_t<T>>::value;
}
