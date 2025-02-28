#pragma once

#include <ranges>

namespace example
{
    template <typename... Ranges>
    constexpr auto product(Ranges&&... ranges)
    {
        return std::views::cartesian_product(std::forward<Ranges>(ranges)...);
    }

    template <typename Range>
    constexpr auto product(Range&& range)
    {
        return range;
    }
}
