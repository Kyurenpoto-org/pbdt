/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <type_traits>

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
struct CallableTarget : std::false_type
{
};

template <typename T>
    requires pbdt::bdd::CallableTarget<T>
struct CallableTarget<T> : std::true_type
{
};

template <typename>
struct CallableProperty : std::false_type
{
};

template <typename T>
    requires pbdt::bdd::CallableProperty<T>
struct CallableProperty<T> : std::true_type
{
};

template <typename>
struct RangeDomain : std::false_type
{
};

template <typename T>
    requires pbdt::bdd::RangeDomain<T>
struct RangeDomain<T> : std::true_type
{
};
