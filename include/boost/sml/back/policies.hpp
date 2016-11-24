//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_BACK_POLICIES_HPP
#define BOOST_SML_BACK_POLICIES_HPP

#include "boost/sml/aux_/utility.hpp"
#include "boost/sml/back/utility.hpp"

namespace back {

template <class>
class sm;

template <class, class...>
struct sm_policy;

struct thread_safety_policy__ {};
struct defer_queue_policy__ {};
struct logger_policy__ {};
struct testing_policy__ {};

struct no_policy {
  using type = no_policy;
  template <class>
  using rebind = no_policy;
  template <class...>
  using defer = no_policy;
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};

template <class>
no_policy get_policy(...);

template <class T, class TPolicy>
TPolicy get_policy(aux::pair<T, TPolicy>*);

template <class, class...>
struct sm_policy;

template <class SM, class... TPolicies>
struct sm_policy {
  using sm = SM;
  using thread_safety_policy = decltype(get_policy<thread_safety_policy__>((aux::inherit<TPolicies...>*)0));
  using defer_queue_policy = decltype(get_policy<defer_queue_policy__>((aux::inherit<TPolicies...>*)0));
  using logger_policy = decltype(get_policy<logger_policy__>((aux::inherit<TPolicies...>*)0));
  using testing_policy = decltype(get_policy<testing_policy__>((aux::inherit<TPolicies...>*)0));
  template <class T>
  using rebind = typename rebind_impl<T, TPolicies...>::type;
};

template <class...>
struct state_machine;

template <class>
struct get_state_name;

template <class T>
struct get_state_name<aux::string<T>> {
  using type = aux::string<T>;
};

template <class T, class... Ts>
struct get_state_name<aux::string<sm<sm_policy<T, Ts...>>>> {
  using type = typename get_state_name<aux::string<sm_policy<T, Ts...>>>::type;
};

template <class T>
struct get_state_name<aux::string<sm_policy<T>>> {
  using type = aux::string<T>;
};

template <class T, class TName>
struct get_state_name<aux::string<sm_policy<T, TName>>> {
  using type = aux::string<T(typename TName::type)>;
};

template <class T>
using get_state_name_t = typename get_state_name<T>::type;

template <class, class TDeps, class TEvent>
void log_process_event(const aux::type<no_policy>&, TDeps&, const TEvent&) {}

template <class SM, class TLogger, class TDeps, class TEvent>
void log_process_event(const aux::type<TLogger>&, TDeps& deps, const TEvent& event) {
  return static_cast<aux::pool_type<TLogger&>&>(deps).value.template log_process_event<SM>(event);
}

template <class, class TDeps, class TSrcState, class TDstState>
void log_state_change(const aux::type<no_policy>&, TDeps&, const TSrcState&, const TDstState&) {}

template <class SM, class TLogger, class TDeps, class TSrcState, class TDstState>
void log_state_change(const aux::type<TLogger>&, TDeps& deps, const TSrcState&, const TDstState&) {
  return static_cast<aux::pool_type<TLogger&>&>(deps).value.template log_state_change<SM>(get_state_name_t<TSrcState>{},
                                                                                          get_state_name_t<TDstState>{});
}

template <class, class TDeps, class TAction, class TEvent>
void log_action(const aux::type<no_policy>&, TDeps&, const TAction&, const TEvent&) {}

template <class SM, class TLogger, class TDeps, class TAction, class TEvent>
void log_action(const aux::type<TLogger>&, TDeps& deps, const TAction& action, const TEvent& event) {
  return static_cast<aux::pool_type<TLogger&>&>(deps).value.template log_action<SM>(action, event);
}

template <class SM, class TLogger, class TDeps, class TAction, class TEvent>
void log_action(const aux::type<TLogger>&, TDeps& deps, const aux::zero_wrapper<TAction>& action, const TEvent& event) {
  return static_cast<aux::pool_type<TLogger&>&>(deps).value.template log_action<SM>(action.get(), event);
}

template <class, class TDeps, class TGuard, class TEvent>
void log_guard(const aux::type<no_policy>&, TDeps&, const TGuard&, const TEvent&, bool) {}

template <class SM, class TLogger, class TDeps, class TGuard, class TEvent>
void log_guard(const aux::type<TLogger>&, TDeps& deps, const TGuard& guard, const TEvent& event, bool result) {
  return static_cast<aux::pool_type<TLogger&>&>(deps).value.template log_guard<SM>(guard, event, result);
}

template <class SM, class TLogger, class TDeps, class TGuard, class TEvent>
void log_guard(const aux::type<TLogger>&, TDeps& deps, const aux::zero_wrapper<TGuard>& guard, const TEvent& event,
               bool result) {
  return static_cast<aux::pool_type<TLogger&>&>(deps).value.template log_guard<SM>(guard.get(), event, result);
}

}  // back

#endif
