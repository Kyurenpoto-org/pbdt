#pragma once

#include <type_traits>

namespace
{
    template <typename... Ts>
    struct TypeList;

    template <template <typename> typename Eval, typename Tl>
    struct TypeAssertion : std::false_type
    {
    };

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
    constexpr bool TypeAssertionValue = TypeAssertion<Eval, Tl>::value;
}
