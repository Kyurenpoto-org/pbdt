/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#ifndef EXSTD_MODULE

#include <array>
#include <tuple>
#include <type_traits>

#endif

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

    namespace detail
    {
        template <typename>
        struct ArrayFormImpl : std::false_type
        {
        };

        template <typename T, size_t N>
        struct ArrayFormImpl<std::array<T, N>> : std::true_type
        {
        };
    }

    template <typename T>
    concept ArrayForm = detail::ArrayFormImpl<std::remove_cvref_t<T>>::value;
}
