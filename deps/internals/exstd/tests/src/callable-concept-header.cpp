/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "exstd/callable-traits.hpp"

#include "type-assertion.hpp"

namespace CallableConceptEvals
{
    template <typename T>
    struct CallableConceptLambdaEval : std::false_type
    {
    };

    template <typename Ret, typename... Args>
         requires exstd::Callable<decltype([](Args...) -> Ret
               {
               })>
               && exstd::FunctionObject<decltype([](Args...) -> Ret
               {
               })>
               && (!exstd::FreeFunction<decltype([](Args...) -> Ret
               {
               })>)
               && (!exstd::MemberFunction<decltype([](Args...) -> Ret
               {
               })>)
     struct CallableConceptLambdaEval<Ret(Args...)> : std::true_type
    {
    };

    template <typename T>
    struct CallableConceptFreeEval : std::false_type
    {
    };

    template <typename Ret, typename... Args>
        requires exstd::Callable<Ret(Args...)> && exstd::Callable<Ret (*)(Args...)>
              && exstd::Callable<Ret (*&)(Args...)> && exstd::Callable<Ret (*&&)(Args...)>
              && exstd::Callable<Ret (&)(Args...)> && exstd::Callable<Ret (&&)(Args...)>
              && exstd::FreeFunction<Ret(Args...)> && exstd::FreeFunction<Ret (*)(Args...)>
              && exstd::FreeFunction<Ret (*&)(Args...)> && exstd::FreeFunction<Ret (*&&)(Args...)>
              && exstd::FreeFunction<Ret (&)(Args...)> && exstd::FreeFunction<Ret (&&)(Args...)>
              && (!exstd::MemberFunction<Ret(Args...)>) && (!exstd::MemberFunction<Ret (*)(Args...)>)
              && (!exstd::MemberFunction<Ret (*&)(Args...)>) && (!exstd::MemberFunction<Ret (*&&)(Args...)>)
              && (!exstd::MemberFunction<Ret (&)(Args...)>) && (!exstd::MemberFunction<Ret (&&)(Args...)>)
              && (!exstd::FunctionObject<Ret(Args...)>) && (!exstd::FunctionObject<Ret (*)(Args...)>)
              && (!exstd::FunctionObject<Ret (*&)(Args...)>) && (!exstd::FunctionObject<Ret (*&&)(Args...)>)
              && (!exstd::FunctionObject<Ret (&)(Args...)>) && (!exstd::FunctionObject<Ret (&&)(Args...)>)
    struct CallableConceptFreeEval<Ret(Args...)> : std::true_type
    {
    };

    template <typename T>
    struct CallableConceptMemberEval : std::false_type
    {
    };

    struct TestStruct;

    template <typename Ret, typename... Args>
        requires exstd::Callable<Ret (TestStruct::*)(Args...)> && exstd::Callable<Ret (TestStruct::*&)(Args...)>
              && exstd::Callable<Ret (TestStruct::*&&)(Args...)> && exstd::Callable<Ret (TestStruct::*)(Args...) const>
              && exstd::Callable<Ret (TestStruct::*&)(Args...) const>
              && exstd::Callable<Ret (TestStruct::*&&)(Args...) const>
              && exstd::Callable<Ret (TestStruct::*)(Args...) volatile>
              && exstd::Callable<Ret (TestStruct::*&)(Args...) volatile>
              && exstd::Callable<Ret (TestStruct::*&&)(Args...) volatile>
              && exstd::Callable<Ret (TestStruct::*)(Args...) const volatile>
              && exstd::Callable<Ret (TestStruct::*&)(Args...) const volatile>
              && exstd::Callable<Ret (TestStruct::*&&)(Args...) const volatile>
              && exstd::Callable<Ret (TestStruct::*)(Args...) &> && exstd::Callable<Ret (TestStruct::*&)(Args...) &>
              && exstd::Callable<Ret (TestStruct::*&&)(Args...) &>
              && exstd::Callable<Ret (TestStruct::*)(Args...) const&>
              && exstd::Callable<Ret (TestStruct::*&)(Args...) const&>
              && exstd::Callable<Ret (TestStruct::*&&)(Args...) const&>
              && exstd::Callable<Ret (TestStruct::*)(Args...) volatile&>
              && exstd::Callable<Ret (TestStruct::*&)(Args...) volatile&>
              && exstd::Callable<Ret (TestStruct::*&&)(Args...) volatile&>
              && exstd::Callable<Ret (TestStruct::*)(Args...) const volatile&>
              && exstd::Callable<Ret (TestStruct::*&)(Args...) const volatile&>
              && exstd::Callable<Ret (TestStruct::*&&)(Args...) const volatile&>
              && exstd::Callable<Ret (TestStruct::*)(Args...) &&> && exstd::Callable<Ret (TestStruct::*&)(Args...) &&>
              && exstd::Callable<Ret (TestStruct::*&&)(Args...) &&>
              && exstd::Callable<Ret (TestStruct::*)(Args...) const&&>
              && exstd::Callable<Ret (TestStruct::*&)(Args...) const&&>
              && exstd::Callable<Ret (TestStruct::*&&)(Args...) const&&>
              && exstd::Callable<Ret (TestStruct::*)(Args...) volatile&&>
              && exstd::Callable<Ret (TestStruct::*&)(Args...) volatile&&>
              && exstd::Callable<Ret (TestStruct::*&&)(Args...) volatile&&>
              && exstd::Callable<Ret (TestStruct::*)(Args...) const volatile&&>
              && exstd::Callable<Ret (TestStruct::*&)(Args...) const volatile&&>
              && exstd::Callable<Ret (TestStruct::*&&)(Args...) const volatile&&>
              && exstd::MemberFunction<Ret (TestStruct::*)(Args...)>
              && exstd::MemberFunction<Ret (TestStruct::*&)(Args...)>
              && exstd::MemberFunction<Ret (TestStruct::*&&)(Args...)>
              && exstd::MemberFunction<Ret (TestStruct::*)(Args...) const>
              && exstd::MemberFunction<Ret (TestStruct::*&)(Args...) const>
              && exstd::MemberFunction<Ret (TestStruct::*&&)(Args...) const>
              && exstd::MemberFunction<Ret (TestStruct::*)(Args...) volatile>
              && exstd::MemberFunction<Ret (TestStruct::*&)(Args...) volatile>
              && exstd::MemberFunction<Ret (TestStruct::*&&)(Args...) volatile>
              && exstd::MemberFunction<Ret (TestStruct::*)(Args...) const volatile>
              && exstd::MemberFunction<Ret (TestStruct::*&)(Args...) const volatile>
              && exstd::MemberFunction<Ret (TestStruct::*&&)(Args...) const volatile>
              && exstd::MemberFunction<Ret (TestStruct::*)(Args...) &>
              && exstd::MemberFunction<Ret (TestStruct::*&)(Args...) &>
              && exstd::MemberFunction<Ret (TestStruct::*&&)(Args...) &>
              && exstd::MemberFunction<Ret (TestStruct::*)(Args...) const&>
              && exstd::MemberFunction<Ret (TestStruct::*&)(Args...) const&>
              && exstd::MemberFunction<Ret (TestStruct::*&&)(Args...) const&>
              && exstd::MemberFunction<Ret (TestStruct::*)(Args...) volatile&>
              && exstd::MemberFunction<Ret (TestStruct::*&)(Args...) volatile&>
              && exstd::MemberFunction<Ret (TestStruct::*&&)(Args...) volatile&>
              && exstd::MemberFunction<Ret (TestStruct::*)(Args...) const volatile&>
              && exstd::MemberFunction<Ret (TestStruct::*&)(Args...) const volatile&>
              && exstd::MemberFunction<Ret (TestStruct::*&&)(Args...) const volatile&>
              && exstd::MemberFunction<Ret (TestStruct::*)(Args...) &&>
              && exstd::MemberFunction<Ret (TestStruct::*&)(Args...) &&>
              && exstd::MemberFunction<Ret (TestStruct::*&&)(Args...) &&>
              && exstd::MemberFunction<Ret (TestStruct::*)(Args...) const&&>
              && exstd::MemberFunction<Ret (TestStruct::*&)(Args...) const&&>
              && exstd::MemberFunction<Ret (TestStruct::*&&)(Args...) const&&>
              && exstd::MemberFunction<Ret (TestStruct::*)(Args...) volatile&&>
              && exstd::MemberFunction<Ret (TestStruct::*&)(Args...) volatile&&>
              && exstd::MemberFunction<Ret (TestStruct::*&&)(Args...) volatile&&>
              && exstd::MemberFunction<Ret (TestStruct::*)(Args...) const volatile&&>
              && exstd::MemberFunction<Ret (TestStruct::*&)(Args...) const volatile&&>
              && exstd::MemberFunction<Ret (TestStruct::*&&)(Args...) const volatile&&>
              && (!exstd::FreeFunction<Ret (TestStruct::*)(Args...)>)
              && (!exstd::FreeFunction<Ret (TestStruct::*&)(Args...)>)
              && (!exstd::FreeFunction<Ret (TestStruct::* &&)(Args...)>)
              && (!exstd::FreeFunction<Ret (TestStruct::*)(Args...) const>)
              && (!exstd::FreeFunction<Ret (TestStruct::*&)(Args...) const>)
              && (!exstd::FreeFunction<Ret (TestStruct::* &&)(Args...) const>)
              && (!exstd::FreeFunction<Ret (TestStruct::*)(Args...) volatile>)
              && (!exstd::FreeFunction<Ret (TestStruct::*&)(Args...) volatile>)
              && (!exstd::FreeFunction<Ret (TestStruct::* &&)(Args...) volatile>)
              && (!exstd::FreeFunction<Ret (TestStruct::*)(Args...) const volatile>)
              && (!exstd::FreeFunction<Ret (TestStruct::*&)(Args...) const volatile>)
              && (!exstd::FreeFunction<Ret (TestStruct::* &&)(Args...) const volatile>)
              && (!exstd::FreeFunction<Ret (TestStruct::*)(Args...) &>)
              && (!exstd::FreeFunction<Ret (TestStruct::*&)(Args...) &>)
              && (!exstd::FreeFunction<Ret (TestStruct::* &&)(Args...) &>)
              && (!exstd::FreeFunction<Ret (TestStruct::*)(Args...) const&>)
              && (!exstd::FreeFunction<Ret (TestStruct::*&)(Args...) const&>)
              && (!exstd::FreeFunction<Ret (TestStruct::* &&)(Args...) const&>)
              && (!exstd::FreeFunction<Ret (TestStruct::*)(Args...) volatile&>)
              && (!exstd::FreeFunction<Ret (TestStruct::*&)(Args...) volatile&>)
              && (!exstd::FreeFunction<Ret (TestStruct::* &&)(Args...) volatile&>)
              && (!exstd::FreeFunction<Ret (TestStruct::*)(Args...) const volatile&>)
              && (!exstd::FreeFunction<Ret (TestStruct::*&)(Args...) const volatile&>)
              && (!exstd::FreeFunction<Ret (TestStruct::* &&)(Args...) const volatile&>)
              && (!exstd::FreeFunction<Ret (TestStruct::*)(Args...) &&>)
              && (!exstd::FreeFunction<Ret (TestStruct::*&)(Args...) &&>)
              && (!exstd::FreeFunction<Ret (TestStruct::* &&)(Args...) &&>)
              && (!exstd::FreeFunction<Ret (TestStruct::*)(Args...) const &&>)
              && (!exstd::FreeFunction<Ret (TestStruct::*&)(Args...) const &&>)
              && (!exstd::FreeFunction<Ret (TestStruct::* &&)(Args...) const &&>)
              && (!exstd::FreeFunction<Ret (TestStruct::*)(Args...) volatile &&>)
              && (!exstd::FreeFunction<Ret (TestStruct::*&)(Args...) volatile &&>)
              && (!exstd::FreeFunction<Ret (TestStruct::* &&)(Args...) volatile &&>)
              && (!exstd::FreeFunction<Ret (TestStruct::*)(Args...) const volatile &&>)
              && (!exstd::FreeFunction<Ret (TestStruct::*&)(Args...) const volatile &&>)
              && (!exstd::FreeFunction<Ret (TestStruct::* &&)(Args...) const volatile &&>)
              && (!exstd::FunctionObject<Ret (TestStruct::*)(Args...)>)
              && (!exstd::FunctionObject<Ret (TestStruct::*&)(Args...)>)
              && (!exstd::FunctionObject<Ret (TestStruct::* &&)(Args...)>)
              && (!exstd::FunctionObject<Ret (TestStruct::*)(Args...) const>)
              && (!exstd::FunctionObject<Ret (TestStruct::*&)(Args...) const>)
              && (!exstd::FunctionObject<Ret (TestStruct::* &&)(Args...) const>)
              && (!exstd::FunctionObject<Ret (TestStruct::*)(Args...) volatile>)
              && (!exstd::FunctionObject<Ret (TestStruct::*&)(Args...) volatile>)
              && (!exstd::FunctionObject<Ret (TestStruct::* &&)(Args...) volatile>)
              && (!exstd::FunctionObject<Ret (TestStruct::*)(Args...) const volatile>)
              && (!exstd::FunctionObject<Ret (TestStruct::*&)(Args...) const volatile>)
              && (!exstd::FunctionObject<Ret (TestStruct::* &&)(Args...) const volatile>)
              && (!exstd::FunctionObject<Ret (TestStruct::*)(Args...) &>)
              && (!exstd::FunctionObject<Ret (TestStruct::*&)(Args...) &>)
              && (!exstd::FunctionObject<Ret (TestStruct::* &&)(Args...) &>)
              && (!exstd::FunctionObject<Ret (TestStruct::*)(Args...) const&>)
              && (!exstd::FunctionObject<Ret (TestStruct::*&)(Args...) const&>)
              && (!exstd::FunctionObject<Ret (TestStruct::* &&)(Args...) const&>)
              && (!exstd::FunctionObject<Ret (TestStruct::*)(Args...) volatile&>)
              && (!exstd::FunctionObject<Ret (TestStruct::*&)(Args...) volatile&>)
              && (!exstd::FunctionObject<Ret (TestStruct::* &&)(Args...) volatile&>)
              && (!exstd::FunctionObject<Ret (TestStruct::*)(Args...) const volatile&>)
              && (!exstd::FunctionObject<Ret (TestStruct::*&)(Args...) const volatile&>)
              && (!exstd::FunctionObject<Ret (TestStruct::* &&)(Args...) const volatile&>)
              && (!exstd::FunctionObject<Ret (TestStruct::*)(Args...) &&>)
              && (!exstd::FunctionObject<Ret (TestStruct::*&)(Args...) &&>)
              && (!exstd::FunctionObject<Ret (TestStruct::* &&)(Args...) &&>)
              && (!exstd::FunctionObject<Ret (TestStruct::*)(Args...) const &&>)
              && (!exstd::FunctionObject<Ret (TestStruct::*&)(Args...) const &&>)
              && (!exstd::FunctionObject<Ret (TestStruct::* &&)(Args...) const &&>)
              && (!exstd::FunctionObject<Ret (TestStruct::*)(Args...) volatile &&>)
              && (!exstd::FunctionObject<Ret (TestStruct::*&)(Args...) volatile &&>)
              && (!exstd::FunctionObject<Ret (TestStruct::* &&)(Args...) volatile &&>)
              && (!exstd::FunctionObject<Ret (TestStruct::*)(Args...) const volatile &&>)
              && (!exstd::FunctionObject<Ret (TestStruct::*&)(Args...) const volatile &&>)
              && (!exstd::FunctionObject<Ret (TestStruct::* &&)(Args...) const volatile &&>)
    struct CallableConceptMemberEval<Ret(Args...)> : std::true_type
    {
    };
}

void callableConcepts()
{
    using TestTypes = TypeList<
        void(), void(int), void(int, int), void(int, int, int), void(int, int, int, int), int(), int(int),
        int(int, int), int(int, int, int), int(int, int, int, int)>;

    static_assert(TypeAssertionValue<CallableConceptEvals::CallableConceptLambdaEval, TestTypes>);
    static_assert(TypeAssertionValue<CallableConceptEvals::CallableConceptFreeEval, TestTypes>);
    static_assert(TypeAssertionValue<CallableConceptEvals::CallableConceptMemberEval, TestTypes>);
}

int main()
{
    callableConcepts();

    return 0;
}
