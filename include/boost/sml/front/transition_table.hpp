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

using _ = detail::_;

/// events

template <class TEvent>
detail::event<TEvent> event __BOOST_SML_VT_INIT;

template <class TEvent>
__BOOST_SML_UNUSED detail::event<detail::on_entry<_, TEvent>> on_entry __BOOST_SML_VT_INIT;

template <class TEvent>
__BOOST_SML_UNUSED detail::event<detail::on_exit<_, TEvent>> on_exit __BOOST_SML_VT_INIT;

template <class TEvent>
detail::event<detail::unexpected_event<TEvent>> unexpected_event __BOOST_SML_VT_INIT;

template <class T>
detail::event<detail::exception<T>> exception __BOOST_SML_VT_INIT;

using anonymous = detail::anonymous;
using initial = detail::initial;

/// states

template <class T>
typename detail::state_sm<T>::type state __BOOST_SML_VT_INIT;

#if !defined(_MSC_VER)  // __pph__
template <class T, T... Chrs>
auto operator""_s() {
  return detail::state<aux::string<Chrs...>>{};
}
template <class T, T... Chrs>
auto operator""_e() {
  return event<aux::string<Chrs...>>;
}
#endif  // __pph__

__BOOST_SML_UNUSED static detail::state<detail::terminate_state> X;
__BOOST_SML_UNUSED static detail::history_state H;

/// actions

__BOOST_SML_UNUSED static detail::defer defer;
__BOOST_SML_UNUSED static detail::process process;

/// transition table

template <class... Ts, __BOOST_SML_REQUIRES(aux::is_same<aux::bool_list<aux::always<Ts>::value...>,
                                                         aux::bool_list<concepts::transitional<Ts>::value...>>::value)>
auto make_transition_table(Ts... ts) {
  return aux::pool<Ts...>{ts...};
}

/// policies

template <class T>
struct thread_safe : aux::pair<detail::thread_safety_policy__, thread_safe<T>> {
  using type = T;
};

template <template <class...> class T>
struct defer_queue : aux::pair<detail::defer_queue_policy__, defer_queue<T>> {
  template <class U>
  using rebind = T<U>;

  template <class... Ts>
  using defer = detail::defer_event<Ts...>;
};

template <class T>
struct logger : aux::pair<detail::logger_policy__, logger<T>> {
  using type = T;
};

#endif
