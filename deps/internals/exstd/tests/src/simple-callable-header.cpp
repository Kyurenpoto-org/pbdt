/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "exstd/callable-traits.hpp"

#include "type-assertion.hpp"

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
    simpleCallableTypes();

    return 0;
}
