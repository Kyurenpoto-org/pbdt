/*
 * SPDX - FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#ifndef PBDT_MODULE

#include "exstd/callable-traits.hpp"

#include "pbdt/test-context.hpp"

#endif

namespace pbdt::bdd
{
    /**
     * @brief A concept that checks if a type is a callable target.
     *
     * @tparam Target
     */
    template <typename Target>
    concept CallableTarget =
        (exstd::Callable<std::decay_t<Target>> && !std::is_void_v<exstd::CallableReturnType<std::decay_t<Target>>>
         && !std::is_same_v<exstd::CallableArgumentsType<std::decay_t<Target>>, std::tuple<>>);

    /**
     * @brief A concept that checks if a type is a callable property.
     *
     * @tparam Prop
     */
    template <typename Prop>
    concept CallableProperty =
        (exstd::Callable<std::decay_t<Prop>> && std::tuple_size_v<exstd::CallableArgumentsType<std::decay_t<Prop>>> == 2
         && std::is_same_v<exstd::CallableReturnType<std::decay_t<Prop>>, test_context::detail::TestContext>);

    /**
     * @brief A concept that checks if a type is a range domain.
     *
     * @tparam Domain
     */
    template <typename Domain>
    concept RangeDomain = std::ranges::range<std::decay_t<Domain>>;
}
