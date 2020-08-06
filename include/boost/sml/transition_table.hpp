//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_TRANSITION_TABLE_HPP
#define BOOST_SML_TRANSITION_TABLE_HPP

#include "boost/sml/aux_/type_traits.hpp"
#include "boost/sml/back/internals.hpp"
#include "boost/sml/concepts/transitional.hpp"
#include "boost/sml/front/actions/defer.hpp"
#include "boost/sml/front/actions/process.hpp"
#include "boost/sml/front/event.hpp"
#include "boost/sml/front/operators.hpp"
#include "boost/sml/front/state.hpp"
#include "boost/sml/front/transition.hpp"

using _ = back::_;

/// events

#if !(defined(_MSC_VER) && !defined(__clang__))  // __pph__
template <class TEvent>
constexpr front::event<TEvent> event{};
#else   // __pph__
template <class TEvent>
front::event<TEvent> event __BOOST_SML_VT_INIT;
#endif  // __pph__

template <class TEvent>
__BOOST_SML_UNUSED front::event<back::on_entry<_, TEvent>> on_entry __BOOST_SML_VT_INIT;

template <class TEvent>
__BOOST_SML_UNUSED front::event<back::on_exit<_, TEvent>> on_exit __BOOST_SML_VT_INIT;

template <class TEvent>
front::event<back::unexpected_event<TEvent>> unexpected_event __BOOST_SML_VT_INIT;

template <class T>
front::event<back::exception<T>> exception __BOOST_SML_VT_INIT;

using anonymous = back::anonymous;
using initial = back::initial;

/// states

#if !(defined(_MSC_VER) && !defined(__clang__))  // __pph__
template <class T>
constexpr typename front::state_sm<T>::type state{};
#else   // __pph__
template <class T>
typename front::state_sm<T>::type state __BOOST_SML_VT_INIT;
#endif  // __pph__

inline namespace literals {
#if defined(__cpp_nontype_template_parameter_class)  // __pph__
template <aux::fixed_string Str>
constexpr auto operator""_s() {
  return []<auto... Ns>(aux::index_sequence<Ns...>) { return front::state<aux::string<char, Str.data[Ns]...>>{}; }
  (aux::make_index_sequence<Str.size>{});
}
template <aux::fixed_string Str>
constexpr auto operator""_e() {
  return []<auto... Ns>(aux::index_sequence<Ns...>) { return event<aux::string<char, Str.data[Ns]...>>; }
  (aux::make_index_sequence<Str.size>{});
}
#elif !(defined(_MSC_VER) && !defined(__clang__))  // __pph__
template <class T, T... Chrs>
constexpr auto operator""_s() {
  return front::state<aux::string<T, Chrs...>>{};
}
template <class T, T... Chrs>
constexpr auto operator""_e() {
  return event<aux::string<T, Chrs...>>;
}
#endif                                             // __pph__
}  // namespace literals

__BOOST_SML_UNUSED static front::state<back::terminate_state> X;
__BOOST_SML_UNUSED static front::history_state H;

/// actions

__BOOST_SML_UNUSED static front::actions::defer defer;
__BOOST_SML_UNUSED static front::actions::process process;

/// transition table

template <class... Ts, __BOOST_SML_REQUIRES(aux::is_same<aux::bool_list<aux::always<Ts>::value...>,
                                                         aux::bool_list<concepts::transitional<Ts>::value...>>::value)>
auto make_transition_table(Ts... ts) {
  return aux::pool<Ts...>{ts...};
}

#endif
