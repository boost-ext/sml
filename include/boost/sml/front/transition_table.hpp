//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_FRONT_TRANSITION_TABLE_HPP
#define BOOST_SML_FRONT_TRANSITION_TABLE_HPP

#include "boost/sml/front/actions/defer.hpp"
#include "boost/sml/front/actions/process.hpp"
#include "boost/sml/front/event.hpp"
#include "boost/sml/front/operators.hpp"
#include "boost/sml/front/state.hpp"
#include "boost/sml/front/transition.hpp"

template <class TEvent>
detail::event<TEvent> event{};
__attribute__((unused)) static const auto on_entry = event<detail::on_entry>;
__attribute__((unused)) static const auto on_exit = event<detail::on_exit>;

template <class T = detail::_>
detail::event<detail::exception<T>> exception{};

template <class T = detail::_>
detail::event<detail::unexpected_event<T>> unexpected_event{};

template <class T, class = void>
struct state2 {
  using type = detail::state<T>;
};
template <class T>
struct state2<T, aux::enable_if_t<concepts::configurable<T>::value>> {
  using type = detail::state<detail::sm<detail::sm_policy<T>>>;
};
template <class T>
typename state2<T>::type state{};
#if !defined(_MSC_VER)
template <class T, T... Chrs>
auto operator""_s() {
  return detail::state<aux::string<Chrs...>>{};
}
template <class T, T... Chrs>
auto operator""_e() {
  return event<aux::string<Chrs...>>;
}
#endif
template <class T>
struct thread_safe : aux::pair<detail::thread_safety_policy__, thread_safe<T>> {
  using type = T;
};
template <template <class...> class T>
struct defer_queue : aux::pair<detail::defer_queue_policy__, defer_queue<T>> {
  template <class U>
  using rebind = T<U>;
};
template <class T>
struct logger : aux::pair<detail::logger_policy__, logger<T>> {
  using type = T;
};
__attribute__((unused)) static detail::state<detail::terminate_state> X;
__attribute__((unused)) static detail::history_state H;
__attribute__((unused)) static detail::defer defer;
__attribute__((unused)) static detail::process process;
template <class... Ts, __BOOST_SML_REQUIRES(aux::is_same<aux::bool_list<aux::always<Ts>::value...>,
                                                         aux::bool_list<concepts::transitional<Ts>::value...>>::value)>
auto make_transition_table(Ts... ts) {
  return aux::pool<Ts...>{ts...};
}
template <class T, class... TPolicies> /*, __BOOST_SML_REQUIRES(concepts::configurable<T>::value)*/
using sm = detail::sm<detail::sm_policy<T, TPolicies...>>;

#endif
