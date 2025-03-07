/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

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
        template <typename> typename MemberFunctionWrap, template <typename> typename FunctionObjectWrap,
        typename Ret, typename... Args>
        requires CallableWrap<decltype([](Args...) -> Ret
              {
              })>::value
              && FunctionObject<decltype([](Args...) -> Ret
              {
              })>::value
              && (!FreeFunction<decltype([](Args...) -> Ret
              {
              })>::value)
              && (!MemberFunction<decltype([](Args...) -> Ret
              {
              })>::value)
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
        requires Callable<Ret(Args...)>::value && Callable<Ret (*)(Args...)>::value
              && Callable<Ret (*&)(Args...)>::value && Callable<Ret (*&&)(Args...)>::value
              && Callable<Ret (&)(Args...)>::value && Callable<Ret (&&)(Args...)>::value
              && FreeFunction<Ret(Args...)>::value && FreeFunction<Ret (*)(Args...)>::value
              && FreeFunction<Ret (*&)(Args...)>::value && FreeFunction<Ret (*&&)(Args...)>::value
              && FreeFunction<Ret (&)(Args...)>::value && FreeFunction<Ret (&&)(Args...)>::value
              && (!MemberFunction<Ret(Args...)>::value) && (!MemberFunction<Ret (*)(Args...)>::value)
              && (!MemberFunction<Ret (*&)(Args...)>::value) && (!MemberFunction<Ret (*&&)(Args...)>::value)
              && (!MemberFunction<Ret (&)(Args...)>::value) && (!MemberFunction<Ret (&&)(Args...)>::value)
              && (!FunctionObject<Ret(Args...)>::value) && (!FunctionObject<Ret (*)(Args...)>::value)
              && (!FunctionObject<Ret (*&)(Args...)>::value) && (!FunctionObject<Ret (*&&)(Args...)>::value)
              && (!FunctionObject<Ret (&)(Args...)>::value) && (!FunctionObject<Ret (&&)(Args...)>::value)
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
        requires Callable<Ret (TestStruct::*)(Args...)>::value && Callable<Ret (TestStruct::*&)(Args...)>::value
              && Callable<Ret (TestStruct::*&&)(Args...)>::value && Callable<Ret (TestStruct::*)(Args...) const>::value
              && Callable<Ret (TestStruct::*&)(Args...) const>::value
              && Callable<Ret (TestStruct::*&&)(Args...) const>::value
              && Callable<Ret (TestStruct::*)(Args...) volatile>::value
              && Callable<Ret (TestStruct::*&)(Args...) volatile>::value
              && Callable<Ret (TestStruct::*&&)(Args...) volatile>::value
              && Callable<Ret (TestStruct::*)(Args...) const volatile>::value
              && Callable<Ret (TestStruct::*&)(Args...) const volatile>::value
              && Callable<Ret (TestStruct::*&&)(Args...) const volatile>::value
              && Callable<Ret (TestStruct::*)(Args...) &>::value && Callable<Ret (TestStruct::*&)(Args...) &>::value
              && Callable<Ret (TestStruct::*&&)(Args...) &>::value
              && Callable<Ret (TestStruct::*)(Args...) const&>::value
              && Callable<Ret (TestStruct::*&)(Args...) const&>::value
              && Callable<Ret (TestStruct::*&&)(Args...) const&>::value
              && Callable<Ret (TestStruct::*)(Args...) volatile&>::value
              && Callable<Ret (TestStruct::*&)(Args...) volatile&>::value
              && Callable<Ret (TestStruct::*&&)(Args...) volatile&>::value
              && Callable<Ret (TestStruct::*)(Args...) const volatile&>::value
              && Callable<Ret (TestStruct::*&)(Args...) const volatile&>::value
              && Callable<Ret (TestStruct::*&&)(Args...) const volatile&>::value
              && Callable<Ret (TestStruct::*)(Args...) &&>::value && Callable<Ret (TestStruct::*&)(Args...) &&>::value
              && Callable<Ret (TestStruct::*&&)(Args...) &&>::value
              && Callable<Ret (TestStruct::*)(Args...) const&&>::value
              && Callable<Ret (TestStruct::*&)(Args...) const&&>::value
              && Callable<Ret (TestStruct::*&&)(Args...) const&&>::value
              && Callable<Ret (TestStruct::*)(Args...) volatile&&>::value
              && Callable<Ret (TestStruct::*&)(Args...) volatile&&>::value
              && Callable<Ret (TestStruct::*&&)(Args...) volatile&&>::value
              && Callable<Ret (TestStruct::*)(Args...) const volatile&&>::value
              && Callable<Ret (TestStruct::*&)(Args...) const volatile&&>::value
              && Callable<Ret (TestStruct::*&&)(Args...) const volatile&&>::value
              && MemberFunction<Ret (TestStruct::*)(Args...)>::value
              && MemberFunction<Ret (TestStruct::*&)(Args...)>::value
              && MemberFunction<Ret (TestStruct::*&&)(Args...)>::value
              && MemberFunction<Ret (TestStruct::*)(Args...) const>::value
              && MemberFunction<Ret (TestStruct::*&)(Args...) const>::value
              && MemberFunction<Ret (TestStruct::*&&)(Args...) const>::value
              && MemberFunction<Ret (TestStruct::*)(Args...) volatile>::value
              && MemberFunction<Ret (TestStruct::*&)(Args...) volatile>::value
              && MemberFunction<Ret (TestStruct::*&&)(Args...) volatile>::value
              && MemberFunction<Ret (TestStruct::*)(Args...) const volatile>::value
              && MemberFunction<Ret (TestStruct::*&)(Args...) const volatile>::value
              && MemberFunction<Ret (TestStruct::*&&)(Args...) const volatile>::value
              && MemberFunction<Ret (TestStruct::*)(Args...) &>::value
              && MemberFunction<Ret (TestStruct::*&)(Args...) &>::value
              && MemberFunction<Ret (TestStruct::*&&)(Args...) &>::value
              && MemberFunction<Ret (TestStruct::*)(Args...) const&>::value
              && MemberFunction<Ret (TestStruct::*&)(Args...) const&>::value
              && MemberFunction<Ret (TestStruct::*&&)(Args...) const&>::value
              && MemberFunction<Ret (TestStruct::*)(Args...) volatile&>::value
              && MemberFunction<Ret (TestStruct::*&)(Args...) volatile&>::value
              && MemberFunction<Ret (TestStruct::*&&)(Args...) volatile&>::value
              && MemberFunction<Ret (TestStruct::*)(Args...) const volatile&>::value
              && MemberFunction<Ret (TestStruct::*&)(Args...) const volatile&>::value
              && MemberFunction<Ret (TestStruct::*&&)(Args...) const volatile&>::value
              && MemberFunction<Ret (TestStruct::*)(Args...) &&>::value
              && MemberFunction<Ret (TestStruct::*&)(Args...) &&>::value
              && MemberFunction<Ret (TestStruct::*&&)(Args...) &&>::value
              && MemberFunction<Ret (TestStruct::*)(Args...) const&&>::value
              && MemberFunction<Ret (TestStruct::*&)(Args...) const&&>::value
              && MemberFunction<Ret (TestStruct::*&&)(Args...) const&&>::value
              && MemberFunction<Ret (TestStruct::*)(Args...) volatile&&>::value
              && MemberFunction<Ret (TestStruct::*&)(Args...) volatile&&>::value
              && MemberFunction<Ret (TestStruct::*&&)(Args...) volatile&&>::value
              && MemberFunction<Ret (TestStruct::*)(Args...) const volatile&&>::value
              && MemberFunction<Ret (TestStruct::*&)(Args...) const volatile&&>::value
              && MemberFunction<Ret (TestStruct::*&&)(Args...) const volatile&&>::value
              && (!FreeFunction<Ret (TestStruct::*)(Args...)>::value)
              && (!FreeFunction<Ret (TestStruct::*&)(Args...)>::value)
              && (!FreeFunction<Ret (TestStruct::* &&)(Args...)>::value)
              && (!FreeFunction<Ret (TestStruct::*)(Args...) const>::value)
              && (!FreeFunction<Ret (TestStruct::*&)(Args...) const>::value)
              && (!FreeFunction<Ret (TestStruct::* &&)(Args...) const>::value)
              && (!FreeFunction<Ret (TestStruct::*)(Args...) volatile>::value)
              && (!FreeFunction<Ret (TestStruct::*&)(Args...) volatile>::value)
              && (!FreeFunction<Ret (TestStruct::* &&)(Args...) volatile>::value)
              && (!FreeFunction<Ret (TestStruct::*)(Args...) const volatile>::value)
              && (!FreeFunction<Ret (TestStruct::*&)(Args...) const volatile>::value)
              && (!FreeFunction<Ret (TestStruct::* &&)(Args...) const volatile>::value)
              && (!FreeFunction<Ret (TestStruct::*)(Args...) &>::value)
              && (!FreeFunction<Ret (TestStruct::*&)(Args...) &>::value)
              && (!FreeFunction<Ret (TestStruct::* &&)(Args...) &>::value)
              && (!FreeFunction<Ret (TestStruct::*)(Args...) const&>::value)
              && (!FreeFunction<Ret (TestStruct::*&)(Args...) const&>::value)
              && (!FreeFunction<Ret (TestStruct::* &&)(Args...) const&>::value)
              && (!FreeFunction<Ret (TestStruct::*)(Args...) volatile&>::value)
              && (!FreeFunction<Ret (TestStruct::*&)(Args...) volatile&>::value)
              && (!FreeFunction<Ret (TestStruct::* &&)(Args...) volatile&>::value)
              && (!FreeFunction<Ret (TestStruct::*)(Args...) const volatile&>::value)
              && (!FreeFunction<Ret (TestStruct::*&)(Args...) const volatile&>::value)
              && (!FreeFunction<Ret (TestStruct::* &&)(Args...) const volatile&>::value)
              && (!FreeFunction<Ret (TestStruct::*)(Args...) &&>::value)
              && (!FreeFunction<Ret (TestStruct::*&)(Args...) &&>::value)
              && (!FreeFunction<Ret (TestStruct::* &&)(Args...) &&>::value)
              && (!FreeFunction<Ret (TestStruct::*)(Args...) const &&>::value)
              && (!FreeFunction<Ret (TestStruct::*&)(Args...) const &&>::value)
              && (!FreeFunction<Ret (TestStruct::* &&)(Args...) const &&>::value)
              && (!FreeFunction<Ret (TestStruct::*)(Args...) volatile &&>::value)
              && (!FreeFunction<Ret (TestStruct::*&)(Args...) volatile &&>::value)
              && (!FreeFunction<Ret (TestStruct::* &&)(Args...) volatile &&>::value)
              && (!FreeFunction<Ret (TestStruct::*)(Args...) const volatile &&>::value)
              && (!FreeFunction<Ret (TestStruct::*&)(Args...) const volatile &&>::value)
              && (!FreeFunction<Ret (TestStruct::* &&)(Args...) const volatile &&>::value)
              && (!FunctionObject<Ret (TestStruct::*)(Args...)>::value)
              && (!FunctionObject<Ret (TestStruct::*&)(Args...)>::value)
              && (!FunctionObject<Ret (TestStruct::* &&)(Args...)>::value)
              && (!FunctionObject<Ret (TestStruct::*)(Args...) const>::value)
              && (!FunctionObject<Ret (TestStruct::*&)(Args...) const>::value)
              && (!FunctionObject<Ret (TestStruct::* &&)(Args...) const>::value)
              && (!FunctionObject<Ret (TestStruct::*)(Args...) volatile>::value)
              && (!FunctionObject<Ret (TestStruct::*&)(Args...) volatile>::value)
              && (!FunctionObject<Ret (TestStruct::* &&)(Args...) volatile>::value)
              && (!FunctionObject<Ret (TestStruct::*)(Args...) const volatile>::value)
              && (!FunctionObject<Ret (TestStruct::*&)(Args...) const volatile>::value)
              && (!FunctionObject<Ret (TestStruct::* &&)(Args...) const volatile>::value)
              && (!FunctionObject<Ret (TestStruct::*)(Args...) &>::value)
              && (!FunctionObject<Ret (TestStruct::*&)(Args...) &>::value)
              && (!FunctionObject<Ret (TestStruct::* &&)(Args...) &>::value)
              && (!FunctionObject<Ret (TestStruct::*)(Args...) const&>::value)
              && (!FunctionObject<Ret (TestStruct::*&)(Args...) const&>::value)
              && (!FunctionObject<Ret (TestStruct::* &&)(Args...) const&>::value)
              && (!FunctionObject<Ret (TestStruct::*)(Args...) volatile&>::value)
              && (!FunctionObject<Ret (TestStruct::*&)(Args...) volatile&>::value)
              && (!FunctionObject<Ret (TestStruct::* &&)(Args...) volatile&>::value)
              && (!FunctionObject<Ret (TestStruct::*)(Args...) const volatile&>::value)
              && (!FunctionObject<Ret (TestStruct::*&)(Args...) const volatile&>::value)
              && (!FunctionObject<Ret (TestStruct::* &&)(Args...) const volatile&>::value)
              && (!FunctionObject<Ret (TestStruct::*)(Args...) &&>::value)
              && (!FunctionObject<Ret (TestStruct::*&)(Args...) &&>::value)
              && (!FunctionObject<Ret (TestStruct::* &&)(Args...) &&>::value)
              && (!FunctionObject<Ret (TestStruct::*)(Args...) const &&>::value)
              && (!FunctionObject<Ret (TestStruct::*&)(Args...) const &&>::value)
              && (!FunctionObject<Ret (TestStruct::* &&)(Args...) const &&>::value)
              && (!FunctionObject<Ret (TestStruct::*)(Args...) volatile &&>::value)
              && (!FunctionObject<Ret (TestStruct::*&)(Args...) volatile &&>::value)
              && (!FunctionObject<Ret (TestStruct::* &&)(Args...) volatile &&>::value)
              && (!FunctionObject<Ret (TestStruct::*)(Args...) const volatile &&>::value)
              && (!FunctionObject<Ret (TestStruct::*&)(Args...) const volatile &&>::value)
              && (!FunctionObject<Ret (TestStruct::* &&)(Args...) const volatile &&>::value)
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
                  typename CallableConceptEvals::LambdaEval<
                      CallableWrap, FreeFunctionWrap, MemberFunctionWrap, FunctionObjectWrap>::type,
                  TestTypes>);
    static_assert(TypeAssertionValue<
                  typename CallableConceptEvals::FreeEval<
                      CallableWrap, FreeFunctionWrap, MemberFunctionWrap, FunctionObjectWrap>::type,
                  TestTypes>);
    static_assert(TypeAssertionValue<
                  typename CallableConceptEvals::MemberEval<
                      CallableWrap, FreeFunctionWrap, MemberFunctionWrap, FunctionObjectWrap>::type,
                  TestTypes>);
}
