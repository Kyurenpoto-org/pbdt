/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <array>
#include <ranges>
#include <span>
#include <string_view>
#include <tuple>

#include "productable-container.hpp"

template class Productable::ProductableContainer<0, decltype(std::make_index_sequence<1>()), 0>;
template class Productable::ProductableContainer<0, decltype(std::make_index_sequence<2>()), 0>;
template class Productable::ProductableContainer<0, decltype(std::make_index_sequence<3>()), 0>;
template class Productable::ProductableContainer<0, decltype(std::make_index_sequence<1>()), 1>;
template class Productable::ProductableContainer<0, decltype(std::make_index_sequence<2>()), 1>;
template class Productable::ProductableContainer<0, decltype(std::make_index_sequence<3>()), 1>;
template class Productable::ProductableContainer<1, decltype(std::make_index_sequence<1>()), 0>;
template class Productable::ProductableContainer<1, decltype(std::make_index_sequence<2>()), 0>;
template class Productable::ProductableContainer<1, decltype(std::make_index_sequence<3>()), 0>;
template class Productable::ProductableContainer<1, decltype(std::make_index_sequence<1>()), 1>;
template class Productable::ProductableContainer<1, decltype(std::make_index_sequence<2>()), 1>;
template class Productable::ProductableContainer<1, decltype(std::make_index_sequence<3>()), 1>;
template class Productable::ProductableContainer<2, decltype(std::make_index_sequence<1>()), 0>;
template class Productable::ProductableContainer<2, decltype(std::make_index_sequence<2>()), 0>;
template class Productable::ProductableContainer<2, decltype(std::make_index_sequence<3>()), 0>;
template class Productable::ProductableContainer<2, decltype(std::make_index_sequence<1>()), 1>;
template class Productable::ProductableContainer<2, decltype(std::make_index_sequence<2>()), 1>;
template class Productable::ProductableContainer<2, decltype(std::make_index_sequence<3>()), 1>;
template class Productable::ProductableContainer<3, decltype(std::make_index_sequence<1>()), 0>;
template class Productable::ProductableContainer<3, decltype(std::make_index_sequence<2>()), 0>;
template class Productable::ProductableContainer<3, decltype(std::make_index_sequence<3>()), 0>;
template class Productable::ProductableContainer<3, decltype(std::make_index_sequence<1>()), 1>;
template class Productable::ProductableContainer<3, decltype(std::make_index_sequence<2>()), 1>;
template class Productable::ProductableContainer<3, decltype(std::make_index_sequence<3>()), 1>;
