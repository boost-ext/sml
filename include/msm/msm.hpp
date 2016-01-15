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
template <char...>
struct string {};
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
template <template <class> class T, typename... Ts>
constexpr auto count_impl() noexcept {
  auto value = 0;
  int _[]{0, (T<Ts>::value ? value++ : value)...};
  (void)_;
  return value;
}
template <template <class> class T, typename... Ts>
struct count {
  using type = count;
  static constexpr auto value = count_impl<T, Ts...>();
};
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

namespace detail {

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

struct operator_base {};
struct self {};
struct anonymous {};
struct always {
  auto operator()() { return true; }
};
struct none {
  void operator()() {}
};
struct process_event {
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
};

struct initial_state {};

template <class TState, class... Ts>
struct state : Ts... {
  template <class T>
  auto operator==(const T &t) const noexcept {
    return transition<state, T>{*this, t};
  }

  template <class T>
  auto operator+(const T &t) const noexcept {
    return transition<state, T>{*this, t};
  }

  template <class T>
  auto operator[](const T &t) const noexcept {
    return transition_sg<state, T>{*this, t};
  }

  template <class T>
  auto operator/(const T &t) const noexcept {
    return transition_sa<state, T>{*this, t};
  }

  template <class T>
  auto operator()(const T &) const noexcept {
    return state<TState, Ts..., T>{};
  }
};

template <class TState>
using is_initial = aux::is_base_of<initial_state, TState>;

template <class>
struct is_sm : aux::false_type {};

template <class... Ts, class... S>
struct is_sm<state<sm_impl<Ts...>, S...>> : aux::true_type {};

template <class>
struct event {
  template <class T>
  auto operator[](const T &t) const noexcept {
    return transition_eg<event, T>{*this, t};
  }

  template <class T>
  auto operator/(const T &t) const noexcept {
    return transition_ea<event, T>{*this, t};
  }
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
  using type = aux::join_t<aux::conditional_t<aux::is_same<E, aux::remove_qualifiers_t<Ts>>::value ||
                                                  aux::is_same<Ts, aux::remove_qualifiers_t<self>>::value,
                                              aux::type_list<>, aux::type_list<Ts>>...>;
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
    bool calls[] = {call(aux::get<Ns - 1>(g), event, deps, sm)...};
    for (auto r : calls) result &= r;
    return result;
  }

  aux::pool<Ts...> g;
};

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
    bool calls[] = {call(aux::get<Ns - 1>(g), event, deps, sm)...};
    for (auto r : calls) result |= r;
    return result;
  }

  aux::pool<Ts...> g;
};

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

template <class E, class G>
struct transition<event<E>, G> {
  template <class T>
  auto operator/(const T &t) const noexcept {
    return transition<event<E>, G, T>{e, g, t};
  }
  event<E> e;
  G g;
};

template <class E, class G, class A>
struct transition<event<E>, G, A> {
  event<E> e;
  G g;
  A a;
};

template <class... S2, class G, class A>
struct transition<state<S2...>, G, A> : transition<state<S2...>, state<S2...>, event<anonymous>, G, A> {
  transition(const G &g, const A &a) : transition<state<S2...>, state<S2...>, event<anonymous>, G, A>{g, a} {}
};

template <class... S1, class... S2>
struct transition<state<S1...>, state<S2...>> : transition<state<S1...>, state<S2...>, event<anonymous>, always, none> {
  transition(const state<S1...> &, const state<S2...> &)
      : transition<state<S1...>, state<S2...>, event<anonymous>, always, none>{always{}, none{}} {}
};

template <class... S2, class G>
struct transition_sg<state<S2...>, G> : transition<state<S2...>, state<S2...>, event<anonymous>, G, none> {
  using transition_t = transition<state<S2...>, state<S2...>, event<anonymous>, G, none>;
  using transition_t::g;

  transition_sg(const state<S2...> &, const G &g)
      : transition<state<S2...>, state<S2...>, event<anonymous>, G, none>{g, none{}} {}

  template <class T>
  auto operator/(const T &t) const noexcept {
    return transition<state<S2...>, G, T>{g, t};
  }
};

template <class... S2, class A>
struct transition_sa<state<S2...>, A> : transition<state<S2...>, state<S2...>, event<anonymous>, always, A> {
  transition_sa(const state<S2...> &, const A &a)
      : transition<state<S2...>, state<S2...>, event<anonymous>, always, A>{always{}, a} {}
};

template <class... S2, class E>
struct transition<state<S2...>, event<E>> {
  const state<S2...> &s2;
  event<E> e;
};

template <class E, class G>
struct transition_eg<event<E>, G> {
  template <class T>
  auto operator/(const T &t) const noexcept {
    return transition<event<E>, G, T>{e, g, t};
  }
  event<E> e;
  G g;
};

template <class E, class A>
struct transition_ea<event<E>, A> {
  event<E> e;
  A a;
};

template <class... S1, class... S2, class G, class A>
struct transition<state<S1...>, transition<state<S2...>, G, A>>
    : transition<state<S1...>, state<S2...>, event<anonymous>, G, A> {
  transition(const state<S1...> &, const transition<state<S2...>, G, A> &t)
      : transition<state<S1...>, state<S2...>, event<anonymous>, G, A>{t.g, t.a} {}
};

template <class... S1, class E, class G, class A>
struct transition<state<S1...>, transition<event<E>, G, A>> : transition<state<S1...>, state<S1...>, event<E>, G, A> {
  transition(const state<S1...> &, const transition<event<E>, G, A> &t)
      : transition<state<S1...>, state<S1...>, event<E>, G, A>{t.g, t.a} {}
};

template <class... S1, class... S2, class E>
struct transition<state<S1...>, transition<state<S2...>, event<E>>>
    : transition<state<S1...>, state<S2...>, event<E>, always, none> {
  transition(const state<S1...> &, const transition<state<S2...>, event<E>> &)
      : transition<state<S1...>, state<S2...>, event<E>, always, none>{always{}, none{}} {}
};

template <class... S1, class... S2, class G>
struct transition<state<S1...>, transition_sg<state<S2...>, G>>
    : transition<state<S1...>, state<S2...>, event<anonymous>, G, none> {
  transition(const state<S1...> &, const transition_sg<state<S2...>, G> &t)
      : transition<state<S1...>, state<S2...>, event<anonymous>, G, none>{t.g, none{}} {}
};

template <class... S1, class... S2, class A>
struct transition<state<S1...>, transition_sa<state<S2...>, A>>
    : transition<state<S1...>, state<S2...>, event<anonymous>, always, A> {
  transition(const state<S1...> &, const transition_sa<state<S2...>, A> &t)
      : transition<state<S1...>, state<S2...>, event<anonymous>, always, A>{always{}, t.a} {}
};

template <class... S2, class E, class G>
struct transition<state<S2...>, transition_eg<event<E>, G>>
    : transition<state<S2...>, state<S2...>, event<E>, G, none> {
  transition(const state<S2...> &, const transition_eg<event<E>, G> &t)
      : transition<state<S2...>, state<S2...>, event<E>, G, none>{t.g, none{}} {}
};

template <class... S1, class... S2, class E, class G>
struct transition<state<S1...>, transition<state<S2...>, transition_eg<event<E>, G>>>
    : transition<state<S1...>, state<S2...>, event<E>, G, none> {
  transition(const state<S1...> &, const transition<state<S2...>, transition_eg<event<E>, G>> &t)
      : transition<state<S1...>, state<S2...>, event<E>, G, none>{t.g, none{}} {}
};

template <class... S2, class E, class A>
struct transition<state<S2...>, transition_ea<event<E>, A>>
    : transition<state<S2...>, state<S2...>, event<E>, always, A> {
  transition(const state<S2...> &, const transition_ea<event<E>, A> &t)
      : transition<state<S2...>, state<S2...>, event<E>, always, A>{always{}, t.a} {}
};

template <class... S1, class... S2, class E, class A>
struct transition<state<S1...>, transition<state<S2...>, transition_ea<event<E>, A>>>
    : transition<state<S1...>, state<S2...>, event<E>, always, A> {
  transition(const state<S1...> &, const transition<state<S2...>, transition_ea<event<E>, A>> &t)
      : transition<state<S1...>, state<S2...>, event<E>, always, A>{always{}, t.a} {}
};

template <class... S1, class... S2, class E, class G, class A>
struct transition<state<S1...>, transition<state<S2...>, transition<event<E>, G, A>>>
    : transition<state<S1...>, state<S2...>, event<E>, G, A> {
  transition(const state<S1...> &, const transition<state<S2...>, transition<event<E>, G, A>> &t)
      : transition<state<S1...>, state<S2...>, event<E>, G, A>{t.g, t.a} {}
};

template <class... S1, class... S2, class E, class G, class A>
struct transition<state<S1...>, state<S2...>, event<E>, G, A> {
  using src_state = state<S1...>;
  using dst_state = state<S2...>;
  using event = E;
  using deps = aux::apply_t<aux::unique_t, aux::join_t<get_deps_t<G, E>, get_deps_t<A, E>>>;

  transition(G g, A a) : g(g), a(a) {}

  template <class SM>
  auto execute(SM &self, const E &event, aux::byte &current_state) noexcept {
    if (call(g, event, self.deps_, self)) {
      call(a, event, self.deps_, self);
      current_state = aux::get_id<typename SM::states_ids_t, -1, dst_state>();
      return true;
    }
    return false;
  }

  G g;
  A a;
};

template <class... S1, class... S2, class E, class A>
struct transition<state<S1...>, state<S2...>, event<E>, always, A> {
  using src_state = state<S1...>;
  using dst_state = state<S2...>;
  using event = E;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<A, E>>;

  transition(always, A a) : a(a) {}

  template <class SM>
  auto execute(SM &self, const E &event, aux::byte &current_state) noexcept {
    call(a, event, self.deps_, self);
    current_state = aux::get_id<typename SM::states_ids_t, -1, dst_state>();
    return true;
  }

  A a;
};

template <class... S1, class... S2, class E, class G>
struct transition<state<S1...>, state<S2...>, event<E>, G, none> {
  using src_state = state<S1...>;
  using dst_state = state<S2...>;
  using event = E;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<G, E>>;

  transition(G g, none) : g(g) {}

  template <class SM>
  auto execute(SM &self, const E &event, aux::byte &current_state) noexcept {
    if (call(g, event, self.deps_, self)) {
      current_state = aux::get_id<typename SM::states_ids_t, -1, dst_state>();
      return true;
    }
    return false;
  }

  G g;
};

template <class... S1, class... S2, class E>
struct transition<state<S1...>, state<S2...>, event<E>, always, none> {
  using src_state = state<S1...>;
  using dst_state = state<S2...>;
  using event = E;
  using deps = aux::type_list<>;

  transition(always, none) {}

  template <class SM>
  auto execute(SM &, const E &, aux::byte &current_state) noexcept {
    current_state = aux::get_id<typename SM::states_ids_t, -1, dst_state>();
    return true;
  }
};

template <class...>
struct transition_impl;

template <class T, class... Ts>
struct transition_impl<T, Ts...> {
  using type = transition_impl;
  template <class SM, class TEvent>
  static bool execute(SM &self, const TEvent &event, aux::byte &current_state) noexcept {
    if (aux::get<T::value>(self.transitions_).execute(self, event, current_state)) {
      return true;
    }
    return transition_impl<Ts...>::execute(self, event, current_state);
  }
};

template <class T>
struct transition_impl<T> {
  using type = transition_impl;
  template <class SM, class TEvent>
  static bool execute(SM &self, const TEvent &event, aux::byte &current_state) noexcept {
    return aux::get<T::value>(self.transitions_).execute(self, event, current_state);
  }
};

template <>
struct transition_impl<> {
  using type = transition_impl;
  template <class SM, class TEvent>
  static bool execute(SM &, const TEvent &, aux::byte &) noexcept {
    return false;
  }
};

template <class, class>
struct state_mappings;

template <class... S, class... Ts>
struct state_mappings<state<S...>, aux::type_list<Ts...>> {
  using element_type = state<S...>;
  using types = aux::type_list<Ts...>;
};

template <class, class>
struct event_mappings;

template <class E, class... Ts>
struct event_mappings<E, aux::inherit<Ts...>> {
  using element_type = event<E>;
  using types = aux::type_list<Ts...>;
};

template <class...>
struct unique_mappings;

template <class, class...>
struct unique_mappings_impl;

template <class... Ts>
using unique_mappings_t = typename unique_mappings<Ts...>::type;

template <class, class, class, class R>
struct get_mapping : aux::type_list<R> {};

template <class E, class T, class R>
struct get_mapping<event<E>, event<E>, T, R>
    : aux::type_list<
          event_mappings<E, aux::apply_t<unique_mappings_t, aux::join_t<typename R::types, typename T::types>>>> {};

template <class... S, class T, class R>
struct get_mapping<state<S...>, state<S...>, T, R>
    : aux::type_list<state_mappings<state<S...>, aux::join_t<typename R::types, typename T::types>>> {};

template <class T, class... Ts>
struct extend_mapping
    : aux::join_t<typename get_mapping<typename T::element_type, typename Ts::element_type, T, Ts>::type...> {};

template <class T, class... Ts>
using extend_mapping_t = aux::apply_t<aux::inherit, typename extend_mapping<T, Ts...>::type>;

template <bool, class, class...>
struct conditional_mapping;

template <class T1, class T2, class... Rs, class... Ts>
struct conditional_mapping<true, aux::type<T1, aux::inherit<Rs...>>, T2, Ts...> {
  using type = unique_mappings_impl<aux::type<aux::inherit<T1>, extend_mapping_t<T2, Rs...>>, Ts...>;
};

template <class T1, class T2, class... Rs, class... Ts>
struct conditional_mapping<false, aux::type<T1, aux::inherit<Rs...>>, T2, Ts...> {
  using type =
      unique_mappings_impl<aux::type<aux::inherit<T1, aux::type<typename T2::element_type>>, aux::inherit<T2, Rs...>>,
                           Ts...>;
};

template <class T1, class T2, class... Rs, class... Ts>
struct unique_mappings_impl<aux::type<T1, aux::inherit<Rs...>>, T2, Ts...>
    : conditional_mapping<aux::is_base_of<aux::type<typename T2::element_type>, T1>::value,
                          aux::type<T1, aux::inherit<Rs...>>, T2, Ts...>::type {};

template <class T1, class Rs>
struct unique_mappings_impl<aux::type<T1, Rs>> : aux::apply_t<aux::inherit, Rs> {};

template <class... Ts>
struct unique_mappings : unique_mappings_impl<aux::type<aux::none_type, aux::inherit<>>, Ts...> {};

template <class T>
struct unique_mappings<T> : aux::inherit<T> {};

template <class, class...>
struct mappings;

template <int... Ns, class... Ts>
struct mappings<aux::pool_impl<aux::index_sequence<Ns...>, Ts...>>
    : unique_mappings_t<event_mappings<
          typename Ts::event, aux::inherit<state_mappings<typename Ts::src_state,
                                                          aux::type_list<aux::integral_constant<int, Ns - 1>>>>>...> {};

template <class T>
using mappings_t = typename mappings<typename T::underlying_type>::type;

template <class>
transition_impl<> get_mapping_impl(...);

template <class T, class R>
R get_mapping_impl(event_mappings<T, R> *);

template <class T, class R>
aux::apply_t<transition_impl, R> get_mapping_impl(state_mappings<T, R> *);

template <class T, class U>
using get_mapping_t = decltype(get_mapping_impl<T>((U *)0));

template <class... Ts>
using merge_deps = aux::apply_t<aux::pool, aux::apply_t<aux::unique_t, aux::join_t<typename Ts::deps...>>>;

template <class, class TEvent>
struct get_events_impl {
  using type = aux::type_list<TEvent>;
};

template <class... Ts, class... S, class TEvent>
struct get_events_impl<state<sm_impl<Ts...>, S...>, TEvent> {
  using type = aux::join_t<aux::type_list<TEvent>, typename sm_impl<Ts...>::events>;
};

template <class... Ts>
using get_events = aux::join_t<typename get_events_impl<typename Ts::src_state, typename Ts::event>::type...>;

template <class... Ts>
using get_states = aux::join_t<aux::type_list<typename Ts::src_state, typename Ts::dst_state>...>;

template <class... Ts>
using get_composite_states = aux::join_t<aux::conditional_t<is_sm<Ts>::value, aux::type_list<Ts>, aux::type_list<>>...>;

template <class... Ts>
using count_initial_states = aux::count<is_initial, typename Ts::src_state...>;

template <class T, class... TDeps>
class sm_impl<T, aux::pool<TDeps...>> : public state<sm_impl<T, aux::pool<TDeps...>>> {
  using transitions_t = decltype(aux::declval<T>().configure());
  using mappings_t = detail::mappings_t<transitions_t>;
  using states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_states, transitions_t>>;
  using states_ids_t = aux::apply_t<aux::type_id, states_t>;
  using composite_states_t = aux::apply_t<get_composite_states, states_t>;
  static constexpr auto regions =
      aux::get_size<transitions_t>::value > 0 ? aux::apply_t<count_initial_states, transitions_t>::value : 1;

  static_assert(regions > 0, "At least one initial state is required");

  template <class...>
  friend struct transition;

  template <class...>
  friend struct transition_impl;

 public:
  using events = aux::apply_t<aux::unique_t, aux::apply_t<get_events, transitions_t>>;

  explicit sm_impl(TDeps... deps) noexcept : deps_{deps...}, transitions_(T{}.configure()) {
    initialize(aux::make_index_sequence<aux::get_size<transitions_t>::value>{});
  }
  explicit sm_impl(T &sm, TDeps... deps) noexcept : deps_{deps...}, transitions_(sm.configure()) {
    initialize(aux::make_index_sequence<aux::get_size<transitions_t>::value>{});
  }
  sm_impl(const sm_impl &) = delete;
  sm_impl(sm_impl &&) = default;

  template <class TEvent>
  bool process_event(const TEvent &event) noexcept {
    return process_event_impl<get_mapping_t<TEvent, mappings_t>>(event, states_t{},
                                                                 aux::make_index_sequence<regions>{});
  }

  template <class TVisitor>
  void visit_current_states(const TVisitor &visitor) const noexcept {
    visit_current_states_impl(visitor, states_t{}, aux::make_index_sequence<regions>{});
  }

  template <class TFlag>
  bool is(const TFlag &) const noexcept {
    auto result = false;
    visit_current_states([&](auto state) { result |= aux::is_base_of<TFlag, decltype(state)>::value; });
    return result;
  }

 private:
  template <int... Ns>
  void initialize(const aux::index_sequence<Ns...> &) noexcept {
    auto region = 0, i = region;
    int _[]{0,
            (region = i, current_state_[region] =
                             is_initial<typename decltype(aux::get<Ns - 1>(transitions_))::src_state>::value ? i++,
             aux::get_id<states_ids_t, 0, typename decltype(aux::get<Ns - 1>(transitions_))::src_state>() : 0, 0)...};
    (void)_;
    process_event(anonymous{});
  }

  void initialize(const aux::index_sequence<> &) noexcept {}

  template <class TMappings, class TEvent, class... TStates>
  auto process_event_impl(const TEvent &event, const aux::type_list<TStates...> &,
                          const aux::index_sequence<1> &) noexcept {
    static bool (*dispatch_table[])(sm_impl &, const TEvent &, aux::byte &) = {
        &get_mapping_t<TStates, TMappings>::template execute<sm_impl, TEvent>...};
    return dispatch_table[current_state_[0]](*this, event, current_state_[0]);
  }

  template <class TMappings, class TEvent, class... TStates, int... Ns>
  auto process_event_impl(const TEvent &event, const aux::type_list<TStates...> &,
                          const aux::index_sequence<Ns...> &) noexcept {
    static bool (*dispatch_table[])(sm_impl &, const TEvent &, aux::byte &) = {
        &get_mapping_t<TStates, TMappings>::template execute<sm_impl, TEvent>...};
    auto handled = false;
    int _[]{0, (handled |= dispatch_table[current_state_[Ns - 1]](*this, event, current_state_[Ns - 1]), 0)...};
    (void)_;
    return handled;
  }

  template <class TEvent, class... TStates>
  auto process_event_composite(const TEvent &event, const aux::type_list<TStates...> &) noexcept {
    static bool (*dispatch_table[])(const TEvent &) = {&sm_impl::process_event_composite_impl<TEvent, TStates>...};
    return dispatch_table[current_state_[0]](event);
  }

  template <class TEvent, class TState, aux::enable_if_t<!is_sm<TState>::value, int> = 0>
  static auto process_event_composite_impl(const TEvent &) noexcept {
    return false;
  }

  template <class TEvent, class TState, aux::enable_if_t<is_sm<TState>::value, int> = 0>
  static auto process_event_composite_impl(const TEvent &event) noexcept {
    return TState{}.process_event(event);
  }

  template <class TVisitor, class... TStates>
  void visit_current_states_impl(const TVisitor &visitor, const aux::type_list<TStates...> &,
                                 const aux::index_sequence<1> &) const noexcept {
    static void (*dispatch_table[])(const TVisitor &) = {&sm_impl::visit_state<TVisitor, TStates>...};
    dispatch_table[current_state_[0]](visitor);
  }

  template <class TVisitor, class... TStates, int... Ns>
  void visit_current_states_impl(const TVisitor &visitor, const aux::type_list<TStates...> &,
                                 const aux::index_sequence<Ns...> &) const noexcept {
    static void (*dispatch_table[])(const TVisitor &) = {&sm_impl::visit_state<TVisitor, TStates>...};
    int _[]{0, (dispatch_table[current_state_[Ns - 1]](visitor), 0)...};
    (void)_;
  }

  template <class TVisitor, class TState>
  static void visit_state(const TVisitor &visitor) noexcept {
    visitor(TState{});
  }

  aux::pool<TDeps...> deps_;
  transitions_t transitions_;
  aux::byte current_state_[regions];
};

}  // detail

template <class T>
auto operator!(const T &t) noexcept {
  return detail::not_<T>(t);
}

template <class T1, class T2>
auto operator&&(const T1 &t1, const T2 &t2) noexcept {
  return detail::and_<T1, T2>(t1, t2);
}

template <class T1, class T2>
auto operator||(const T1 &t1, const T2 &t2) noexcept {
  return detail::or_<T1, T2>(t1, t2);
}

template <class T1, class T2>
auto operator, (const T1 &t1, const T2 &t2) noexcept {
  return detail::seq_<T1, T2>(t1, t2);
}

template <class TEvent>
detail::event<TEvent> event{};

template <class T>
using state = detail::state<T>;

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wgnu-string-literal-operator-template"
#endif

#if !defined(_MSC_VER)
template <class T, T... Chars>
auto operator""_s() {
  return state<aux::string<Chars...>>{};
}
#endif

detail::initial_state initial;
detail::process_event process_event;

template <class... Ts>
auto make_transition_table(Ts... ts) noexcept {
  return aux::pool<Ts...>{ts...};
}

template <class T>
using sm = detail::sm_impl<T, aux::apply_t<detail::merge_deps, decltype(aux::declval<T>().configure())>>;

template <class TEvent, class TConfig>
struct dispatcher {
  template <class T>
  static void dispatch_event(const TEvent &e, T &sm) noexcept {
    dispatch_event(e, sm, typename T::events{});
  }

 private:
  template <class T, class E, class... Ts>
  static void dispatch_event(const TEvent &event, T &sm, const aux::type_list<E, Ts...> &) noexcept {
    if (TConfig::template get_id<E>() == TConfig::get_id(event)) {
      sm.process_event(TConfig::template get_event<E>(event));
    }
    dispatch_event(event, sm, aux::type_list<Ts...>{});
  }

  template <class T>
  static void dispatch_event(const TEvent &, T &, const aux::type_list<> &) noexcept {}
};
}  // v_1_0_0
}  // msm
