//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
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

template <class T, class R, class TBase, class... TArgs>
struct callable<T, R (TBase::*)(TArgs...)> : aux::true_type {};

template <class T, class R, class TBase, class... TArgs>
struct callable<T, R (TBase::*)(TArgs...) const> : aux::true_type {};

#if defined(__cpp_noexcept_function_type)  // __pph__
template <class T, class R, class TBase, class... TArgs>
struct callable<T, R (TBase::*)(TArgs...) noexcept> : aux::true_type {};

template <class T, class R, class TBase, class... TArgs>
struct callable<T, R (TBase::*)(TArgs...) const noexcept> : aux::true_type {};
#endif  // __pph__
}  // namespace concepts

#endif
