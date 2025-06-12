/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#ifndef EXSTD_MODULE

#include <tuple>
#include <type_traits>

#endif

namespace exstd
{
    namespace detail
    {
        template <size_t Idx, typename T>
        struct IndexedType
        {
            consteval size_t idx() const
            {
                return Idx;
            }

            T value;
        };

        template <typename, typename>
        struct TypeIndexedContainerImpl;

        template <size_t... Idxs, template <typename...> typename Container, typename... Ts>
        struct TypeIndexedContainerImpl<std::index_sequence<Idxs...>, Container<Ts...>>
        {
            using type = Container<IndexedType<Idxs, std::tuple_element_t<Idxs, std::tuple<Ts...>>>...>;
        };

        template <typename>
        struct TypeIndexedContainer;

        template <template <typename...> typename Container, typename... Ts>
        struct TypeIndexedContainer<Container<Ts...>>
        {
            using type =
                typename TypeIndexedContainerImpl<std::make_index_sequence<sizeof...(Ts)>, Container<Ts...>>::type;
        };
    }

    template <typename T>
    using TypeIndexedContainerType = typename detail::TypeIndexedContainer<std::remove_cvref_t<T>>::type;

    namespace detail
    {
        template <typename, template <typename> typename>
        struct TypeMappedContainer;

        template <template <typename...> typename Container, template <typename> typename TypeMap, typename... Ts>
        struct TypeMappedContainer<Container<Ts...>, TypeMap>
        {
            using type = Container<TypeMap<Ts>...>;
        };
    }

    template <typename T, template <typename> typename TypeMap>
    using TypeMappedContainerType = typename detail::TypeMappedContainer<std::remove_cvref_t<T>, TypeMap>::type;

    namespace detail
    {
        template <typename, template <typename...> typename>
        struct ReplacedContainer;

        template <template <typename...> typename From, template <typename...> typename To, typename... Ts>
        struct ReplacedContainer<From<Ts...>, To>
        {
            using type = To<Ts...>;
        };
    }

    template <typename From, template <typename...> typename To>
    using ReplacedContainerType = typename detail::ReplacedContainer<std::remove_cvref_t<From>, To>::type;
}
