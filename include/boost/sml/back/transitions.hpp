//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_BACK_STATE_TRANSITIONS_HPP
#define BOOST_SML_BACK_STATE_TRANSITIONS_HPP

#include "boost/sml/aux_/utility.hpp"

namespace back {

template <class>
class sm;

template <class>
struct sm_impl;

template <class...>
struct transitions;

template <class...>
struct transitions_sub;

template <class T, class... Ts>
struct transitions<T, Ts...> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent& event, SM& sm, TDeps& deps, TSubs& subs, typename SM::state_t& current_state) {
    if (aux::get<T>(sm.transitions_).execute(event, sm, deps, subs, current_state, typename SM::has_entry_exits{})) {
      return true;
    }
    return transitions<Ts...>::execute(event, sm, deps, subs, current_state);
  }
};

template <class T>
struct transitions<T> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent& event, SM& sm, TDeps& deps, TSubs& subs, typename SM::state_t& current_state) {
    return execute_impl(event, sm, deps, subs, current_state);
  }

  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute_impl(const TEvent& event, SM& sm, TDeps& deps, TSubs& subs, typename SM::state_t& current_state) {
    return aux::get<T>(sm.transitions_).execute(event, sm, deps, subs, current_state, typename SM::has_entry_exits{});
  }

  template <class _, class TEvent, class SM, class TDeps, class TSubs>
  static bool execute_impl(const on_exit<_, TEvent>& event, SM& sm, TDeps& deps, TSubs& subs,
                           typename SM::state_t& current_state) {
    aux::get<T>(sm.transitions_).execute(event, sm, deps, subs, current_state, typename SM::has_entry_exits{});
    return false;  // from bottom to top
  }
};

template <>
struct transitions<aux::true_type> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent& event, SM& sm, TDeps& deps, TSubs& subs, typename SM::state_t& current_state) {
    sm.process_internal_event(unexpected_event<TEvent>{event}, deps, subs, current_state);
    return false;
  }
};

template <>
struct transitions<aux::false_type> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent&, SM&, TDeps&, TSubs&, typename SM::state_t&) {
    return false;
  }
};

template <class TSM, class T, class... Ts>
struct transitions_sub<sm<TSM>, T, Ts...> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent& event, SM& sm, TDeps& deps, TSubs& subs, typename SM::state_t& current_state) {
    return execute_impl(event, sm, deps, subs, current_state);
  }

  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute_impl(const TEvent& event, SM& sm, TDeps& deps, TSubs& subs, typename SM::state_t& current_state) {
    const auto handled = aux::get<sm_impl<TSM>>(subs).process_event(event, deps, subs);
    return handled ? handled : transitions<T, Ts...>::execute(event, sm, deps, subs, current_state);
  }

  template <class _, class TEvent, class SM, class TDeps, class TSubs>
  static bool execute_impl(const back::on_entry<_, TEvent>& event, SM& sm, TDeps& deps, TSubs& subs,
                           typename SM::state_t& current_state) {
    transitions<T, Ts...>::execute(event, sm, deps, subs, current_state);
    aux::get<sm_impl<TSM>>(subs).process_event(event, deps, subs);
    return true;  // from top to bottom
  }
};

template <class TSM>
struct transitions_sub<sm<TSM>> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent& event, SM&, TDeps& deps, TSubs& subs, typename SM::state_t&) {
    return execute_impl(event, deps, subs);
  }

  template <class TEvent, class TDeps, class TSubs>
  static bool execute_impl(const TEvent& event, TDeps& deps, TSubs& subs) {
    aux::get<sm_impl<TSM>>(subs).template process_event<TEvent>(event, deps, subs);
    return true;
  }

  template <class _, class TEvent, class TDeps, class TSubs>
  static bool execute_impl(const on_entry<_, TEvent>& event, TDeps& deps, TSubs& subs) {
    return aux::get<sm_impl<TSM>>(subs).process_event(event, deps, subs);
  }
};

}  // back
#endif
