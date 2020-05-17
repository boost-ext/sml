//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_CONCEPTS_COMPOSABLE_HPP
#define BOOST_SML_CONCEPTS_COMPOSABLE_HPP

#include "boost/sml/aux_/type_traits.hpp"
#include "boost/sml/aux_/utility.hpp"

namespace concepts {

template <class T>
decltype(aux::declval<T>().operator()()) composable_impl(int);

template <class>
void composable_impl(...);

template <class T>
struct composable : aux::is<aux::pool, decltype(composable_impl<T>(0))> {};

}  // namespace concepts

#endif
