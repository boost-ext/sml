//
// Copyright (c) 2016-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_CONCEPTS_CALLABLE_HPP
#define BOOST_SML_CONCEPTS_CALLABLE_HPP

#include "boost/sml/aux_/type_traits.hpp"

namespace concepts {

struct callable_fallback {
  void operator()();
};

template <class T>
aux::false_type test_callable(aux::non_type<void (callable_fallback::*)(), &T::operator()> *);

template <class>
aux::true_type test_callable(...);

template <class, class T>
struct callable
    : decltype(test_callable<aux::inherit<aux::conditional_t<__is_class(T), T, aux::none_type>, callable_fallback>>(0)) {};

}  // namespace concepts

#endif
