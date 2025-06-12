/*
 * SPDX - FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#ifndef EXSTD_MODULE

#include <concepts>
#include <functional>

#endif

namespace exstd
{
    namespace detail
    {
        template <typename T, typename = void>
        struct FunctionObjectImpl : std::false_type
        {
        };
        template <typename T>
        struct FunctionObjectImpl<T, std::void_t<decltype(&T::operator())>> : std::true_type
        {
        };
    }

    template <typename T>
    concept FunctionObject = detail::FunctionObjectImpl<T>::value;

    namespace detail
    {
        template <typename>
        struct RemovePointerCallableImpl;

        template <typename Ret, typename... Args>
        struct RemovePointerCallableImpl<Ret(Args...)>
        {
            using type = Ret(Args...);
        };
        template <typename Ret, typename... Args>
        struct RemovePointerCallableImpl<Ret (&)(Args...)>
        {
            using type = Ret (&)(Args...);
        };
        template <typename Ret, typename... Args>
        struct RemovePointerCallableImpl<Ret (&&)(Args...)>
        {
            using type = Ret (&&)(Args...);
        };
        template <typename Ret, typename... Args>
        struct RemovePointerCallableImpl<Ret (*)(Args...)> : RemovePointerCallableImpl<Ret(Args...)>
        {
        };
        template <typename Ret, typename... Args>
        struct RemovePointerCallableImpl<Ret (*&)(Args...)> : RemovePointerCallableImpl<Ret (&)(Args...)>
        {
        };
        template <typename Ret, typename... Args>
        struct RemovePointerCallableImpl<Ret (*&&)(Args...)> : RemovePointerCallableImpl<Ret (&&)(Args...)>
        {
        };
    }

    template <typename T>
    using RemovePointerCallableType = typename detail::RemovePointerCallableImpl<T>::type;

    // RemoveReferenceCallable for free-functions
    namespace detail
    {
        template <typename>
        struct RemoveReferenceCallableImpl;

        template <typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret(Args...)>
        {
            using type = Ret(Args...);
        };
        template <typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (*)(Args...)>
        {
            using type = Ret (*)(Args...);
        };
        template <typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (*&)(Args...)> : RemoveReferenceCallableImpl<Ret (*)(Args...)>
        {
        };
        template <typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (*&&)(Args...)> : RemoveReferenceCallableImpl<Ret (*)(Args...)>
        {
        };
        template <typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (&)(Args...)> : RemoveReferenceCallableImpl<Ret(Args...)>
        {
        };
        template <typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (&&)(Args...)> : RemoveReferenceCallableImpl<Ret(Args...)>
        {
        };
    }

    // RemoveReferenceCallable for member-functions
    namespace detail
    {
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*)(Args...)>
        {
            using type = Ret (Class::*)(Args...);
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&)(Args...)> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&&)(Args...)> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...)>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*)(Args...) const>
        {
            using type = Ret (Class::*)(Args...) const;
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&)(Args...) const> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...) const>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&&)(Args...) const> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...) const>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*)(Args...) volatile>
        {
            using type = Ret (Class::*)(Args...) volatile;
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&)(Args...) volatile> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...) volatile>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&&)(Args...) volatile> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...) volatile>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*)(Args...) const volatile>
        {
            using type = Ret (Class::*)(Args...) const volatile;
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&)(Args...) const volatile> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...) const volatile>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&&)(Args...) const volatile> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...) const volatile>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*)(Args...) &>
        {
            using type = Ret (Class::*)(Args...) &;
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&)(Args...) &> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...) &>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&&)(Args...) &> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...) &>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*)(Args...) const&>
        {
            using type = Ret (Class::*)(Args...) const&;
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&)(Args...) const&> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...) const&>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&&)(Args...) const&> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...) const&>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*)(Args...) volatile&>
        {
            using type = Ret (Class::*)(Args...) volatile&;
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&)(Args...) volatile&> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...) volatile&>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&&)(Args...) volatile&> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...) volatile&>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*)(Args...) const volatile&>
        {
            using type = Ret (Class::*)(Args...) const volatile&;
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&)(Args...) const volatile&> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...) const volatile&>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&&)(Args...) const volatile&> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...) const volatile&>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*)(Args...) &&>
        {
            using type = Ret (Class::*)(Args...) &&;
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&)(Args...) &&> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...) &&>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&&)(Args...) &&> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...) &&>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*)(Args...) const&&>
        {
            using type = Ret (Class::*)(Args...) const&&;
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&)(Args...) const&&> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...) const&&>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&&)(Args...) const&&> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...) const&&>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*)(Args...) volatile&&>
        {
            using type = Ret (Class::*)(Args...) volatile&&;
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&)(Args...) volatile&&> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...) volatile&&>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&&)(Args...) volatile&&> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...) volatile&&>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*)(Args...) const volatile&&>
        {
            using type = Ret (Class::*)(Args...) const volatile&&;
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&)(Args...) const volatile&&> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...) const volatile&&>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveReferenceCallableImpl<Ret (Class::*&&)(Args...) const volatile&&> :
            RemoveReferenceCallableImpl<Ret (Class::*)(Args...) const volatile&&>
        {
        };
    }

    template <typename T>
    using RemoveReferenceCallableType = typename detail::RemoveReferenceCallableImpl<T>::type;

    template <typename T>
    using StandardFreeFunctionType = RemoveReferenceCallableType<RemovePointerCallableType<T>>;

    namespace detail
    {
        template <typename>
        struct RemoveCVRefMemberFunctionImpl;

        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...)>
        {
            using type = Ret (Class::*)(Args...);
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...)>
        {
            using type = Ret (Class::*&)(Args...);
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...)>
        {
            using type = Ret (Class::*&&)(Args...);
        };

        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...) const> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...) const> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...) const> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...)>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...) volatile> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...) volatile> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...) volatile> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...)>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...) const volatile> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...) const volatile> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...) const volatile> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...)>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...) &> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...) &> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...) &> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...)>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...) const&> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...) const&> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...) const&> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...)>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...) volatile&> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...) volatile&> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...) volatile&> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...)>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...) const volatile&> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...) const volatile&> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...) const volatile&> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...)>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...) &&> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...) &&> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...) &&> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...)>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...) const&&> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...) const&&> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...) const&&> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...)>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...) volatile&&> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...) volatile&&> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...) volatile&&> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...)>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...) const volatile&&> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...) const volatile&&> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*&)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...) const volatile&&> :
            RemoveCVRefMemberFunctionImpl<Ret (Class::*&&)(Args...)>
        {
        };
    }

    template <typename T>
    using RemoveCVRefMemberFunctionType = typename detail::RemoveCVRefMemberFunctionImpl<T>::type;

    template <typename T>
    using StandardMemberFunctionType = RemoveReferenceCallableType<RemoveCVRefMemberFunctionType<T>>;

    namespace detail
    {
        template <typename T>
        struct FreeFunctionImpl : std::false_type
        {
        };

        template <typename Ret, typename... Args>
        struct FreeFunctionImpl<Ret(Args...)> : std::true_type
        {
        };
        template <typename Ret, typename... Args>
        struct FreeFunctionImpl<Ret (*)(Args...)> : FreeFunctionImpl<Ret(Args...)>
        {
        };
        template <typename Ret, typename... Args>
        struct FreeFunctionImpl<Ret (*&)(Args...)> : FreeFunctionImpl<Ret(Args...)>
        {
        };
        template <typename Ret, typename... Args>
        struct FreeFunctionImpl<Ret (*&&)(Args...)> : FreeFunctionImpl<Ret(Args...)>
        {
        };
        template <typename Ret, typename... Args>
        struct FreeFunctionImpl<Ret (&)(Args...)> : FreeFunctionImpl<Ret(Args...)>
        {
        };
        template <typename Ret, typename... Args>
        struct FreeFunctionImpl<Ret (&&)(Args...)> : FreeFunctionImpl<Ret(Args...)>
        {
        };
    }

    template <typename T>
    concept FreeFunction = detail::FreeFunctionImpl<T>::value;

    namespace detail
    {
        template <typename T>
        struct MemberFunctionImpl : std::false_type
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*)(Args...)> : std::true_type
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&)(Args...)> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&&)(Args...)> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*)(Args...) const> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&)(Args...) const> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&&)(Args...) const> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*)(Args...) volatile> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&)(Args...) volatile> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&&)(Args...) volatile> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*)(Args...) const volatile> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&)(Args...) const volatile> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&&)(Args...) const volatile> :
            MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*)(Args...) &> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&)(Args...) &> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&&)(Args...) &> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*)(Args...) const&> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&)(Args...) const&> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&&)(Args...) const&> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*)(Args...) volatile&> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&)(Args...) volatile&> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&&)(Args...) volatile&> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*)(Args...) const volatile&> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&)(Args...) const volatile&> :
            MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&&)(Args...) const volatile&> :
            MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*)(Args...) &&> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&)(Args...) &&> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&&)(Args...) &&> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*)(Args...) const&&> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&)(Args...) const&&> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&&)(Args...) const&&> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*)(Args...) volatile&&> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&)(Args...) volatile&&> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&&)(Args...) volatile&&> : MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };

        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*)(Args...) const volatile&&> :
            MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&)(Args...) const volatile&&> :
            MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
        template <typename Class, typename Ret, typename... Args>
        struct MemberFunctionImpl<Ret (Class::*&&)(Args...) const volatile&&> :
            MemberFunctionImpl<Ret (Class::*)(Args...)>
        {
        };
    }

    template <typename T>
    concept MemberFunction = detail::MemberFunctionImpl<T>::value;

    template <typename T>
    concept Callable = FreeFunction<T> || MemberFunction<T> || FunctionObject<T>;

    namespace detail
    {
        template <typename>
        struct SimpleCallableImpl;

        template <typename T>
            requires FreeFunction<T>
        struct SimpleCallableImpl<T>
        {
            using type = StandardFreeFunctionType<T>;
        };

        template <typename T>
            requires MemberFunction<T>
        class SimpleCallableImpl<T>
        {
            template <typename U>
            struct StandardTransform;

            template <typename Class, typename Ret, typename... Args>
            struct StandardTransform<Ret (Class::*)(Args...)>
            {
                using type = Ret(Args...);
            };

        public:
            using type = typename StandardTransform<StandardMemberFunctionType<T>>::type;
        };

        template <typename T>
            requires FunctionObject<T>
        struct SimpleCallableImpl<T> : SimpleCallableImpl<decltype(&T::operator())>
        {
        };
    }

    template <typename T>
    using SimpleCallableType = typename detail::SimpleCallableImpl<T>::type;

    namespace detail
    {
        template <typename>
        struct CallableComponentImpl;

        template <typename T>
            requires Callable<T>
        class CallableComponentImpl<T>
        {
            template <typename>
            struct SimpleCallableComponent;

            template <typename Ret, typename... Args>
            struct SimpleCallableComponent<Ret(Args...)>
            {
                using ArgumentsType = std::tuple<Args...>;
                using ReturnType = Ret;
            };

        public:
            using ArgumentsType = typename SimpleCallableComponent<SimpleCallableType<T>>::ArgumentsType;
            using ReturnType = typename SimpleCallableComponent<SimpleCallableType<T>>::ReturnType;
        };
    }

    template <typename T>
    using CallableArgumentsType = typename detail::CallableComponentImpl<T>::ArgumentsType;

    template <typename T>
    using CallableReturnType = typename detail::CallableComponentImpl<T>::ReturnType;
}
