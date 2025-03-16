/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <algorithm>
#include <array>
#include <ranges>
#include <span>
#include <tuple>
#include <type_traits>
#include <vector>

#include "exstd/type-traits.hpp"

namespace exstd
{
    namespace detail
    {
        template <typename T>
        constexpr auto toConcatable(T&& sample)
        {
            if constexpr (TupleForm<T>)
            {
                return sample;
            }
            else
            {
                return std::tuple{ sample };
            }
        }

        struct FlatTuple
        {
            constexpr auto operator()(const auto& elem) const
            {
                return std::apply(
                    []<typename... Args>(Args&&... args)
                    {
                        return std::tuple_cat(toConcatable(std::forward<Args>(args))...);
                    },
                    elem
                );
            }
        };

#if __cpp_lib_ranges_cartesian_product >= 202207L
        template <typename... Ranges>
        using CartesianProductView = std::ranges::cartesian_product_view<Ranges...>;
#else
        template <std::ranges::input_range First, std::ranges::forward_range... Rests>
        class CartesianProductView
        {
        private:
            struct iterator
            {
                friend CartesianProductView;

                using value_type = std::tuple<std::ranges::range_value_t<First>, std::ranges::range_value_t<Rests>...>;
                using reference =
                    std::tuple<std::ranges::range_reference_t<First>, std::ranges::range_reference_t<Rests>...>;
                using difference_type = std::ptrdiff_t;
                using iterator_category = std::input_iterator_tag;

                constexpr iterator() = default;

                constexpr iterator& operator++()
                {
                    next();
                    return *this;
                }

                constexpr void operator++(int)
                {
                    ++*this;
                }

                constexpr iterator operator++(int)
                    requires std::ranges::forward_range<First>
                {
                    auto tmp = *this;
                    ++*this;
                    return tmp;
                }

                [[nodiscard]]
                constexpr auto operator*() const
                {
                    return std::apply(
                        []<typename... Args>(Args&&... args)
                        {
                            return std::tuple{ *std::forward<Args>(args)... };
                        },
                        current
                    );
                }

                [[nodiscard]]
                constexpr bool operator==(const iterator& other) const
                    requires std::equality_comparable<std::ranges::iterator_t<First>>
                {
                    return current == other.current;
                }

                [[nodiscard]]
                constexpr bool operator==(std::default_sentinel_t) const
                {
                    return lastElement(std::make_index_sequence<sizeof...(Rests)>{});
                }

                [[nodiscard]]
                constexpr bool operator<=>(const iterator& other) const
                {
                    return current <=> other.current;
                }

            private:
                constexpr iterator(
                    CartesianProductView& parent,
                    std::tuple<std::ranges::iterator_t<First>, std::ranges::iterator_t<Rests>...> current
                ) :
                    parent{ parent },
                    current{ std::move(current) }
                {
                }

                template <size_t Idx = sizeof...(Rests)>
                constexpr void next()
                {
                    auto& it = std::get<Idx>(current);
                    ++it;
                    if constexpr (Idx > 0)
                    {
                        auto& range = std::get<Idx>(parent.ranges);
                        if (it == std::end(range))
                        {
                            it = std::begin(range);
                            next<Idx - 1>();
                        }
                    }
                }

                template <size_t... Indice>
                constexpr bool lastElement(std::index_sequence<Indice...>) const
                {
                    return ((std::get<Indice>(current) == std::ranges::end(std::get<Indice>(parent.ranges))) || ...);
                }

                CartesianProductView& parent;
                std::tuple<std::ranges::iterator_t<First>, std::ranges::iterator_t<Rests>...> current;
            };

        public:
            constexpr CartesianProductView() = default;

            constexpr CartesianProductView(First first, Rests... rests) :
                ranges{ std::move(first), std::move(rests)... }
            {
            }

            constexpr iterator begin() const
            {
                return iterator{ *this, std::apply(
                                            [](auto&&... args)
                                            {
                                                return std::tuple{ std::ranges::begin(args)... };
                                            },
                                            ranges
                                        ) };
            }

            constexpr std::default_sentinel_t end() const
            {
                return std::default_sentinel;
            }

        private:
            std::tuple<First, Rests...> ranges;
        };
#endif

        template <typename... Ranges>
        struct FlattenCartesianProductView : std::ranges::view_interface<FlattenCartesianProductView<Ranges...>>
        {
            constexpr FlattenCartesianProductView() = default;

            constexpr FlattenCartesianProductView(Ranges... ranges) :
                base{ std::move(ranges)... }
            {
            }

            constexpr auto begin() const
            {
                return base.begin();
            }

            constexpr auto end() const
            {
                return base.end();
            }

            constexpr auto size() const
            {
                return base.size();
            }

            constexpr auto empty() const
            {
                return base.empty();
            }

        private:
            CartesianProductView<Ranges...> base;
        };

        template <typename... Ranges>
            requires(TupleForm<std::ranges::range_value_t<Ranges>> || ...)
        struct FlattenCartesianProductView<Ranges...> :
            std::ranges::view_interface<FlattenCartesianProductView<Ranges...>>
        {
            constexpr FlattenCartesianProductView() = default;

            constexpr FlattenCartesianProductView(Ranges... ranges) :
                base{
                    CartesianProductView<Ranges...>{ std::move(ranges)... },
                    FlatTuple{},
                }
            {
            }

            constexpr auto begin() const
            {
                return base.begin();
            }

            constexpr auto end() const
            {
                return base.end();
            }

            constexpr auto size() const
            {
                return base.size();
            }

            constexpr auto empty() const
            {
                return base.empty();
            }

        private:
            std::ranges::transform_view<CartesianProductView<Ranges...>, FlatTuple> base;
        };

        template <typename... Ranges>
        FlattenCartesianProductView(Ranges&&...) -> FlattenCartesianProductView<std::views::all_t<Ranges>...>;
    }

    template <typename... Ranges>
        requires(std::ranges::range<Ranges> && ...)
    constexpr auto flattenCartesianProduct(Ranges&&... ranges)
    {
        return detail::FlattenCartesianProductView{ std::forward<Ranges>(ranges)... };
    }

    // https://stackoverflow.com/questions/70482497/detecting-compile-time-constantness-of-range-size
    namespace detail
    {
        template <auto>
        struct require_constant;

        template <typename>
        struct CompileTimeSimpleRangeExtent;

        template <std::ranges::input_range Range>
        struct CompileTimeSimpleRangeExtent<Range> : std::integral_constant<size_t, std::dynamic_extent>
        {
        };

        template <std::ranges::input_range Range>
            requires std::is_bounded_array_v<Range>
        struct CompileTimeSimpleRangeExtent<Range> : std::integral_constant<size_t, std::extent_v<Range>>
        {
        };

        template <std::ranges::sized_range Range>
            requires(requires { typename require_constant<Range::size()>; })
        struct CompileTimeSimpleRangeExtent<Range> : std::integral_constant<size_t, Range::size()>
        {
        };

        template <typename T, size_t N>
        struct CompileTimeSimpleRangeExtent<std::array<T, N>> : std::integral_constant<size_t, N>
        {
        };

        template <typename T, size_t N>
        struct CompileTimeSimpleRangeExtent<std::span<T, N>> : std::integral_constant<size_t, N>
        {
        };

        template <typename View>
            requires std::ranges::view<std::remove_cvref_t<View>>
        struct CompileTimeViewExtent : std::integral_constant<size_t, std::dynamic_extent>
        {
        };

        template <std::ranges::range Range>
        struct CompileTimeViewExtent<std::ranges::ref_view<Range>> :
            std::integral_constant<size_t, CompileTimeSimpleRangeExtent<std::remove_cvref_t<Range>>::value>
        {
        };

        template <std::ranges::range Range>
        struct CompileTimeViewExtent<std::ranges::owning_view<Range>> :
            std::integral_constant<size_t, CompileTimeSimpleRangeExtent<std::remove_cvref_t<Range>>::value>
        {
        };

        template <typename View>
            requires std::ranges::view<std::remove_cvref_t<View>>
        struct CompileTimeViewExtent<std::ranges::reverse_view<View>> : CompileTimeViewExtent<std::remove_cvref_t<View>>
        {
        };

#if __cpp_lib_ranges_enumerate >= 202302L
        template <typename View>
            requires std::ranges::view<std::remove_cvref_t<View>>
        struct CompileTimeViewExtent<std::ranges::enumerate_view<View>> :
            CompileTimeViewExtent<std::remove_cvref_t<View>>
        {
        };
#endif

        template <std::ranges::input_range Range, size_t N>
        struct CompileTimeViewExtent<std::ranges::elements_view<Range, N>> :
            CompileTimeViewExtent<std::remove_cvref_t<Range>>
        {
        };

        template <std::ranges::input_range Range, typename F>
        struct CompileTimeViewExtent<std::ranges::transform_view<Range, F>> :
            CompileTimeViewExtent<std::remove_cvref_t<Range>>
        {
        };

        template <std::ranges::input_range... Ranges>
        struct CompileTimeViewExtent<std::ranges::zip_view<Ranges...>> :
            std::integral_constant<size_t, std::min({ CompileTimeViewExtent<std::remove_cvref_t<Ranges>>::value... })>
        {
        };

#if __cpp_lib_ranges_zip >= 202110L
        template <typename F, std::ranges::input_range... Ranges>
        struct CompileTimeViewExtent<std::ranges::zip_transform_view<F, Ranges...>> :
            std::integral_constant<size_t, std::min({ CompileTimeViewExtent<std::remove_cvref_t<Ranges>>::value... })>
        {
        };

        template <std::ranges::forward_range Range, size_t N>
        struct CompileTimeViewExtent<std::ranges::adjacent_view<Range, N>> :
            std::integral_constant<
                size_t, CompileTimeViewExtent<std::remove_cvref_t<Range>>::value == std::dynamic_extent
                            ? std::dynamic_extent
                            : CompileTimeViewExtent<std::remove_cvref_t<Range>>::value - N + 1>
        {
        };

        template <std::ranges::forward_range Range, typename F, size_t N>
        struct CompileTimeViewExtent<std::ranges::adjacent_transform_view<Range, F, N>> :
            std::integral_constant<
                size_t, CompileTimeViewExtent<std::remove_cvref_t<Range>>::value == std::dynamic_extent
                            ? std::dynamic_extent
                            : CompileTimeViewExtent<std::remove_cvref_t<Range>>::value - N + 1>
        {
        };
#endif

        template <std::ranges::input_range First, std::ranges::forward_range... Rests>
        struct CompileTimeViewExtent<CartesianProductView<First, Rests...>> :
            std::integral_constant<
                size_t, (((CompileTimeViewExtent<std::remove_cvref_t<First>>::value == std::dynamic_extent) || ...
                          || (CompileTimeViewExtent<std::remove_cvref_t<Rests>>::value == std::dynamic_extent))
                             ? std::dynamic_extent
                             : (CompileTimeViewExtent<std::remove_cvref_t<First>>::value * ...
                                * CompileTimeViewExtent<std::remove_cvref_t<Rests>>::value))>
        {
        };

        template <typename... Ranges>
        struct CompileTimeViewExtent<FlattenCartesianProductView<Ranges...>> :
            CompileTimeViewExtent<CartesianProductView<Ranges...>>
        {
        };

        template <typename... Ranges>
            requires(TupleForm<std::ranges::range_value_t<Ranges>> || ...)
        struct CompileTimeViewExtent<FlattenCartesianProductView<Ranges...>> :
            CompileTimeViewExtent<std::ranges::transform_view<CartesianProductView<Ranges...>, FlatTuple>>
        {
        };

        template <std::ranges::input_range Range>
        consteval auto compileTimeRangeExtent()
        {
            constexpr auto size = CompileTimeSimpleRangeExtent<Range>::value;
            if constexpr (size == std::dynamic_extent)
            {
                return CompileTimeViewExtent<Range>::value;
            }
            else
            {
                return size;
            }
        }
    }

    template <std::ranges::input_range Range>
    constexpr auto toContainer(Range&& range)
    {
        using value_type = std::remove_cvref_t<std::ranges::range_value_t<Range>>;
        constexpr auto size = detail::compileTimeRangeExtent<std::remove_cvref_t<Range>>();
        if constexpr (size != std::dynamic_extent)
        {
            std::array<value_type, size> result;
            std::copy(range.begin(), range.end(), result.begin());
            return result;
        }
        else
        {
            std::vector<value_type> result;
            if constexpr (std::ranges::sized_range<Range>)
            {
                result.reserve(range.size());
            }
            std::copy(range.begin(), range.end(), std::back_inserter(result));
            return result;
        }
    }
}
