//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_BACK_POLICIES_HPP
#define BOOST_SML_BACK_POLICIES_HPP

#include "boost/sml/aux_/utility.hpp"

namespace back {

struct no_policy {
  using type = no_policy;
  template <class>
  using rebind = no_policy;
  template <class...>
  using defer = no_policy;
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};

struct thread_safety_policy__ {};
struct defer_queue_policy__ {};
struct logger_policy__ {};
struct testing_policy__ {};

template <class, class TDeps, class TEvent>
void log_process_event(const aux::type<no_policy>&, TDeps&, const TEvent&) {}

template <class SM, class TLogger, class TDeps, class TEvent>
void log_process_event(const aux::type<TLogger>&, TDeps& deps, const TEvent& event) {
  return static_cast<aux::pool_type<TLogger&>&>(deps).value.template log_process_event<SM>(event);
}

template <class, class TDeps, class TSrcState, class TDstState>
void log_state_change(const aux::type<no_policy>&, TDeps&, const TSrcState&, const TDstState&) {}

template <class SM, class TLogger, class TDeps, class TSrcState, class TDstState>
void log_state_change(const aux::type<TLogger>&, TDeps& deps, const TSrcState& src, const TDstState& dst) {
  return static_cast<aux::pool_type<TLogger&>&>(deps).value.template log_state_change<SM>(src, dst);
}

template <class, class TDeps, class TAction, class TEvent>
void log_action(const aux::type<no_policy>&, TDeps&, const TAction&, const TEvent&) {}

template <class SM, class TLogger, class TDeps, class TAction, class TEvent>
void log_action(const aux::type<TLogger>&, TDeps& deps, const TAction& action, const TEvent& event) {
  return static_cast<aux::pool_type<TLogger&>&>(deps).value.template log_action<SM>(action, event);
}

template <class, class TDeps, class TGuard, class TEvent>
void log_guard(const aux::type<no_policy>&, TDeps&, const TGuard&, const TEvent&, bool) {}

template <class SM, class TLogger, class TDeps, class TGuard, class TEvent>
void log_guard(const aux::type<TLogger>&, TDeps& deps, const TGuard& guard, const TEvent& event, bool result) {
  return static_cast<aux::pool_type<TLogger&>&>(deps).value.template log_guard<SM>(guard, event, result);
}

}  // back

#endif
