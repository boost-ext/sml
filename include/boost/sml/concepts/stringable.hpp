//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_CONCEPTS_STRINGABLE_HPP
#define BOOST_SML_CONCEPTS_STRINGABLE_HPP

#include "boost/sml/aux_/type_traits.hpp"

namespace concepts {
template <class T, class = decltype(T::c_str())>
aux::true_type test_stringable(const T &);
aux::false_type test_stringable(...);
template <class T, class = void>
struct stringable : aux::false_type {};
template <class T>
struct stringable<T, decltype(void(sizeof(T)))> : decltype(test_stringable(aux::declval<T>())) {};
}  // concepts

#endif
