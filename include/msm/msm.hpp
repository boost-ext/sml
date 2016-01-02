//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once
namespace msm {
inline namespace v_1_0_0 {
namespace aux {
using byte = unsigned char;
struct none_type {};
template <class...>
struct type {};
template <class...>
struct object {
  unsigned char b;
};
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
struct remove_qualifiers {
  using type = T;
};
template <class T>
struct remove_qualifiers<const T> {
  using type = T;
};
template <class T>
struct remove_qualifiers<T &> {
  using type = T;
};
template <class T>
struct remove_qualifiers<const T &> {
  using type = T;
};
template <class T>
struct remove_qualifiers<T *> {
  using type = T;
};
template <class T>
struct remove_qualifiers<const T *> {
  using type = T;
};
template <class T>
struct remove_qualifiers<T *const &> {
  using type = T;
};
template <class T>
struct remove_qualifiers<T *const> {
  using type = T;
};
template <class T>
struct remove_qualifiers<const T *const> {
  using type = T;
};
template <class T>
struct remove_qualifiers<T &&> {
  using type = T;
};
template <class T>
using remove_qualifiers_t = typename remove_qualifiers<T>::type;
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
struct unique_impl;
template <class T1, class T2, class... Rs, class... Ts>
struct unique_impl<type<T1, Rs...>, T2, Ts...>
    : conditional_t<is_base_of<type<T2>, T1>::value, unique_impl<type<inherit<T1>, Rs...>, Ts...>,
                    unique_impl<type<inherit<T1, type<T2>>, Rs..., T2>, Ts...>> {};
template <class T1, class... Rs>
struct unique_impl<type<T1, Rs...>> : type_list<Rs...> {};
template <class... Ts>
struct unique : unique_impl<type<none_type>, Ts...> {};
template <class T>
struct unique<T> : type_list<T> {};
template <class... Ts>
using unique_t = typename unique<Ts...>::type;
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
  using underlying_type = pool_impl<make_index_sequence<sizeof...(Ts)>, Ts...>;
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
  return N - 1;
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
}  // aux

struct operator_base {};
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

struct state_base {};
struct initial_state_base {
} initial;
struct terminate_state_base {
} terminate;

template <class>
struct state_impl : state_base {
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

  template <class T>
  const auto &operator()(const T &) const noexcept {
    return *((state_impl<T> *)this);
  }
};

using state = state_impl<state_base>;

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

template <class S1, class S2>
struct transition<state_impl<S1>, state_impl<S2>>
    : transition<state_impl<S1>, state_impl<S2>, event_impl<anonymous>, always, none> {
  transition(const state_impl<S1> &s1, const state_impl<S2> &s2)
      : transition<state_impl<S1>, state_impl<S2>, event_impl<anonymous>, always, none>{s1, s2, event_impl<anonymous>{},
                                                                                        always{}, none{}} {}
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
  using type = aux::join_t<aux::conditional_t<aux::is_same<E, aux::remove_qualifiers_t<Ts>>::value, aux::type_list<>,
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
          aux::enable_if_t<!aux::is_same<TEvent, aux::remove_qualifiers_t<T>>::value &&
                               !aux::is_same<self, aux::remove_qualifiers_t<T>>::value,
                           int> = 0>
decltype(auto) get_arg(const TEvent &, TDeps &deps, SM &) noexcept {
  return aux::get<T>(deps);
}

template <class T, class TEvent, class TDeps, class SM,
          aux::enable_if_t<aux::is_same<TEvent, aux::remove_qualifiers_t<T>>::value, int> = 0>
decltype(auto) get_arg(const TEvent &event, TDeps &, SM &) noexcept {
  return event;
}

template <class T, class TEvent, class TDeps, class SM,
          aux::enable_if_t<aux::is_same<self, aux::remove_qualifiers_t<T>>::value, int> = 0>
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
class seq_ : operator_base {
 public:
  explicit seq_(Ts... ts) noexcept : a(ts...) {}

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
  return seq_<T1, T2>(t1, t2);
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

  template <class SM>
  auto execute(void *e, SM &sm) noexcept {
    const auto &event = *static_cast<const E *>(e);
    if (call(g, event, sm.deps_, sm)) {
      call(a, event, sm.deps_, sm);
      // std::cout << typeid(src_state).name() << ":" << typeid(dst_state).name() << std::endl;
      return true;
    }
    return false;
  }

  const state_impl<S1> &s1;
  const state_impl<S2> &s2;
  event_impl<E> e;
  G g;
  A a;
};

template <class S1, class S2, class E, class A>
struct transition<state_impl<S1>, state_impl<S2>, event_impl<E>, always, A> {
  using src_state = S1;
  using dst_state = S2;
  using event = E;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<A, E>>;

  template <class SM>
  auto execute(void *e, SM &sm) noexcept {
    call(a, *static_cast<const E *>(e), sm.deps_, sm);
    return true;
  }

  const state_impl<S1> &s1;
  const state_impl<S2> &s2;
  event_impl<E> e;
  always g;
  A a;
};

template <class... Ts>
using merge_deps = aux::apply_t<aux::pool, aux::apply_t<aux::unique_t, aux::join_t<typename Ts::deps...>>>;

template <class, class...>
struct initial_states_nr_impl;

template <int... Ns, class... Ts>
struct initial_states_nr_impl<aux::pool_impl<aux::index_sequence<Ns...>, Ts...>> {
  static constexpr auto value =
      sizeof(aux::inherit<aux::conditional_t<aux::is_same<initial_state_base, typename Ts::src_state>::value,
                                             aux::object<Ts, aux::integral_constant<int, Ns>>,
                                             aux::type<Ts, aux::integral_constant<int, Ns>>>...>);
};

template <class T>
using initial_states_nr = initial_states_nr_impl<typename T::underlying_type>;

template <class TState, class TEvent>
struct get_events_ {
  using type = aux::type_list<TEvent>;
};

template <class... Ts, class TEvent>
struct get_events_<sm_impl<Ts...>, TEvent> {
  using type = aux::join_t<aux::type_list<TEvent>, typename sm_impl<Ts...>::events>;
};

template <class... Ts>
using get_event = aux::join_t<typename get_events_<typename Ts::src_state, typename Ts::event>::type...>;

template <class>
struct is_sm : aux::false_type {};

template <class... Ts>
struct is_sm<sm_impl<Ts...>> : aux::true_type {};

template <class T, class... TDeps>
class sm_impl<T, aux::pool<TDeps...>> : public state_impl<sm_impl<T, aux::pool<TDeps...>>> {
  using process_event_ptr = bool (*)(sm_impl &, int, void *, int, int);
  using transitions_t = decltype(aux::declval<T>().configure());
  using events_t = aux::apply_t<aux::unique_t, aux::apply_t<get_event, transitions_t>>;
  using events_ids_t = aux::apply_t<aux::type_id, events_t>;

  static constexpr int events_nr = aux::get_size<events_t>::value;
  static constexpr int transitions_nr = aux::get_size<transitions_t>::value;
  static constexpr int regions_nr = initial_states_nr<transitions_t>::value;

  template <class...>
  friend struct transition;

  template <class, class>
  friend class sm_impl;

 public:
  using events = events_t;

  explicit sm_impl(const T &fsm, TDeps... deps) noexcept : fsm_(fsm), deps_{deps...}, transitions_(fsm_.configure()) {
    init(aux::make_index_sequence<transitions_nr>{});
  }

  void start() noexcept {}
  void stop() noexcept {}

  template <class TEvent>
  auto process_event(const TEvent &event) noexcept {
    // std::cout << "here" << (int)dispatch_table_mappings_[0][0][0] << std::endl;
    return process_event__(event, aux::integral_constant<int, aux::get_id<events_ids_t, -1, TEvent>()>{});
  }

  template <class TFlag>
  auto is(const TFlag &) const noexcept {
    return is__<TFlag>(aux::make_index_sequence<transitions_nr>{}, aux::make_index_sequence<regions_nr>{});
  }

 private:
  template <int... Ns>
  void init(const aux::index_sequence<Ns...> &) noexcept {
    auto slot = 1;
    auto region = 0;
    const state_base *states[transitions_nr] = {&aux::get<Ns - 1>(transitions_).s2...};
    int _[]{0, (init_impl<Ns - 1>(region, slot, states), 0)...};
    (void)_;
  }

  template <int>
  void init_initial(...) noexcept {}

  template <int N>
  void init_initial(int &region, int &slot, int id, const aux::true_type &) noexcept {
    update_dispatch_table<N>(transitions_nr + region, id, slot);
    current_state[region] = transitions_nr + region;
    ++region;
  }

  template <int>
  void init_sm(...) noexcept {}

  template <int N>
  void init_sm(const aux::true_type &) noexcept {
    // std::cout << "set sm: [" << N << "]" << transitions_nr << std::endl;
    update_dispatch_table<N>(transitions_nr + 1);
    dispatch_table_[transitions_nr + 1] = &sm_impl::template process_event_sm_impl<N>;
  }

  template <int N>
  void init_dispatch_table(int id, int &slot, const state_base *states[]) noexcept {
    const auto &transition = aux::get<N>(transitions_);
    for (auto i = 0; i < transitions_nr; ++i) {
      // std::cout << &transition.s1 << " " << states[i] << std::endl;
      if (&transition.s1 == states[i]) {
        update_dispatch_table<N>(i, id, slot);
      }
    }
  }

  template <int N>
  void init_impl(int &region, int &slot, const state_base *states[]) noexcept {
    using Transition = decltype(aux::get<N>(transitions_));
    constexpr auto id = aux::get_id<events_ids_t, events_nr - 1, typename Transition::event>();
    init_initial<N>(region, slot, id, aux::is_same<initial_state_base, typename Transition::src_state>{});
    init_sm<N>(is_sm<typename Transition::dst_state>{});
    init_dispatch_table<N>(id, slot, states);
    dispatch_table_[slot++] = &sm_impl::template process_event_impl<N>;
  }

  template <int N>
  void update_dispatch_table(int i, int id, int slot) {
    for (auto x = 0; x < 5 /*max number per event*/; ++x) {
      if (!dispatch_table_mappings_[i][id][x]) {
        dispatch_table_mappings_[i][id][x] = slot;
        // std::cout << "set: [" << i << "][" << id << "][" << x << "] :" << slot << std::endl;
        break;
      }
    }
  }

  template <int N>
  void update_dispatch_table(int slot) {
    for (auto id = 0; id < events_nr; ++id) {
      update_dispatch_table<N>(N, id, slot);
    }
  }

  static auto no_transition(sm_impl &, int, void *, int, int) noexcept {
    // std::cout << "no transition" << std::endl;
    return false;
  }

  template <class TEvent>
  auto process_event__(const TEvent &, const aux::integral_constant<int, -1> &) noexcept {
    // std::cout << "no transition__" << std::endl;
    return false;
  }

  template <class TEvent, int N>
  auto process_event__(const TEvent &event, const aux::integral_constant<int, N> &) noexcept {
    return process_event__((void *)&event, N);
  }

  auto process_event__(void *event, int id) noexcept {
    auto handled = false;
    for (auto r = 0; r < regions_nr; ++r) {
      // std::cout << "xx process_event: [" << (int)current_state[0] << "][" << N << "][0]" << std::endl;
      handled |= dispatch_table_[dispatch_table_mappings_[current_state[0]][id][0]](*this, 0, event, id, 0);
    }

    if (!handled) {
      // std::cout << "no transition" << std::endl;
    }
    return handled;
  }

  template <int N>
  static auto process_event_impl(sm_impl &self, int r, void *event, int id, int next) noexcept {
    if (aux::get<N>(self.transitions_).execute(event, self)) {
      // on_exit
      self.current_state[r] = N;
      // on_entry
      return true;
    }

    return self.dispatch_table_[self.dispatch_table_mappings_[self.current_state[r]][id][next + 1]](self, r, event, id,
                                                                                                    next + 1);
  }

  template <class SM, class... TEvents>
  static auto id__(int id, const aux::type_list<TEvents...> &) noexcept {
    int events_mapping[sizeof...(TEvents)] = {aux::get_id<typename SM::events_ids_t, -1, TEvents>()...};
    return events_mapping[id];
  }

  template <int N>
  static auto process_event_sm_impl(sm_impl &self, int r, void *event, int id, int next) noexcept {
    using Transition = decltype(aux::get<N>(self.transitions_));
    using SM = typename Transition::dst_state;

    auto i = id__<SM>(id, events{});
    if (i != -1 && ((SM &)aux::get<N>(self.transitions_).s2).process_event__(event, i)) {
      return true;
    }

    return self.dispatch_table_[self.dispatch_table_mappings_[self.current_state[r]][id][next + 1]](self, r, event, id,
                                                                                                    next + 1);
  }

  template <class TFlag, int... Ns, int... Rs>
  auto is__(const aux::index_sequence<Ns...> &, const aux::index_sequence<Rs...> &) const noexcept {
    using is_ptr = bool (*)(const sm_impl &);
    is_ptr dispatch_table[sizeof...(Ns) + sizeof...(Rs)] = {&sm_impl::template is_impl<TFlag, Ns - 1>...,
                                                            &sm_impl::template is_init_impl<TFlag, Rs - 1>...};
    auto is_flag = false;
    for (auto r = 0; r < regions_nr; ++r) {
      is_flag |= dispatch_table[current_state[r]](*this);
    }
    return is_flag;
  }

  template <class TFlag, int N>
  static auto is_impl(const sm_impl &) noexcept {
    return aux::is_same<TFlag, typename decltype(aux::get<N>(transitions_))::dst_state>::value;
  }

  template <class TFlag, int N>
  static auto is_init_impl(const sm_impl &) noexcept {
    return aux::is_same<TFlag, typename decltype(aux::get<N>(transitions_))::src_state>::value;
  }

  const T &fsm_;
  aux::pool<TDeps...> deps_;
  transitions_t transitions_;
  process_event_ptr dispatch_table_[transitions_nr + 1 + 1 /*subfsm*/] = {&sm_impl::no_transition};
  aux::byte dispatch_table_mappings_[transitions_nr + regions_nr + 1][events_nr][1 /*max transitions per event*/ +
                                                                                 1 /*subfsm*/] = {};
  aux::byte current_state[regions_nr] = {};
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
