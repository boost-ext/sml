// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_CONCEPTS_TRANSITIONAL_HPP
#define BOOST_SML_CONCEPTS_TRANSITIONAL_HPP

#include "boost/sml/aux_/type_traits.hpp"

namespace concepts {

aux::false_type transitional_impl(...);

template <class T>
auto transitional_impl(T &&t) -> aux::always<typename T::dst_state, typename T::src_state, typename T::event, typename T::deps,
                                             decltype(T::initial), decltype(T::history)>;
template <class T>
struct transitional : decltype(transitional_impl(aux::declval<T>())) {};

}  // namespace concepts

#endif
