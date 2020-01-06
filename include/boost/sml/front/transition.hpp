//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_FRONT_TRANSITION_HPP
#define BOOST_SML_FRONT_TRANSITION_HPP

#include "boost/sml/aux_/type_traits.hpp"
#include "boost/sml/aux_/utility.hpp"
#include "boost/sml/back/internals.hpp"
#include "boost/sml/front/state.hpp"

namespace front {

struct internal {};

template <class, class>
struct ignore;

template <class E, class... Ts>
struct ignore<E, aux::type_list<Ts...>> {
  template <class T>
  struct non_events {
    using type =
        aux::conditional_t<aux::is_same<back::get_event_t<E>, aux::remove_const_t<aux::remove_reference_t<T>>>::value ||
                               aux::is_same<T, action_base>::value,
                           aux::type_list<>, aux::type_list<T>>;
  };

  using type = aux::join_t<typename non_events<Ts>::type...>;
};

template <class T, class E, class = void>
struct get_deps {
  using type = typename ignore<E, args_t<T, E>>::type;
};

template <class T, class E>
using get_deps_t = typename get_deps<T, E>::type;

template <template <class...> class T, class... Ts, class E>
struct get_deps<T<Ts...>, E, aux::enable_if_t<aux::is_base_of<operator_base, T<Ts...>>::value>> {
  using type = aux::join_t<get_deps_t<Ts, E>...>;
};

struct always {
  using type = always;
  bool operator()() const { return true; }
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};

struct none {
  using type = none;
  void operator()() {}
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};

template <class...>
struct transition;

template <class E, class G>
struct transition<front::event<E>, G> {
  template <class T>
  auto operator/(const T &t) const {
    return transition<front::event<E>, G, aux::zero_wrapper<T>>{e, g, aux::zero_wrapper<T>{t}};
  }
  front::event<E> e;
  G g;
};

template <class E, class G, class A>
struct transition<front::event<E>, G, A> {
  front::event<E> e;
  G g;
  A a;
};

template <class S2, class G, class A>
struct transition<state<S2>, G, A> : transition<state<internal>, state<S2>, front::event<back::anonymous>, G, A> {
  using transition<state<internal>, state<S2>, front::event<back::anonymous>, G, A>::g;
  using transition<state<internal>, state<S2>, front::event<back::anonymous>, G, A>::a;
  transition(const G &g, const A &a) : transition<state<internal>, state<S2>, front::event<back::anonymous>, G, A>{g, a} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<back::anonymous>, G, A>{g, a};
  }
};

template <class S1, class S2>
struct transition<state<S1>, state<S2>> : transition<state<S1>, state<S2>, front::event<back::anonymous>, always, none> {
  transition(const state<S1> &, const state<S2> &)
      : transition<state<S1>, state<S2>, front::event<back::anonymous>, always, none>{always{}, none{}} {}
};

template <class S2, class G>
struct transition_sg<state<S2>, G> : transition<state<internal>, state<S2>, front::event<back::anonymous>, G, none> {
  using transition<state<internal>, state<S2>, front::event<back::anonymous>, G, none>::g;
  transition_sg(const state<S2> &, const G &g)
      : transition<state<internal>, state<S2>, front::event<back::anonymous>, G, none>{g, none{}} {}
  template <class T>
  auto operator/(const T &t) const {
    return transition<state<S2>, G, aux::zero_wrapper<T>>{g, aux::zero_wrapper<T>{t}};
  }
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<back::anonymous>, G, none>{g, none{}};
  }
};

template <class S2, class A>
struct transition_sa<state<S2>, A> : transition<state<internal>, state<S2>, front::event<back::anonymous>, always, A> {
  using transition<state<internal>, state<S2>, front::event<back::anonymous>, always, A>::a;
  transition_sa(const state<S2> &, const A &a)
      : transition<state<internal>, state<S2>, front::event<back::anonymous>, always, A>{always{}, a} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<back::anonymous>, always, A>{always{}, a};
  }
};

template <class S2, class E>
struct transition<state<S2>, front::event<E>> {
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<E>, always, none>{always{}, none{}};
  }
  const state<S2> &s2;
  front::event<E> e;
};

template <class E, class G>
struct transition_eg<front::event<E>, G> {
  template <class T>
  auto operator/(const T &t) const {
    return transition<front::event<E>, G, aux::zero_wrapper<T>>{e, g, aux::zero_wrapper<T>{t}};
  }
  front::event<E> e;
  G g;
};

template <class E, class A>
struct transition_ea<front::event<E>, A> {
  front::event<E> e;
  A a;
};

template <class S1, class S2, class G, class A>
struct transition<state<S1>, transition<state<S2>, G, A>>
    : transition<state<S1>, state<S2>, front::event<back::anonymous>, G, A> {
  transition(const state<S1> &, const transition<state<S2>, G, A> &t)
      : transition<state<S1>, state<S2>, front::event<back::anonymous>, G, A>{t.g, t.a} {}
};

template <class S1, class E, class G, class A>
struct transition<state<S1>, transition<front::event<E>, G, A>>
    : transition<state<internal>, state<S1>, front::event<E>, G, A> {
  using transition<state<internal>, state<S1>, front::event<E>, G, A>::g;
  using transition<state<internal>, state<S1>, front::event<E>, G, A>::a;
  transition(const state<S1> &, const transition<front::event<E>, G, A> &t)
      : transition<state<internal>, state<S1>, front::event<E>, G, A>{t.g, t.a} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S1>, front::event<E>, G, A>{g, a};
  }
};

template <class S1, class S2, class E>
struct transition<state<S1>, transition<state<S2>, front::event<E>>>
    : transition<state<S1>, state<S2>, front::event<E>, always, none> {
  transition(const state<S1> &, const transition<state<S2>, front::event<E>> &)
      : transition<state<S1>, state<S2>, front::event<E>, always, none>{always{}, none{}} {}
};

template <class S1, class S2, class G>
struct transition<state<S1>, transition_sg<state<S2>, G>>
    : transition<state<S1>, state<S2>, front::event<back::anonymous>, G, none> {
  transition(const state<S1> &, const transition_sg<state<S2>, G> &t)
      : transition<state<S1>, state<S2>, front::event<back::anonymous>, G, none>{t.g, none{}} {}
};

template <class S1, class S2, class A>
struct transition<state<S1>, transition_sa<state<S2>, A>>
    : transition<state<S1>, state<S2>, front::event<back::anonymous>, always, A> {
  transition(const state<S1> &, const transition_sa<state<S2>, A> &t)
      : transition<state<S1>, state<S2>, front::event<back::anonymous>, always, A>{always{}, t.a} {}
};

template <class S2, class E, class G>
struct transition<state<S2>, transition_eg<front::event<E>, G>>
    : transition<state<internal>, state<S2>, front::event<E>, G, none> {
  using transition<state<internal>, state<S2>, front::event<E>, G, none>::g;
  transition(const state<S2> &, const transition_eg<front::event<E>, G> &t)
      : transition<state<internal>, state<S2>, front::event<E>, G, none>{t.g, none{}} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<E>, G, none>{g, none{}};
  }
};

template <class S1, class S2, class E, class G>
struct transition<state<S1>, transition<state<S2>, transition_eg<front::event<E>, G>>>
    : transition<state<S1>, state<S2>, front::event<E>, G, none> {
  transition(const state<S1> &, const transition<state<S2>, transition_eg<front::event<E>, G>> &t)
      : transition<state<S1>, state<S2>, front::event<E>, G, none>{t.g, none{}} {}
};

template <class S2, class E, class A>
struct transition<state<S2>, transition_ea<front::event<E>, A>>
    : transition<state<internal>, state<S2>, front::event<E>, always, A> {
  using transition<state<internal>, state<S2>, front::event<E>, always, A>::a;
  transition(const state<S2> &, const transition_ea<front::event<E>, A> &t)
      : transition<state<internal>, state<S2>, front::event<E>, always, A>{always{}, t.a} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<E>, always, A>{always{}, a};
  }
};

template <class S1, class S2, class E, class A>
struct transition<state<S1>, transition<state<S2>, transition_ea<front::event<E>, A>>>
    : transition<state<S1>, state<S2>, front::event<E>, always, A> {
  transition(const state<S1> &, const transition<state<S2>, transition_ea<front::event<E>, A>> &t)
      : transition<state<S1>, state<S2>, front::event<E>, always, A>{always{}, t.a} {}
};

template <class S1, class S2, class E, class G, class A>
struct transition<state<S1>, transition<state<S2>, transition<front::event<E>, G, A>>>
    : transition<state<S1>, state<S2>, front::event<E>, G, A> {
  transition(const state<S1> &, const transition<state<S2>, transition<front::event<E>, G, A>> &t)
      : transition<state<S1>, state<S2>, front::event<E>, G, A>{t.g, t.a} {}
};

template <class T, class TSubs, class... Ts, class... THs>
void update_composite_states(TSubs &subs, aux::true_type, const aux::type_list<THs...> &) {
  using state_t = typename T::state_t;
  auto &sm = back::sub_sm<T>::get(&subs);
  (void)sm;
#if defined(__cpp_fold_expressions)  // __pph__
  ((sm.current_state_[aux::get_id<state_t, THs>((typename T::initial_states_ids_t *)0)] =
        aux::get_id<state_t, THs>((typename T::states_ids_t *)0)),
   ...);
#else   // __pph__
  (void)aux::swallow{0, (sm.current_state_[aux::get_id<state_t, THs>((typename T::initial_states_ids_t *)0)] =
                             aux::get_id<state_t, THs>((typename T::states_ids_t *)0),
                         0)...};
#endif  // __pph__
}

template <class T, class TSubs, class... Ts>
void update_composite_states(TSubs &subs, aux::false_type, Ts &&...) {
  back::sub_sm<T>::get(&subs).initialize(typename T::initial_states_t{});
}

template <class SM, class TDeps, class TSubs, class TSrcState, class TDstState>
void update_current_state(SM &, TDeps &deps, TSubs &, typename SM::state_t &current_state,
                          const typename SM::state_t &new_state, const TSrcState &, const TDstState &) {
  back::policies::log_state_change<typename SM::sm_t>(aux::type<typename SM::logger_t>{}, deps,
                                                      aux::string<typename TSrcState::type>{},
                                                      aux::string<typename TDstState::type>{});
  current_state = new_state;
}

template <class SM, class TDeps, class TSubs, class TSrcState, class T>
void update_current_state(SM &, TDeps &deps, TSubs &subs, typename SM::state_t &current_state,
                          const typename SM::state_t &new_state, const TSrcState &, const state<back::sm<T>> &) {
  back::policies::log_state_change<typename SM::sm_t>(aux::type<typename SM::logger_t>{}, deps,
                                                      aux::string<typename TSrcState::type>{}, aux::string<T>{});
  current_state = new_state;
  update_composite_states<back::sm_impl<T>>(subs, typename back::sm_impl<T>::has_history_states{},
                                            typename back::sm_impl<T>::history_states_t{});
}

template <class S1, class S2, class E, class G, class A>
struct transition<state<S1>, state<S2>, front::event<E>, G, A> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;

  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = G;
  using action = A;
  using deps = aux::apply_t<aux::unique_t, aux::join_t<get_deps_t<G, E>, get_deps_t<A, E>>>;

  transition(const G &g, const A &a) : g(g), a(a) {}

  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::true_type) {
    if (call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs)) {
      sm.process_internal_event(back::on_exit<back::_, TEvent>{event}, deps, subs, current_state);
      update_current_state(sm, deps, subs, current_state,
                           aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                           state<dst_state>{});
      call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
      sm.process_internal_event(back::on_entry<back::_, TEvent>{event}, deps, subs, current_state);
      return true;
    }
    return false;
  }

  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::false_type) {
    if (call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs)) {
      update_current_state(sm, deps, subs, current_state,
                           aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                           state<dst_state>{});
      call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
      return true;
    }
    return false;
  }

  G g;
  A a;
};

template <class S2, class E, class G, class A>
struct transition<state<internal>, state<S2>, front::event<E>, G, A> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;

  using src_state = typename state<S2>::type;
  using dst_state = internal;
  using event = E;
  using guard = G;
  using action = A;
  using deps = aux::apply_t<aux::unique_t, aux::join_t<get_deps_t<G, E>, get_deps_t<A, E>>>;

  transition(const G &g, const A &a) : g(g), a(a) {}

  template <class TEvent, class SM, class TDeps, class TSubs, class... Ts>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &, Ts &&...) {
    if (call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs)) {
      call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
      return true;
    }
    return false;
  }

  G g;
  A a;
};

template <class S1, class S2, class E, class A>
struct transition<state<S1>, state<S2>, front::event<E>, always, A> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;

  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = always;
  using action = A;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<A, E>>;

  transition(const always &, const A &a) : a(a) {}

  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::true_type) {
    sm.process_internal_event(back::on_exit<back::_, TEvent>{event}, deps, subs, current_state);
    update_current_state(sm, deps, subs, current_state,
                         aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                         state<dst_state>{});
    call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
    sm.process_internal_event(back::on_entry<back::_, TEvent>{event}, deps, subs, current_state);
    return true;
  }

  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::false_type) {
    update_current_state(sm, deps, subs, current_state,
                         aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                         state<dst_state>{});
    call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
    return true;
  }

  A a;
};

template <class S2, class E, class A>
struct transition<state<internal>, state<S2>, front::event<E>, always, A> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;

  using src_state = typename state<S2>::type;
  using dst_state = internal;
  using event = E;
  using guard = always;
  using action = A;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<A, E>>;

  transition(const always &, const A &a) : a(a) {}

  template <class TEvent, class SM, class TDeps, class TSubs, class... Ts>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &, Ts &&...) {
    call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
    return true;
  }

  A a;
};

template <class S1, class S2, class E, class G>
struct transition<state<S1>, state<S2>, front::event<E>, G, none> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;

  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = G;
  using action = none;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<G, E>>;

  transition(const G &g, const none &) : g(g) {}

  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::true_type) {
    if (call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs)) {
      sm.process_internal_event(back::on_exit<back::_, TEvent>{event}, deps, subs, current_state);
      update_current_state(sm, deps, subs, current_state,
                           aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                           state<dst_state>{});
      sm.process_internal_event(back::on_entry<back::_, TEvent>{event}, deps, subs, current_state);
      return true;
    }
    return false;
  }

  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::false_type) {
    if (call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs)) {
      update_current_state(sm, deps, subs, current_state,
                           aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                           state<dst_state>{});
      return true;
    }
    return false;
  }

  G g;
};

template <class S2, class E, class G>
struct transition<state<internal>, state<S2>, front::event<E>, G, none> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;

  using src_state = typename state<S2>::type;
  using dst_state = internal;
  using event = E;
  using guard = G;
  using action = none;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<G, E>>;

  transition(const G &g, const none &) : g(g) {}

  template <class TEvent, class SM, class TDeps, class TSubs, class... Ts>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &, Ts &&...) {
    return call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs);
  }

  G g;
};

template <class S1, class S2, class E>
struct transition<state<S1>, state<S2>, front::event<E>, always, none> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;

  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = always;
  using action = none;
  using deps = aux::type_list<>;

  transition(const always &, const none &) {}

  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::true_type) {
    sm.process_internal_event(back::on_exit<back::_, TEvent>{event}, deps, subs, current_state);
    update_current_state(sm, deps, subs, current_state,
                         aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                         state<dst_state>{});
    sm.process_internal_event(back::on_entry<back::_, TEvent>{event}, deps, subs, current_state);
    return true;
  }

  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::false_type) {
    update_current_state(sm, deps, subs, current_state,
                         aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                         state<dst_state>{});
    return true;
  }

  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};

template <class S2, class E>
struct transition<state<internal>, state<S2>, front::event<E>, always, none> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;

  using src_state = typename state<S2>::type;
  using dst_state = internal;
  using event = E;
  using guard = always;
  using action = none;
  using deps = aux::type_list<>;

  transition(const always &, const none &) {}

  template <class TEvent, class SM, class TDeps, class TSubs, class... Ts>
  bool execute(const TEvent &, SM &, TDeps &, TSubs &, typename SM::state_t &, Ts &&...) {
    return true;
  }

  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};

}  // namespace front

#endif
