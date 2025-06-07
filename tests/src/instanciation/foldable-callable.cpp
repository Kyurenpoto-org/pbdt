/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include "generators/foldable-callable.hpp"

template class Foldable::FoldableCallable<0, 0, Foldable::IndexToType<0>, Foldable::IndexToType<0>>;
template class Foldable::FoldableCallable<0, 0, Foldable::IndexToType<0>, Foldable::IndexToType<1>>;
template class Foldable::FoldableCallable<0, 0, Foldable::IndexToType<1>, Foldable::IndexToType<0>>;
template class Foldable::FoldableCallable<0, 0, Foldable::IndexToType<1>, Foldable::IndexToType<1>>;
template class Foldable::FoldableCallable<0, 1, Foldable::IndexToType<0>, Foldable::IndexToType<0>>;
template class Foldable::FoldableCallable<0, 1, Foldable::IndexToType<0>, Foldable::IndexToType<1>>;
template class Foldable::FoldableCallable<0, 1, Foldable::IndexToType<1>, Foldable::IndexToType<0>>;
template class Foldable::FoldableCallable<0, 1, Foldable::IndexToType<1>, Foldable::IndexToType<1>>;
template class Foldable::FoldableCallable<1, 0, Foldable::IndexToType<0>, Foldable::IndexToType<0>>;
template class Foldable::FoldableCallable<1, 0, Foldable::IndexToType<0>, Foldable::IndexToType<1>>;
template class Foldable::FoldableCallable<1, 0, Foldable::IndexToType<1>, Foldable::IndexToType<0>>;
template class Foldable::FoldableCallable<1, 0, Foldable::IndexToType<1>, Foldable::IndexToType<1>>;
template class Foldable::FoldableCallable<1, 1, Foldable::IndexToType<0>, Foldable::IndexToType<0>>;
template class Foldable::FoldableCallable<1, 1, Foldable::IndexToType<0>, Foldable::IndexToType<1>>;
template class Foldable::FoldableCallable<1, 1, Foldable::IndexToType<1>, Foldable::IndexToType<0>>;
template class Foldable::FoldableCallable<1, 1, Foldable::IndexToType<1>, Foldable::IndexToType<1>>;
