/*
 * SPDX - FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "exstd/callable-traits.hpp"

namespace
{
    template <typename... Ts>
    struct TypeList;

    template <template <typename> typename Eval, typename Tl>
    struct TypeAssertion;

    template <template <typename> typename Eval, typename... Ts>
    struct TypeAssertion<Eval, TypeList<Ts...>> : std::false_type
    {
    };

    template <template <typename> typename Eval, typename First, typename... Rest>
        requires Eval<First>::value
    struct TypeAssertion<Eval, TypeList<First, Rest...>> : TypeAssertion<Eval, TypeList<Rest...>>
    {
    };

    template <template <typename> typename Eval>
    struct TypeAssertion<Eval, TypeList<>> : std::true_type
    {
    };

    template <template <typename> typename Eval, typename Tl>
    constexpr bool TypeAssertionValue = false;

    template <template <typename> typename Eval, typename... Ts>
    constexpr bool TypeAssertionValue<Eval, TypeList<Ts...>> = TypeAssertion<Eval, TypeList<Ts...>>::value;
}

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

namespace SimpleCallableTypeEvals
{
    template <typename T>
    struct SimpleCallableLambdaEval : std::false_type
    {
    };

    template <typename Ret, typename... Args>
    struct SimpleCallableLambdaEval<Ret(Args...)>
        : std::is_same<
              Ret(Args...),
              exstd::SimpleCallableType<decltype([](Args...) -> Ret
              {
              })>>
    {
    };

    template <typename T>
    struct SimpleCallableFreeEval : std::false_type
    {
    };

    template <typename Ret, typename... Args>
    struct SimpleCallableFreeEval<Ret(Args...)> :
        std::conjunction<
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret(Args...)>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (*)(Args...)>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (*&)(Args...)>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (*&&)(Args...)>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (&)(Args...)>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (&&)(Args...)>>>
    {
    };

    template <typename T>
    struct SimpleCallableMemberEval : std::false_type
    {
    };

    struct TestStruct;

    template <typename Ret, typename... Args>
    struct SimpleCallableMemberEval<Ret(Args...)> :
        std::conjunction<
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*)(Args...)>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&)(Args...)>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&&)(Args...)>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*)(Args...) const>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&)(Args...) const>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&&)(Args...) const>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*)(Args...) volatile>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&)(Args...) volatile>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&&)(Args...) volatile>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*)(Args...) const volatile>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&)(Args...) const volatile>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&&)(Args...) const volatile>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*)(Args...)&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&)(Args...)&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&&)(Args...)&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*)(Args...) const&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&)(Args...) const&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&&)(Args...) const&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*)(Args...) volatile&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&)(Args...) volatile&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&&)(Args...) volatile&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*)(Args...) const volatile&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&)(Args...) const volatile&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&&)(Args...) const volatile&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*)(Args...) &&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&)(Args...) &&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&&)(Args...) &&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*)(Args...) const&&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&)(Args...) const&&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&&)(Args...) const&&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*)(Args...) volatile&&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&)(Args...) volatile&&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&&)(Args...) volatile&&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*)(Args...) const volatile&&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&)(Args...) const volatile&&>>,
            std::is_same<Ret(Args...), exstd::SimpleCallableType<Ret (TestStruct::*&&)(Args...) const volatile&&>>>
    {
    };
}

void simpleCallableTypes()
{
    using TestTypes = TypeList<
        void(), void(int), void(int, int), void(int, int, int), void(int, int, int, int), int(), int(int),
        int(int, int), int(int, int, int), int(int, int, int, int)>;

    static_assert(TypeAssertionValue<SimpleCallableTypeEvals::SimpleCallableLambdaEval, TestTypes>);
    static_assert(TypeAssertionValue<SimpleCallableTypeEvals::SimpleCallableFreeEval, TestTypes>);
    static_assert(TypeAssertionValue<SimpleCallableTypeEvals::SimpleCallableMemberEval, TestTypes>);
}

int main()
{
    callableConcepts();
    simpleCallableTypes();

    return 0;
}
