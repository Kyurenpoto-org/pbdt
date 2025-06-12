/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "callable-concept-common.hpp"

import exstd.type_traits;

template <typename>
struct Callable : std::false_type
{
};

template <typename T>
    requires exstd::Callable<T>
struct Callable<T> : std::true_type
{
};

template <typename>
struct FreeFunction : std::false_type
{
};

template <typename T>
    requires exstd::FreeFunction<T>
struct FreeFunction<T> : std::true_type
{
};

template <typename>
struct MemberFunction : std::false_type
{
};

template <typename T>
    requires exstd::MemberFunction<T>
struct MemberFunction<T> : std::true_type
{
};

template <typename>
struct FunctionObject : std::false_type
{
};

template <typename T>
    requires exstd::FunctionObject<T>
struct FunctionObject<T> : std::true_type
{
};

int main()
{
    callableConcepts<Callable, FreeFunction, MemberFunction, FunctionObject>();

    return 0;
}
