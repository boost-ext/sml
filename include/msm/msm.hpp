//
// Copyright (c) 2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once
namespace msm {
inline namespace v_1_0_0 {
namespace aux {
struct none_type {};
template <class...>
struct type {};
template <class...>
struct type_list {
  using type = type_list;
};
template <class... Ts>
struct inherit : Ts... {
  using type = inherit;
};
template <class T>
T &&declval();
template <class T, T V>
struct integral_constant {
  using type = integral_constant;
  static constexpr T value = V;
};
using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;
template <bool B, class T, class F>
struct conditional {
  using type = T;
};
template <class T, class F>
struct conditional<false, T, F> {
  using type = F;
};
template <bool B, class T, class F>
using conditional_t = typename conditional<B, T, F>::type;
template <bool B, class T = void>
struct enable_if {};
template <class T>
struct enable_if<true, T> {
  using type = T;
};
template <bool B, class T = void>
using enable_if_t = typename enable_if<B, T>::type;
template <class, class>
struct is_same : false_type {};
template <class T>
struct is_same<T, T> : true_type {};
template <class T, class U>
struct is_base_of : integral_constant<bool, __is_base_of(T, U)> {};
template <class T>
struct remove_nextualifiers {
  using type = T;
};
template <class T>
struct remove_nextualifiers<const T> {
  using type = T;
};
template <class T>
struct remove_nextualifiers<T &> {
  using type = T;
};
template <class T>
struct remove_nextualifiers<const T &> {
  using type = T;
};
template <class T>
struct remove_nextualifiers<T *> {
  using type = T;
};
template <class T>
struct remove_nextualifiers<const T *> {
  using type = T;
};
template <class T>
struct remove_nextualifiers<T *const &> {
  using type = T;
};
template <class T>
struct remove_nextualifiers<T *const> {
  using type = T;
};
template <class T>
struct remove_nextualifiers<const T *const> {
  using type = T;
};
template <class T>
struct remove_nextualifiers<T &&> {
  using type = T;
};
template <class T>
using remove_nextualifiers_t = typename remove_nextualifiers<T>::type;
template <class>
struct function_traits;
template <class R, class... TArgs>
struct function_traits<R (*)(TArgs...)> {
  using args = type_list<TArgs...>;
};
template <class R, class... TArgs>
struct function_traits<R(TArgs...)> {
  using args = type_list<TArgs...>;
};
template <class R, class T, class... TArgs>
struct function_traits<R (T::*)(TArgs...)> {
  using args = type_list<TArgs...>;
};
template <class R, class T, class... TArgs>
struct function_traits<R (T::*)(TArgs...) const> {
  using args = type_list<TArgs...>;
};
template <class T>
using function_traits_t = typename function_traits<T>::args;
template <int...>
struct index_sequence {
  using type = index_sequence;
};
template <class, class>
struct concat;
template <int... I1, int... I2>
struct concat<index_sequence<I1...>, index_sequence<I2...>> : index_sequence<I1..., (sizeof...(I1) + I2)...> {};
template <int N>
struct make_index_sequence_impl
    : concat<typename make_index_sequence_impl<N / 2>::type, typename make_index_sequence_impl<N - N / 2>::type>::type {
};
template <>
struct make_index_sequence_impl<0> : index_sequence<> {};
template <>
struct make_index_sequence_impl<1> : index_sequence<1> {};
template <int N>
using make_index_sequence = typename make_index_sequence_impl<N>::type;
template <class...>
struct join;
template <>
struct join<> {
  using type = type_list<>;
};
template <class... TArgs>
struct join<type_list<TArgs...>> {
  using type = type_list<TArgs...>;
};
template <class... TArgs1, class... TArgs2>
struct join<type_list<TArgs1...>, type_list<TArgs2...>> {
  using type = type_list<TArgs1..., TArgs2...>;
};
template <class... TArgs1, class... TArgs2, class... Ts>
struct join<type_list<TArgs1...>, type_list<TArgs2...>, Ts...> {
  using type = typename join<type_list<TArgs1..., TArgs2...>, Ts...>::type;
};
template <class... TArgs>
using join_t = typename join<TArgs...>::type;
template <class, class...>
struct uninextue_impl;
template <class T1, class T2, class... Rs, class... Ts>
struct uninextue_impl<type<T1, Rs...>, T2, Ts...>
    : conditional_t<is_base_of<type<T2>, T1>::value, uninextue_impl<type<inherit<T1>, Rs...>, Ts...>,
                    uninextue_impl<type<inherit<T1, type<T2>>, Rs..., T2>, Ts...>> {};
template <class T1, class... Rs>
struct uninextue_impl<type<T1, Rs...>> : type_list<Rs...> {};
template <class... Ts>
struct uninextue : uninextue_impl<type<none_type>, Ts...> {};
template <class T>
struct uninextue<T> : type_list<T> {};
template <class... Ts>
using unique_t = typename uninextue<Ts...>::type;
template <template <class...> class, class>
struct apply;
template <template <class...> class T, template <class...> class U, class... Ts>
struct apply<T, U<Ts...>> : T<Ts...> {};
template <template <class...> class T, class D>
using apply_t = typename apply<T, D>::type;
template <int, class T>
struct pool_type {
  using type = T;
  type object;
};
template <int N, class R, class... Ts>
struct pool_type<N, R(Ts...)> {
  using type = R (*)(Ts...);
  type object;
};
template <class, class...>
struct pool_impl;
template <int... Ns, class... Ts>
struct pool_impl<index_sequence<Ns...>, Ts...> : pool_type<Ns, Ts>... {
  explicit pool_impl(Ts... ts) : pool_type<Ns, Ts>{ts}... {}
};
template <int N, class T>
decltype(auto) get_impl_nr(pool_type<N, T> *object) noexcept {
  return static_cast<pool_type<N, T> &>(*object).object;
}
template <int N, class TPool>
decltype(auto) get(TPool &p) noexcept {
  return get_impl_nr<N + 1>(&p);
}
template <class T, int N>
decltype(auto) get_impl_type(pool_type<N, T> *object) noexcept {
  return static_cast<pool_type<N, T> &>(*object).object;
}
template <class T, class TPool>
decltype(auto) get(TPool &p) noexcept {
  return get_impl_type<T>(&p);
}
template <class... Ts>
struct pool : pool_impl<make_index_sequence<sizeof...(Ts)>, Ts...> {
  using type = pool;
  using pool_impl<make_index_sequence<sizeof...(Ts)>, Ts...>::pool_impl;
};
template <int, class T>
struct type_id_type {};
template <class, class...>
struct type_id_impl;
template <int... Ns, class... Ts>
struct type_id_impl<index_sequence<Ns...>, Ts...> : type_id_type<Ns, Ts>... {};
template <class... Ts>
struct type_id : type_id_impl<make_index_sequence<sizeof...(Ts)>, Ts...> {
  using type = type_id;
};
template <class T, int, int N>
constexpr auto get_id_impl(type_id_type<N, T> *) noexcept {
  return N;
}
template <class T, int D>
constexpr auto get_id_impl(...) noexcept {
  return D;
}
template <class TIds, int D, class T>
constexpr auto get_id() noexcept {
  return get_id_impl<T, D>((TIds *)0);
}
template <class>
struct get_size;
template <template <class...> class T, class... Ts>
struct get_size<T<Ts...>> {
  static constexpr auto value = sizeof...(Ts);
};
template <class...>
struct sum_up;
template <class T, class... Ts>
struct sum_up<T, Ts...> : aux::integral_constant<int, T::value + sum_up<Ts...>::value> {};
template <class T>
struct sum_up<T> : aux::integral_constant<int, T::value> {};
template <>
struct sum_up<> : aux::integral_constant<int, 0> {};
}  // aux

struct operator_base {};
struct state_base {};
struct init_state_base {};
struct init_base {
} init;
struct self {};
struct anonymous {
  static constexpr auto id = -1;
  anonymous(...) {}
};
struct not_handled {
  static constexpr auto id = -2;
  not_handled(...) {}
};
struct on_entry {
  static constexpr auto id = -3;
  on_entry(...) {}
};
struct on_exit {
  static constexpr auto id = -4;
  on_exit(...) {}
};
struct always {
  auto operator()() { return true; }
};
struct none {
  void operator()() {}
};

template <class... Ts>
auto make_transition_table(Ts... ts) noexcept {
  return aux::pool<Ts...>{ts...};
}

template <class, class>
class sm_impl;

template <class...>
struct transition;
template <class, class>
struct transition_sg;
template <class, class>
struct transition_sa;
template <class, class>
struct transition_eg;
template <class, class>
struct transition_ea;

template <class T>
struct inherit {
  using type = T;
};
template <class... Ts>
struct inherit<sm_impl<Ts...>> {
  using type = aux::none_type;
};
template <class T>
using inherit_t = typename inherit<T>::type;

template <class>
struct state_properties {};
template <template <class...> class TState, class... Ts>
struct state_properties<TState<Ts...>> : inherit_t<Ts>... {};
template <class...>
struct init_state;

template <class TState>
struct state_impl : state_properties<TState>, state_base {
  template <class T>
  auto operator==(const T &t) const noexcept {
    return transition<state_impl, T>{*this, t};
  }

  template <class T>
  auto operator+(const T &t) const noexcept {
    return transition<state_impl, T>{*this, t};
  }

  template <class T>
  auto operator[](const T &t) const noexcept {
    return transition_sg<state_impl, T>{*this, t};
  }

  template <class T>
  auto operator/(const T &t) const noexcept {
    return transition_sa<state_impl, T>{*this, t};
  }

  auto operator()(const init_base &) {
    return state_impl<init_state<>>();  // todo move it
  }
};

template <class... Ts>
struct state : state_impl<state<Ts...>> {};
template <class... Ts>
struct init_state : state_impl<init_state<Ts...>>, init_state_base {};
struct any_state : state_impl<any_state> {
} _;

template <class TEvent>
struct event_impl {
  template <class T>
  auto operator[](const T &t) const noexcept {
    return transition_eg<event_impl, T>{*this, t};
  }

  template <class T>
  auto operator/(const T &t) const noexcept {
    return transition_ea<event_impl, T>{*this, t};
  }
};

template <class TEvent>
event_impl<TEvent> event{};

template <class E, class G>
struct transition<event_impl<E>, G> {
  template <class T>
  auto operator/(const T &t) const noexcept {
    return transition<event_impl<E>, G, T>{e, g, t};
  }
  event_impl<E> e;
  G g;
};

template <class E, class G, class A>
struct transition<event_impl<E>, G, A> {
  event_impl<E> e;
  G g;
  A a;
};

template <class S2, class G, class A>
struct transition<state_impl<S2>, G, A> : transition<state_impl<S2>, state_impl<S2>, event_impl<anonymous>, G, A> {
  transition(const state_impl<S2> &s2, const G &g, const A &a)
      : transition<state_impl<S2>, state_impl<S2>, event_impl<anonymous>, G, A>{s2, s2, event_impl<anonymous>{}, g, a} {
  }
};

template <class S1, class... Ts>
struct transition<state_impl<S1>, state<Ts...>>
    : transition<state_impl<S1>, state_impl<state<Ts...>>, event_impl<anonymous>, always, none> {
  transition(const state_impl<S1> &s1, const state<Ts...> &s2)
      : transition<state_impl<S1>, state_impl<state<Ts...>>, event_impl<anonymous>, always, none>{
            s1, s2, event_impl<anonymous>{}, always{}, none{}} {}
};

template <class S2, class G>
struct transition_sg<state_impl<S2>, G> : transition<state_impl<S2>, state_impl<S2>, event_impl<anonymous>, G, none> {
  using transition_t = transition<state_impl<S2>, state_impl<S2>, event_impl<anonymous>, G, none>;
  using transition_t::s2;
  using transition_t::g;

  transition_sg(const state_impl<S2> &s2, const G &g)
      : transition<state_impl<S2>, state_impl<S2>, event_impl<anonymous>, G, none>{s2, s2, event_impl<anonymous>{}, g,
                                                                                   none{}} {}

  template <class T>
  auto operator/(const T &t) const noexcept {
    return transition<state_impl<S2>, G, T>{s2, g, t};
  }
};

template <class S2, class A>
struct transition_sa<state_impl<S2>, A> : transition<state_impl<S2>, state_impl<S2>, event_impl<anonymous>, always, A> {
  transition_sa(const state_impl<S2> &s2, const A &a)
      : transition<state_impl<S2>, state_impl<S2>, event_impl<anonymous>, always, A>{s2, s2, event_impl<anonymous>{},
                                                                                     always{}, a} {}
};

template <class S2, class E>
struct transition<state_impl<S2>, event_impl<E>> {
  const state_impl<S2> &s2;
  event_impl<E> e;
};

template <class E, class G>
struct transition_eg<event_impl<E>, G> {
  template <class T>
  auto operator/(const T &t) const noexcept {
    return transition<event_impl<E>, G, T>{e, g, t};
  }
  event_impl<E> e;
  G g;
};

template <class E, class A>
struct transition_ea<event_impl<E>, A> {
  event_impl<E> e;
  A a;
};

template <class, class>
aux::type_list<> args_impl__(...);
template <class T, class>
auto args_impl__(int) -> aux::function_traits_t<T>;
template <class T, class E>
auto args_impl__(int) -> aux::function_traits_t<decltype(&T::template operator() < E > )>;
template <class T, class>
auto args_impl__(int) -> aux::function_traits_t<decltype(&T::operator())>;
template <class T, class E>
auto args__(...) -> decltype(args_impl__<T, E>(0));
template <class T, class>
auto args__(int) -> aux::function_traits_t<decltype(&T::call_operator_args__)>;
template <class T, class E>
using args_t = decltype(args__<T, E>(0));

template <class, class>
struct ignore;

template <class E, class... Ts>
struct ignore<E, aux::type_list<Ts...>> {
  using type = aux::join_t<aux::conditional_t<aux::is_same<E, aux::remove_nextualifiers_t<Ts>>::value, aux::type_list<>,
                                              aux::type_list<Ts>>...>;
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

template <class T, class TEvent, class TDeps, class SM,
          aux::enable_if_t<!aux::is_same<TEvent, aux::remove_nextualifiers_t<T>>::value &&
                               !aux::is_same<self, aux::remove_nextualifiers_t<T>>::value,
                           int> = 0>
decltype(auto) get_arg(const TEvent &, TDeps &deps, SM &) noexcept {
  return aux::get<T>(deps);
}

template <class T, class TEvent, class TDeps, class SM,
          aux::enable_if_t<aux::is_same<TEvent, aux::remove_nextualifiers_t<T>>::value, int> = 0>
decltype(auto) get_arg(const TEvent &event, TDeps &, SM &) noexcept {
  return event;
}

template <class T, class TEvent, class TDeps, class SM,
          aux::enable_if_t<aux::is_same<self, aux::remove_nextualifiers_t<T>>::value, int> = 0>
decltype(auto) get_arg(const TEvent &, TDeps &, SM &sm) noexcept {
  return sm;
}

template <class... Ts, class T, class TEvent, class TDeps, class SM,
          aux::enable_if_t<!aux::is_base_of<operator_base, T>::value, int> = 0>
auto call_impl(const aux::type_list<Ts...> &, T object, const TEvent &event, TDeps &deps, SM &sm) noexcept {
  return object(get_arg<Ts>(event, deps, sm)...);
}

template <class... Ts, class T, class TEvent, class TDeps, class SM,
          aux::enable_if_t<aux::is_base_of<operator_base, T>::value, int> = 0>
auto call_impl(const aux::type_list<Ts...> &, T object, const TEvent &event, TDeps &deps, SM &sm) noexcept {
  return object(event, deps, sm);
}

template <class T, class TEvent, class TDeps, class SM>
auto call(T object, const TEvent &event, TDeps &deps, SM &sm) noexcept {
  return call_impl(args_t<T, TEvent>{}, object, event, deps, sm);
}

template <class... Ts>
class senext_ : operator_base {
 public:
  explicit senext_(Ts... ts) noexcept : a(ts...) {}

  template <class TEvent, class TDeps, class SM>
  void operator()(const TEvent &event, TDeps &deps, SM &sm) noexcept {
    for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, deps, sm);
  }

 private:
  template <int... Ns, class TEvent, class TDeps, class SM>
  void for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TDeps &deps, SM &sm) noexcept {
    int _[]{0, (call(aux::get<Ns - 1>(a), event, deps, sm), 0)...};
    (void)_;
  }

  aux::pool<Ts...> a;
};

template <class T1, class T2>
auto operator, (const T1 &t1, const T2 &t2) noexcept {
  return senext_<T1, T2>(t1, t2);
}

template <class... Ts>
class and_ : operator_base {
 public:
  explicit and_(Ts... ts) noexcept : g(ts...) {}

  template <class TEvent, class TDeps, class SM>
  auto operator()(const TEvent &event, TDeps &deps, SM &sm) noexcept {
    return for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, deps, sm);
  }

 private:
  template <int... Ns, class TEvent, class TDeps, class SM>
  auto for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TDeps &deps, SM &sm) noexcept {
    auto result = true;
    for (auto r : {call(aux::get<Ns - 1>(g), event, deps, sm)...}) result &= r;
    return result;
  }

  aux::pool<Ts...> g;
};

template <class T1, class T2>
auto operator&&(const T1 &t1, const T2 &t2) noexcept {
  return and_<T1, T2>(t1, t2);
}

template <class... Ts>
class or_ : operator_base {
 public:
  explicit or_(Ts... ts) noexcept : g(ts...) {}

  template <class TEvent, class TDeps, class SM>
  auto operator()(const TEvent &event, TDeps &deps, SM &sm) noexcept {
    return for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, deps, sm);
  }

 private:
  template <int... Ns, class TEvent, class TDeps, class SM>
  auto for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TDeps &deps, SM &sm) noexcept {
    auto result = false;
    for (auto r : {call(aux::get<Ns - 1>(g), event, deps, sm)...}) result |= r;
    return result;
  }

  aux::pool<Ts...> g;
};

template <class T1, class T2>
auto operator||(const T1 &t1, const T2 &t2) noexcept {
  return or_<T1, T2>(t1, t2);
}

template <class T>
class not_ : operator_base {
 public:
  explicit not_(T t) noexcept : g(t) {}

  template <class TEvent, class TDeps, class SM>
  auto operator()(const TEvent &event, TDeps &deps, SM &sm) noexcept {
    return !call(g, event, deps, sm);
  }

 private:
  T g;
};

template <class T>
auto operator!(const T &t) noexcept {
  return not_<T>(t);
}

template <class S1, class S2, class G, class A>
struct transition<state_impl<S1>, transition<state_impl<S2>, G, A>>
    : transition<state_impl<S1>, state_impl<S2>, event_impl<anonymous>, G, A> {
  transition(const state_impl<S1> &s1, const transition<state_impl<S2>, G, A> &t)
      : transition<state_impl<S1>, state_impl<S2>, event_impl<anonymous>, G, A>{s1, t.s2, event_impl<anonymous>{}, t.g,
                                                                                t.a} {}
};

template <class S1, class E, class G, class A>
struct transition<state_impl<S1>, transition<event_impl<E>, G, A>>
    : transition<state_impl<S1>, state_impl<S1>, event_impl<E>, G, A> {
  transition(const state_impl<S1> &s1, const transition<event_impl<E>, G, A> &t)
      : transition<state_impl<S1>, state_impl<S1>, event_impl<E>, G, A>{s1, s1, t.e, t.g, t.a} {}
};

template <class S1, class S2, class E>
struct transition<state_impl<S1>, transition<state_impl<S2>, event_impl<E>>>
    : transition<state_impl<S1>, state_impl<S2>, event_impl<E>, always, none> {
  transition(const state_impl<S1> &s1, const transition<state_impl<S2>, event_impl<E>> &t)
      : transition<state_impl<S1>, state_impl<S2>, event_impl<E>, always, none>{s1, t.s2, t.e, always{}, none{}} {}
};

template <class S1, class S2, class G>
struct transition<state_impl<S1>, transition_sg<state_impl<S2>, G>>
    : transition<state_impl<S1>, state_impl<S2>, event_impl<anonymous>, G, none> {
  transition(const state_impl<S1> &s1, const transition_sg<state_impl<S2>, G> &t)
      : transition<state_impl<S1>, state_impl<S2>, event_impl<anonymous>, G, none>{s1, t.s2, event_impl<anonymous>{},
                                                                                   t.g, none{}} {}
};

template <class S1, class S2, class A>
struct transition<state_impl<S1>, transition_sa<state_impl<S2>, A>>
    : transition<state_impl<S1>, state_impl<S2>, event_impl<anonymous>, always, A> {
  transition(const state_impl<S1> &s1, const transition_sa<state_impl<S2>, A> &t)
      : transition<state_impl<S1>, state_impl<S2>, event_impl<anonymous>, always, A>{s1, t.s2, event_impl<anonymous>{},
                                                                                     always{}, t.a} {}
};

template <class S2, class E, class G>
struct transition<state_impl<S2>, transition_eg<event_impl<E>, G>>
    : transition<state_impl<S2>, state_impl<S2>, event_impl<E>, G, none> {
  transition(const state_impl<S2> &s2, const transition_eg<event_impl<E>, G> &t)
      : transition<state_impl<S2>, state_impl<S2>, event_impl<E>, G, none>{s2, s2, t.e, t.g, none{}} {}
};

template <class S1, class S2, class E, class G>
struct transition<state_impl<S1>, transition<state_impl<S2>, transition_eg<event_impl<E>, G>>>
    : transition<state_impl<S1>, state_impl<S2>, event_impl<E>, G, none> {
  transition(const state_impl<S1> &s1, const transition<state_impl<S2>, transition_eg<event_impl<E>, G>> &t)
      : transition<state_impl<S1>, state_impl<S2>, event_impl<E>, G, none>{s1, t.s2, t.e, t.g, none{}} {}
};

template <class S2, class E, class A>
struct transition<state_impl<S2>, transition_ea<event_impl<E>, A>>
    : transition<state_impl<S2>, state_impl<S2>, event_impl<E>, always, A> {
  transition(const state_impl<S2> &s2, const transition_ea<event_impl<E>, A> &t)
      : transition<state_impl<S2>, state_impl<S2>, event_impl<E>, always, A>{s2, s2, t.e, always{}, t.a} {}
};

template <class S1, class S2, class E, class A>
struct transition<state_impl<S1>, transition<state_impl<S2>, transition_ea<event_impl<E>, A>>>
    : transition<state_impl<S1>, state_impl<S2>, event_impl<E>, always, A> {
  transition(const state_impl<S1> &s1, const transition<state_impl<S2>, transition_ea<event_impl<E>, A>> &t)
      : transition<state_impl<S1>, state_impl<S2>, event_impl<E>, always, A>{s1, t.s2, t.e, always{}, t.a} {}
};

template <class S1, class S2, class E, class G, class A>
struct transition<state_impl<S1>, transition<state_impl<S2>, transition<event_impl<E>, G, A>>>
    : transition<state_impl<S1>, state_impl<S2>, event_impl<E>, G, A> {
  transition(const state_impl<S1> &s1, const transition<state_impl<S2>, transition<event_impl<E>, G, A>> &t)
      : transition<state_impl<S1>, state_impl<S2>, event_impl<E>, G, A>{s1, t.s2, t.e, t.g, t.a} {}
};

template <class S1, class S2, class E, class G, class A>
struct transition<state_impl<S1>, state_impl<S2>, event_impl<E>, G, A> {
  using src_state = S1;
  using dst_state = S2;
  using event = E;
  using deps = aux::apply_t<aux::unique_t, aux::join_t<get_deps_t<G, E>, get_deps_t<A, E>>>;

  const state_impl<S1> &s1;
  const state_impl<S2> &s2;
  event_impl<E> e;
  G g;
  A a;
};

template <class... Ts>
using merge_deps = aux::apply_t<aux::pool, aux::apply_t<aux::unique_t, aux::join_t<typename Ts::deps...>>>;

template <class... Ts>
using init_states_nr = aux::apply_t<
    aux::sum_up,
    aux::type_list<aux::integral_constant<int, aux::is_base_of<init_state_base, typename Ts::src_state>::value>...>>;

template <class... Ts>
using get_event = aux::type_list<typename Ts::event...>;

template <class T, class... TDeps>
class sm_impl<T, aux::pool<TDeps...>> : public state_impl<state<sm_impl<T, aux::pool<TDeps...>>>> {
  using process_event_ptr = bool (sm_impl::*)(int, void *, int, int);
  using transitions_t = decltype(aux::declval<T>().configure());
  static constexpr int transitions_nr = aux::get_size<transitions_t>::value;
  static constexpr int regions_nr = aux::apply_t<init_states_nr, transitions_t>::value;
  using indexes_t = aux::make_index_sequence<transitions_nr>;

  static_assert(regions_nr > 0, "At least one init state is required, 'msm::state<> idle; idle(init)'");

 public:
  using events = aux::apply_t<aux::unique_t, aux::apply_t<get_event, transitions_t>>;

 private:
  using events_ids = aux::apply_t<aux::type_id, events>;
  static constexpr int events_nr = aux::get_size<events>::value;

 public:
  explicit sm_impl(const T &fsm, TDeps... deps) noexcept : fsm_(fsm), deps_{deps...}, transitions_(fsm_.configure()) {
    init(indexes_t{});
  }

  ~sm_impl() noexcept {}

  template <class TEvent>
  void process_event(const TEvent &event) noexcept {
    // std::cout << "process_event: [" << current << "][" << aux::get_id<events_ids, events_nr-1, TEvent>() << "][0]" <<
    // std::endl;
    constexpr auto id = aux::get_id<events_ids, events_nr - 1, TEvent>();
    for (auto r = 0; r < regions_nr; ++r) {
      (this->*dispatch_table_[current[r]][id][0])(r, (void *)&event, id, 0);
    }
  }

 private:
  template <int... Ns>
  void init(const aux::index_sequence<Ns...> &) noexcept {
    for (auto i = 0; i < transitions_nr; ++i)
      for (auto j = 0; j < events_nr; ++j)
        for (auto k = 0; k < transitions_nr; ++k) dispatch_table_[i][j][k] = &sm_impl::no_transition;

    const state_base *c[transitions_nr] = {};
    int _[]{0, (init_impl<Ns - 1>(c), 0)...};
    (void)_;
  }

  template <int N>
  void init_impl(const state_base *c[]) noexcept {
    using Tr = decltype(aux::get<N>(transitions_));
    constexpr auto id = aux::get_id<events_ids, events_nr - 1, typename Tr::event>();
    const auto &tr = aux::get<N>(transitions_);
    if (!N) c[0] = &tr.s1;

    for (auto i = 0; i < transitions_nr; ++i) {
      if (c[i] == &tr.s1) {
        for (auto r = 0; r < transitions_nr; ++r) {
          if (dispatch_table_[i][id][r] == &sm_impl::no_transition) {
            // std::cout << "set: [" << i  << "][" << id << "][" << r << "] ";
            dispatch_table_[i][id][r] = &sm_impl::template process_event_impl<N>;
            break;
          }
        }
      }
    }
    c[N + 1] = &tr.s2;
  }

  auto no_transition(int, void *, int, int) noexcept {
    std::cout << "no transition" << std::endl;
    return false;
  }

  template <int N>
  auto process_event_impl(int r, void *e, int eid, int next) noexcept {
    using type = decltype(aux::get<N>(transitions_));
    const auto &transition = aux::get<N>(transitions_);
    const auto &event = *static_cast<const typename type::event *>(e);

    if (call(transition.g, event, deps_, *this)) {
      call(transition.a, event, deps_, *this);
      current[r] = N + 1;
      return true;
    }

    // std::cout << "process_event: [" << current[r] << "][" << eid << "][" << next+1 << "]" << std::endl;
    return (this->*dispatch_table_[current[r]][eid][next + 1])(r, e, eid, next + 1);
  }

  const T &fsm_;
  aux::pool<TDeps...> deps_;
  transitions_t transitions_;
  process_event_ptr dispatch_table_[transitions_nr + 1][events_nr][1];
  int current[!regions_nr ? 1 : regions_nr] = {};
};

template <class T>
using sm = sm_impl<T, aux::apply_t<merge_deps, decltype(aux::declval<T>().configure())>>;

struct process_event_impl {
  template <class TEvent>
  struct process_impl {
    void call_operator_args__(self);

    template <class SM>
    void operator()(SM &sm, ...) noexcept {
      sm.process_event(event);
    }

    TEvent event;
  };

  template <class TEvent>
  auto operator()(const TEvent &event) noexcept {
    return process_impl<TEvent>{event};
  }
} process_event;

template <class TEvent, class TConfig>
struct dispatcher {
  template <class T>
  static void dispatch_event(const TEvent &e, T &sm) noexcept {
    dispatch_event_impl(e, sm, typename T::events{});
  }

 private:
  template <class T, class E, class... Ts>
  static void dispatch_event_impl(const TEvent &event, T &sm, const aux::type_list<E, Ts...> &) noexcept {
    if (TConfig::template get_id<E>() == TConfig::get_id(event)) {
      sm.process_event(TConfig::template get_event<E>(event));
    }
    dispatch_event_impl(event, sm, aux::type_list<Ts...>{});
  }

  template <class T>
  static void dispatch_event_impl(const TEvent &, T &, const aux::type_list<> &) noexcept {}
};
}
}  // msm
