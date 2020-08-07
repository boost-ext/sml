//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
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

using testing = back::policies::testing;
template <class T>
using logger = back::policies::logger<T>;
template <class T>
using thread_safe = back::policies::thread_safe<T>;
template <class T>
using dispatch = back::policies::dispatch<T>;
template <template <class...> class T>
using defer_queue = back::policies::defer_queue<T>;
template <template <class...> class T>
using process_queue = back::policies::process_queue<T>;

/// state machine

#if defined(_MSC_VER) && !defined(__clang__)  // __pph__
template <class T, class... TPolicies, class T__ = aux::remove_reference_t<decltype(aux::declval<T>())>>
using sm = back::sm<back::sm_policy<T__, TPolicies...>>;
#else   // __pph__
template <class T, class... TPolicies>
using sm = back::sm<back::sm_policy<T, TPolicies...>>;
#endif  // __pph__

#if defined(__cpp_deduction_guides)  // __pph__
namespace front {
template <class T, class... TPolicies>
struct sm : back::sm<back::sm_policy<T, TPolicies...>> {
  constexpr sm(T t) : back::sm<back::sm_policy<T, TPolicies...>>::sm{t} {}
  using back::sm<back::sm_policy<T, TPolicies...>>::sm;
};
}  // namespace front
#endif  // __pph__

#endif
