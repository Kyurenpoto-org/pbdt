/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "type-assertion.hpp"

namespace SimpleCallableTypeEvals
{
    template <template <typename> typename, typename>
    struct LambdaEvalImpl : std::false_type
    {
    };

    template <template <typename> typename SimpleCallableType, typename Ret, typename... Args>
      struct LambdaEvalImpl<SimpleCallableType, Ret(Args...)>
          : std::is_same<
                Ret(Args...),
                SimpleCallableType<decltype([](Args...) -> Ret
                {
                })>>
    {
    };

    template <template <typename> typename SimpleCallableType>
    struct LambdaEval
    {
        template <typename T>
        using type = LambdaEvalImpl<SimpleCallableType, T>;
    };

    template <template <typename> typename, typename>
    struct FreeEvalImpl : std::false_type
    {
    };

    template <template <typename> typename SimpleCallableType, typename Ret, typename... Args>
    struct FreeEvalImpl<SimpleCallableType, Ret(Args...)> :
        std::conjunction<
            std::is_same<Ret(Args...), SimpleCallableType<Ret(Args...)>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (*)(Args...)>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (*&)(Args...)>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (*&&)(Args...)>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (&)(Args...)>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (&&)(Args...)>>>
    {
    };

    template <template <typename> typename SimpleCallableType>
    struct FreeEval
    {
        template <typename T>
        using type = FreeEvalImpl<SimpleCallableType, T>;
    };

    template <template <typename> typename, typename>
    struct MemberEvalImpl : std::false_type
    {
    };

    struct TestStruct;

    template <template <typename> typename SimpleCallableType, typename Ret, typename... Args>
    struct MemberEvalImpl<SimpleCallableType, Ret(Args...)> :
        std::conjunction<
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*)(Args...)>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&)(Args...)>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&&)(Args...)>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*)(Args...) const>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&)(Args...) const>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&&)(Args...) const>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*)(Args...) volatile>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&)(Args...) volatile>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&&)(Args...) volatile>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*)(Args...) const volatile>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&)(Args...) const volatile>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&&)(Args...) const volatile>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*)(Args...)&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&)(Args...)&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&&)(Args...)&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*)(Args...) const&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&)(Args...) const&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&&)(Args...) const&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*)(Args...) volatile&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&)(Args...) volatile&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&&)(Args...) volatile&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*)(Args...) const volatile&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&)(Args...) const volatile&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&&)(Args...) const volatile&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*)(Args...) &&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&)(Args...) &&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&&)(Args...) &&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*)(Args...) const&&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&)(Args...) const&&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&&)(Args...) const&&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*)(Args...) volatile&&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&)(Args...) volatile&&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&&)(Args...) volatile&&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*)(Args...) const volatile&&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&)(Args...) const volatile&&>>,
            std::is_same<Ret(Args...), SimpleCallableType<Ret (TestStruct::*&&)(Args...) const volatile&&>>>
    {
    };

    template <template <typename> typename SimpleCallableType>
    struct MemberEval
    {
        template <typename T>
        using type = MemberEvalImpl<SimpleCallableType, T>;
    };
}

template <template <typename> typename SimpleCallableType>
void simpleCallableTypes()
{
    using TestTypes = TypeList<
        void(), void(int), void(int, int), void(int, int, int), void(int, int, int, int), int(), int(int),
        int(int, int), int(int, int, int), int(int, int, int, int)>;

    static_assert(TypeAssertionValue<
                  typename SimpleCallableTypeEvals::LambdaEval<SimpleCallableType>::type, TestTypes>);
    static_assert(TypeAssertionValue<typename SimpleCallableTypeEvals::FreeEval<SimpleCallableType>::type, TestTypes>);
    static_assert(TypeAssertionValue<
                  typename SimpleCallableTypeEvals::MemberEval<SimpleCallableType>::type, TestTypes>);
}
