/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <type_traits>

namespace
{
    template<typename Ret, typename... Args>
    using LambdaType = decltype([](Args...) -> Ret
    {
        if constexpr (std::is_void_v<Ret>)
        {
            return;
        }
        else
        {
            return Ret{};
        }
    });
}
