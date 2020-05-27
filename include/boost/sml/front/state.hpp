//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_FRONT_STATE_HPP
#define BOOST_SML_FRONT_STATE_HPP

#include "boost/sml/aux_/utility.hpp"
#include "boost/sml/concepts/callable.hpp"
#include "boost/sml/concepts/composable.hpp"

namespace front {

struct initial_state {};
struct history_state {};

template <class...>
struct transition;

template <class, class>
struct transition_sa;

template <class, class>
struct transition_sg;

template <class, class>
struct transition_eg;

template <class>
struct state;

template <class TState>
struct state_impl {
  template <class T>
  auto operator<=(const T &t) const {
    return transition<TState, T>{static_cast<const TState &>(*this), t};
  }
  template <class T>
  auto operator+(const T &t) const {
    return transition<TState, T>{static_cast<const TState &>(*this), t};
  }
  template <class T, __BOOST_SML_REQUIRES(concepts::callable<bool, T>::value)>
  auto operator[](const T &t) const {
    return transition_sg<TState, aux::zero_wrapper<T>>{static_cast<const TState &>(*this), aux::zero_wrapper<T>{t}};
  }
  template <class T, __BOOST_SML_REQUIRES(concepts::callable<void, T>::value)>
  auto operator/(const T &t) const {
    return transition_sa<TState, aux::zero_wrapper<T>>{static_cast<const TState &>(*this), aux::zero_wrapper<T>{t}};
  }
};

template <class TState>
struct state : state_impl<state<TState>> {
  using type = TState;

  static constexpr auto initial = false;
  static constexpr auto history = false;

  auto operator*() const { return state<TState(initial_state)>{}; }
  auto operator()(const initial_state &) const { return state<TState(initial_state)>{}; }
  auto operator()(const history_state &) const { return state<TState(history_state)>{}; }
  template <class... Ts>
  auto operator()(const state<Ts> &...) const {
    return state<TState(Ts...)>{};
  }
  template <class T>
  auto operator=(const T &t) const {
    return transition<T, state>{t, *this};
  }
  template <class T>
  auto sm() const {
    return state<back::sm<back::sm_policy<T, aux::identity<TState>>>>{};
  }
};

template <class TState>
struct state<TState(initial_state)> : state_impl<state<TState(initial_state)>> {
  using type = TState;

  static constexpr auto initial = true;
  static constexpr auto history = false;

  template <class T>
  auto operator=(const T &t) const {
    return transition<T, state>{t, *this};
  }
};

template <class TState>
struct state<TState(history_state)> : state_impl<state<TState(history_state)>> {
  using type = TState;

  static constexpr auto initial = true;
  static constexpr auto history = true;

  template <class T>
  auto operator=(const T &t) const {
    return transition<T, state>{t, *this};
  }
};

#if defined(_MSC_VER) && !defined(__clang__)  // __pph__
template <class T, class T__ = aux::remove_reference_t<decltype(aux::declval<T>())>, class = void>
struct state_sm {
  using type = state<T>;
};

template <class T, class T__>
struct state_sm<T, T__, aux::enable_if_t<concepts::composable<T__>::value>> {
  using type = state<back::sm<back::sm_policy<T__>>>;
};
#else   // __pph__
template <class T, class = void>
struct state_sm {
  using type = state<T>;
};
template <class T>
struct state_sm<T, aux::enable_if_t<concepts::composable<T>::value>> {
  using type = state<back::sm<back::sm_policy<T>>>;
};
#endif  // __pph__

}  // namespace front

#endif
