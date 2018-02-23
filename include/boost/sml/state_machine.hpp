//
// Copyright (c) 2016-2017 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_STATE_MACHINE_HPP
#define BOOST_SML_STATE_MACHINE_HPP

#include "boost/sml/back/policies.hpp"
#include "boost/sml/back/state_machine.hpp"
#include "boost/sml/front/actions/defer.hpp"

/// policies

template <class T>
struct thread_safe : aux::pair<back::thread_safety_policy__, thread_safe<T>> {
  using type = T;
};

template <template <class...> class T>
struct defer_queue : aux::pair<back::defer_queue_policy__, defer_queue<T>> {
  template <class U>
  using rebind = T<U>;

  template <class... Ts>
  using defer = front::actions::defer_event<Ts...>;
};

template <class T>
struct logger : aux::pair<back::logger_policy__, logger<T>> {
  using type = T;
};

struct testing : aux::pair<back::testing_policy__, testing> {};

#if defined(_MSC_VER)  // __pph__
template <class T, class... TPolicies, class T__ = decltype(aux::declval<T>())>
using sm = back::sm<back::sm_policy<T__, TPolicies...>>;
#else   // __pph__
template <class T, class... TPolicies>
using sm = back::sm<back::sm_policy<T, TPolicies...>>;
#endif  // __pph__

#endif
