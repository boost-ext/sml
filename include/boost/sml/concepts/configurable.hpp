//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_CONCEPTS_CONFIGURABLE_HPP
#define BOOST_SML_CONCEPTS_CONFIGURABLE_HPP

#include "boost/sml/aux_/type_traits.hpp"
#include "boost/sml/aux_/utility.hpp"

namespace concepts {

template <class T>
decltype(aux::declval<T>()()) configurable_impl(int);

template <class>
void configurable_impl(...);

template <class T>
struct configurable : aux::is_pool<decltype(configurable_impl<T>(0))> {};

}  // concepts

#endif
