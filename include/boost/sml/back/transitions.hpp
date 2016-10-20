//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_BACK_STATE_TRANSITIONS_HPP
#define BOOST_SML_BACK_STATE_TRANSITIONS_HPP

namespace detail {

template <class>
class sm_impl;

struct _ {};

template <class TException>
struct exception : internal_event {
  using type = TException;
  explicit exception(const TException& exception = {}) : exception_(exception) {}
  TException exception_;
};

template <class TEvent = _>
struct unexpected_event : internal_event {
  using type = TEvent;
  explicit unexpected_event(const TEvent& event = {}) : event_(event) {}
  TEvent event_;
};

template <class...>
struct transitions;

template <class...>
struct transitions_sub;

template <class T, class... Ts>
struct transitions<T, Ts...> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent& event, SM& sm, TDeps& deps, TSubs& subs, typename SM::state_t& current_state) {
    if (aux::get<T>(sm.transitions_).execute(event, sm, deps, subs, current_state)) {
      return true;
    }
    return transitions<Ts...>::execute(event, sm, deps, subs, current_state);
  }
};

template <class T>
struct transitions<T> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent& event, SM& sm, TDeps& deps, TSubs& subs, typename SM::state_t& current_state) {
    return aux::get<T>(sm.transitions_).execute(event, sm, deps, subs, current_state);
  }

  template <class, class SM, class TDeps, class TSubs>
  static bool execute(const on_exit& event, SM& sm, TDeps& deps, TSubs& subs, typename SM::state_t& current_state) {
    aux::get<T>(sm.transitions_).execute(event, sm, deps, subs, current_state);
    return false;  // from bottom to top
  }
};

template <>
struct transitions<> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent& event, SM& sm, TDeps& deps, TSubs& subs, typename SM::state_t& current_state) {
    sm.process_internal_event(deps, subs, unexpected_event<TEvent>{event}, current_state);
    return false;
  }
};

template <class TSM, class T, class... Ts>
struct transitions_sub<sm<TSM>, T, Ts...> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent& event, SM& sm, TDeps& deps, TSubs& subs, typename SM::state_t& current_state) {
    const auto handled = aux::try_get<sm_impl<TSM>>(&subs).process_event(event, deps, subs);
    return handled ? handled : transitions<T, Ts...>::execute(event, sm, deps, subs, current_state);
  }

  template <class, class SM, class TDeps, class TSubs>
  static bool execute(const on_entry& event, SM& sm, TDeps& deps, TSubs& subs, typename SM::state_t& current_state) {
    transitions<T, Ts...>::execute(event, sm, deps, subs, current_state);
    aux::try_get<sm_impl<TSM>>(&subs).process_event(event, deps, subs);
    return true;  // from top to bottom
  }
};

template <class TSM>
struct transitions_sub<sm<TSM>> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent& event, SM&, TDeps& deps, TSubs& subs, typename SM::state_t&) {
    aux::try_get<sm_impl<TSM>>(&subs).process_event(event, deps, subs);
    return true;
  }

  template <class, class SM, class TDeps, class TSubs>
  static bool execute(const on_entry& event, SM&, TDeps& deps, TSubs& subs, typename SM::state_t&) {
    return aux::try_get<sm_impl<TSM>>(&subs).process_event(event, deps, subs);
  }
};

}  // detail
#endif
