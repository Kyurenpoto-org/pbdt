/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include "lambda-traits.hpp"
#include "type-assertion.hpp"

namespace CallableConceptEvals
{
    template <
        template <typename> typename, template <typename> typename, template <typename> typename,
        template <typename> typename, typename>
    struct LambdaEvalImpl : std::false_type
    {
    };

    template <
        template <typename> typename CallableWrap, template <typename> typename FreeFunctionWrap,
        template <typename> typename MemberFunctionWrap, template <typename> typename FunctionObjectWrap, typename Ret,
        typename... Args>
        requires CallableWrap<LambdaType<Ret, Args...>>::value && FunctionObjectWrap<LambdaType<Ret, Args...>>::value
              && (!FreeFunctionWrap<LambdaType<Ret, Args...>>::value)
              && (!MemberFunctionWrap<LambdaType<Ret, Args...>>::value)
    struct LambdaEvalImpl<CallableWrap, FreeFunctionWrap, MemberFunctionWrap, FunctionObjectWrap, Ret(Args...)> :
        std::true_type
    {
    };

    template <
        template <typename> typename CallableWrap, template <typename> typename FreeFunctionWrap,
        template <typename> typename MemberFunctionWrap, template <typename> typename FunctionObjectWrap>
    struct LambdaEval
    {
        template <typename T>
        using type = LambdaEvalImpl<CallableWrap, FreeFunctionWrap, MemberFunctionWrap, FunctionObjectWrap, T>;
    };

    template <
        template <typename> typename, template <typename> typename, template <typename> typename,
        template <typename> typename, typename>
    struct FreeEvalImpl : std::false_type
    {
    };

    template <
        template <typename> typename CallableWrap, template <typename> typename FreeFunctionWrap,
        template <typename> typename MemberFunctionWrap, template <typename> typename FunctionObjectWrap, typename Ret,
        typename... Args>
        requires CallableWrap<Ret(Args...)>::value && CallableWrap<Ret (*)(Args...)>::value
              && CallableWrap<Ret (*&)(Args...)>::value && CallableWrap<Ret (*&&)(Args...)>::value
              && CallableWrap<Ret (&)(Args...)>::value && CallableWrap<Ret (&&)(Args...)>::value
              && FreeFunctionWrap<Ret(Args...)>::value && FreeFunctionWrap<Ret (*)(Args...)>::value
              && FreeFunctionWrap<Ret (*&)(Args...)>::value && FreeFunctionWrap<Ret (*&&)(Args...)>::value
              && FreeFunctionWrap<Ret (&)(Args...)>::value && FreeFunctionWrap<Ret (&&)(Args...)>::value
              && (!MemberFunctionWrap<Ret(Args...)>::value) && (!MemberFunctionWrap<Ret (*)(Args...)>::value)
              && (!MemberFunctionWrap<Ret (*&)(Args...)>::value) && (!MemberFunctionWrap<Ret (*&&)(Args...)>::value)
              && (!MemberFunctionWrap<Ret (&)(Args...)>::value) && (!MemberFunctionWrap<Ret (&&)(Args...)>::value)
              && (!FunctionObjectWrap<Ret(Args...)>::value) && (!FunctionObjectWrap<Ret (*)(Args...)>::value)
              && (!FunctionObjectWrap<Ret (*&)(Args...)>::value) && (!FunctionObjectWrap<Ret (*&&)(Args...)>::value)
              && (!FunctionObjectWrap<Ret (&)(Args...)>::value) && (!FunctionObjectWrap<Ret (&&)(Args...)>::value)
    struct FreeEvalImpl<CallableWrap, FreeFunctionWrap, MemberFunctionWrap, FunctionObjectWrap, Ret(Args...)> :
        std::true_type
    {
    };

    template <
        template <typename> typename CallableWrap, template <typename> typename FreeFunctionWrap,
        template <typename> typename MemberFunctionWrap, template <typename> typename FunctionObjectWrap>
    struct FreeEval
    {
        template <typename T>
        using type = FreeEvalImpl<CallableWrap, FreeFunctionWrap, MemberFunctionWrap, FunctionObjectWrap, T>;
    };

    template <
        template <typename> typename, template <typename> typename, template <typename> typename,
        template <typename> typename, typename>
    struct MemberEvalImpl : std::false_type
    {
    };

    struct TestStruct;

    template <
        template <typename> typename CallableWrap, template <typename> typename FreeFunctionWrap,
        template <typename> typename MemberFunctionWrap, template <typename> typename FunctionObjectWrap, typename Ret,
        typename... Args>
        requires CallableWrap<Ret (TestStruct::*)(Args...)>::value && CallableWrap<Ret (TestStruct::*&)(Args...)>::value
              && CallableWrap<Ret (TestStruct::*&&)(Args...)>::value
              && CallableWrap<Ret (TestStruct::*)(Args...) const>::value
              && CallableWrap<Ret (TestStruct::*&)(Args...) const>::value
              && CallableWrap<Ret (TestStruct::*&&)(Args...) const>::value
              && CallableWrap<Ret (TestStruct::*)(Args...) volatile>::value
              && CallableWrap<Ret (TestStruct::*&)(Args...) volatile>::value
              && CallableWrap<Ret (TestStruct::*&&)(Args...) volatile>::value
              && CallableWrap<Ret (TestStruct::*)(Args...) const volatile>::value
              && CallableWrap<Ret (TestStruct::*&)(Args...) const volatile>::value
              && CallableWrap<Ret (TestStruct::*&&)(Args...) const volatile>::value
              && CallableWrap<Ret (TestStruct::*)(Args...) &>::value
              && CallableWrap<Ret (TestStruct::*&)(Args...) &>::value
              && CallableWrap<Ret (TestStruct::*&&)(Args...) &>::value
              && CallableWrap<Ret (TestStruct::*)(Args...) const&>::value
              && CallableWrap<Ret (TestStruct::*&)(Args...) const&>::value
              && CallableWrap<Ret (TestStruct::*&&)(Args...) const&>::value
              && CallableWrap<Ret (TestStruct::*)(Args...) volatile&>::value
              && CallableWrap<Ret (TestStruct::*&)(Args...) volatile&>::value
              && CallableWrap<Ret (TestStruct::*&&)(Args...) volatile&>::value
              && CallableWrap<Ret (TestStruct::*)(Args...) const volatile&>::value
              && CallableWrap<Ret (TestStruct::*&)(Args...) const volatile&>::value
              && CallableWrap<Ret (TestStruct::*&&)(Args...) const volatile&>::value
              && CallableWrap<Ret (TestStruct::*)(Args...) &&>::value
              && CallableWrap<Ret (TestStruct::*&)(Args...) &&>::value
              && CallableWrap<Ret (TestStruct::*&&)(Args...) &&>::value
              && CallableWrap<Ret (TestStruct::*)(Args...) const&&>::value
              && CallableWrap<Ret (TestStruct::*&)(Args...) const&&>::value
              && CallableWrap<Ret (TestStruct::*&&)(Args...) const&&>::value
              && CallableWrap<Ret (TestStruct::*)(Args...) volatile&&>::value
              && CallableWrap<Ret (TestStruct::*&)(Args...) volatile&&>::value
              && CallableWrap<Ret (TestStruct::*&&)(Args...) volatile&&>::value
              && CallableWrap<Ret (TestStruct::*)(Args...) const volatile&&>::value
              && CallableWrap<Ret (TestStruct::*&)(Args...) const volatile&&>::value
              && CallableWrap<Ret (TestStruct::*&&)(Args...) const volatile&&>::value
              && MemberFunctionWrap<Ret (TestStruct::*)(Args...)>::value
              && MemberFunctionWrap<Ret (TestStruct::*&)(Args...)>::value
              && MemberFunctionWrap<Ret (TestStruct::*&&)(Args...)>::value
              && MemberFunctionWrap<Ret (TestStruct::*)(Args...) const>::value
              && MemberFunctionWrap<Ret (TestStruct::*&)(Args...) const>::value
              && MemberFunctionWrap<Ret (TestStruct::*&&)(Args...) const>::value
              && MemberFunctionWrap<Ret (TestStruct::*)(Args...) volatile>::value
              && MemberFunctionWrap<Ret (TestStruct::*&)(Args...) volatile>::value
              && MemberFunctionWrap<Ret (TestStruct::*&&)(Args...) volatile>::value
              && MemberFunctionWrap<Ret (TestStruct::*)(Args...) const volatile>::value
              && MemberFunctionWrap<Ret (TestStruct::*&)(Args...) const volatile>::value
              && MemberFunctionWrap<Ret (TestStruct::*&&)(Args...) const volatile>::value
              && MemberFunctionWrap<Ret (TestStruct::*)(Args...) &>::value
              && MemberFunctionWrap<Ret (TestStruct::*&)(Args...) &>::value
              && MemberFunctionWrap<Ret (TestStruct::*&&)(Args...) &>::value
              && MemberFunctionWrap<Ret (TestStruct::*)(Args...) const&>::value
              && MemberFunctionWrap<Ret (TestStruct::*&)(Args...) const&>::value
              && MemberFunctionWrap<Ret (TestStruct::*&&)(Args...) const&>::value
              && MemberFunctionWrap<Ret (TestStruct::*)(Args...) volatile&>::value
              && MemberFunctionWrap<Ret (TestStruct::*&)(Args...) volatile&>::value
              && MemberFunctionWrap<Ret (TestStruct::*&&)(Args...) volatile&>::value
              && MemberFunctionWrap<Ret (TestStruct::*)(Args...) const volatile&>::value
              && MemberFunctionWrap<Ret (TestStruct::*&)(Args...) const volatile&>::value
              && MemberFunctionWrap<Ret (TestStruct::*&&)(Args...) const volatile&>::value
              && MemberFunctionWrap<Ret (TestStruct::*)(Args...) &&>::value
              && MemberFunctionWrap<Ret (TestStruct::*&)(Args...) &&>::value
              && MemberFunctionWrap<Ret (TestStruct::*&&)(Args...) &&>::value
              && MemberFunctionWrap<Ret (TestStruct::*)(Args...) const&&>::value
              && MemberFunctionWrap<Ret (TestStruct::*&)(Args...) const&&>::value
              && MemberFunctionWrap<Ret (TestStruct::*&&)(Args...) const&&>::value
              && MemberFunctionWrap<Ret (TestStruct::*)(Args...) volatile&&>::value
              && MemberFunctionWrap<Ret (TestStruct::*&)(Args...) volatile&&>::value
              && MemberFunctionWrap<Ret (TestStruct::*&&)(Args...) volatile&&>::value
              && MemberFunctionWrap<Ret (TestStruct::*)(Args...) const volatile&&>::value
              && MemberFunctionWrap<Ret (TestStruct::*&)(Args...) const volatile&&>::value
              && MemberFunctionWrap<Ret (TestStruct::*&&)(Args...) const volatile&&>::value
              && (!FreeFunctionWrap<Ret (TestStruct::*)(Args...)>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*&)(Args...)>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::* &&)(Args...)>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*)(Args...) const>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*&)(Args...) const>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::* &&)(Args...) const>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*)(Args...) volatile>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*&)(Args...) volatile>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::* &&)(Args...) volatile>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*)(Args...) const volatile>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*&)(Args...) const volatile>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::* &&)(Args...) const volatile>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*)(Args...) &>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*&)(Args...) &>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::* &&)(Args...) &>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*)(Args...) const&>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*&)(Args...) const&>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::* &&)(Args...) const&>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*)(Args...) volatile&>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*&)(Args...) volatile&>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::* &&)(Args...) volatile&>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*)(Args...) const volatile&>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*&)(Args...) const volatile&>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::* &&)(Args...) const volatile&>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*)(Args...) &&>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*&)(Args...) &&>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::* &&)(Args...) &&>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*)(Args...) const &&>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*&)(Args...) const &&>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::* &&)(Args...) const &&>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*)(Args...) volatile &&>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*&)(Args...) volatile &&>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::* &&)(Args...) volatile &&>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*)(Args...) const volatile &&>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::*&)(Args...) const volatile &&>::value)
              && (!FreeFunctionWrap<Ret (TestStruct::* &&)(Args...) const volatile &&>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*)(Args...)>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*&)(Args...)>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::* &&)(Args...)>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*)(Args...) const>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*&)(Args...) const>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::* &&)(Args...) const>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*)(Args...) volatile>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*&)(Args...) volatile>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::* &&)(Args...) volatile>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*)(Args...) const volatile>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*&)(Args...) const volatile>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::* &&)(Args...) const volatile>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*)(Args...) &>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*&)(Args...) &>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::* &&)(Args...) &>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*)(Args...) const&>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*&)(Args...) const&>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::* &&)(Args...) const&>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*)(Args...) volatile&>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*&)(Args...) volatile&>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::* &&)(Args...) volatile&>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*)(Args...) const volatile&>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*&)(Args...) const volatile&>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::* &&)(Args...) const volatile&>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*)(Args...) &&>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*&)(Args...) &&>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::* &&)(Args...) &&>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*)(Args...) const &&>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*&)(Args...) const &&>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::* &&)(Args...) const &&>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*)(Args...) volatile &&>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*&)(Args...) volatile &&>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::* &&)(Args...) volatile &&>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*)(Args...) const volatile &&>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::*&)(Args...) const volatile &&>::value)
              && (!FunctionObjectWrap<Ret (TestStruct::* &&)(Args...) const volatile &&>::value)
    struct MemberEvalImpl<CallableWrap, FreeFunctionWrap, MemberFunctionWrap, FunctionObjectWrap, Ret(Args...)> :
        std::true_type
    {
    };

    template <
        template <typename> typename CallableWrap, template <typename> typename FreeFunctionWrap,
        template <typename> typename MemberFunctionWrap, template <typename> typename FunctionObjectWrap>
    struct MemberEval
    {
        template <typename T>
        using type = MemberEvalImpl<CallableWrap, FreeFunctionWrap, MemberFunctionWrap, FunctionObjectWrap, T>;
    };
}

template <
    template <typename> typename CallableWrap, template <typename> typename FreeFunctionWrap,
    template <typename> typename MemberFunctionWrap, template <typename> typename FunctionObjectWrap>
void callableConcepts()
{
    using TestTypes = TypeList<
        void(), void(int), void(int, int), void(int, int, int), void(int, int, int, int), int(), int(int),
        int(int, int), int(int, int, int), int(int, int, int, int)>;

    static_assert(TypeAssertionValue<
                  CallableConceptEvals::LambdaEval<
                      CallableWrap, FreeFunctionWrap, MemberFunctionWrap, FunctionObjectWrap>::template type,
                  TestTypes>);
    static_assert(TypeAssertionValue<
                  CallableConceptEvals::FreeEval<
                      CallableWrap, FreeFunctionWrap, MemberFunctionWrap, FunctionObjectWrap>::template type,
                  TestTypes>);
    static_assert(TypeAssertionValue<
                  CallableConceptEvals::MemberEval<
                      CallableWrap, FreeFunctionWrap, MemberFunctionWrap, FunctionObjectWrap>::template type,
                  TestTypes>);
}
