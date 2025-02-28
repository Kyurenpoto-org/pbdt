#include <array>
#include <print>
#include <ranges>

#include "boost/ut.hpp"
#include "frozen/set.h"

#include "example/example-lib.hpp"

int main()
{
    {
        static_assert(
            frozen::make_set(example::product(std::array<int, 3>{ 1, 2, 3 }))
            == frozen::make_set(example::product(std::array<int, 3>{ 3, 2, 1 }))
        );
    }

    {
        constexpr auto toArray = [](auto&& range)
        {
            std::array<std::tuple<int, int>, 15> result;
            std::copy(range.begin(), range.end(), result.begin());
            return result;
        };

        constexpr auto a =
            toArray(example::product(std::array<int, 3>{ 1, 2, 3 }, std::array<int, 5>{ 1, 2, 3, 4, 5 }));
        constexpr auto b =
            toArray(example::product(std::array<int, 5>{ 1, 2, 3, 4, 5 }, std::array<int, 3>{ 1, 2, 3 }));

        static_assert(
            frozen::make_set(a)
            == frozen::make_set(toArray(
                b
                | std::views::transform(
                    [](const auto& tup)
                    {
                        return std::tuple{ std::get<1>(tup), std::get<0>(tup) };
                    }
                )
            ))
        );
    }

    return EXIT_SUCCESS;
}
