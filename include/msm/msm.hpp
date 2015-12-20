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
  using pool_impl<make_index_sequence<sizeof...(Ts)>, Ts...>::pool_impl;
};
template <class>
struct get_size;
template <class... Ts>
struct get_size<pool<Ts...>> {
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
template <class T>
struct sm__ : T {
  using T::process_event__;
};

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
template <class T>
struct get_state {
  using type = T;
};
template <class T>
struct get_state<state<T>> {
  using type = T;
};
template <class T>
using get_state_t = typename get_state<T>::type;

template <class>
struct is_sm_state : aux::false_type {};
template <class... Ts>
struct is_sm_state<sm_impl<Ts...>> : aux::true_type {};

template <class T>
auto match(const state_base *s1, const state_base *s2) {
  return s1 == s2;
}
template <>
auto match<any_state>(const state_base *, const state_base *) {
  return true;
}

template <class S1, class S2, class E, class G, class A>
struct transition<state_impl<S1>, state_impl<S2>, event_impl<E>, G, A> {
  using src_state = S1;
  using dst_state = S2;
  using event = E;
  using deps = aux::apply_t<aux::unique_t, aux::join_t<get_deps_t<G, E>, get_deps_t<A, E>>>;

  void init_state(const state_base *cs[], int &i) const noexcept { init_state_impl(S1{}, cs, i); }
  void init_state_impl(...) const noexcept {}
  template <class... Ts>
  void init_state_impl(const msm::init_state<Ts...> &, const state_base *cs[], int &i) const noexcept {
    cs[i++] = &s1;
  }
  template <class TEvent, class TDeps, class SM, aux::enable_if_t<aux::is_same<TEvent, E>::value, int> = 0>
  void process_event(const state_base **state, const TEvent &event, TDeps &deps, SM &sm, bool &handled) const noexcept {
    if (!handled && match<get_state_t<S1>>(*state, &s1)) {
      if (!process_event_via_sub_sm(event, is_sm_state<get_state_t<S1>>{}) && call(g, event, deps, sm)) {
        call(a, event, deps, sm);
        sm.process_event__(on_entry{});
        *state = &s2;
        sm.process_event__(on_exit{});
        handled = true;
      }
    }
  }

  template <class TEvent, class TDeps, class SM, aux::enable_if_t<!aux::is_same<TEvent, E>::value, int> = 0>
  void process_event(const state_base **state, const TEvent &event, TDeps &, SM &, bool &handled) const noexcept {
    if (!handled && match<get_state_t<S1>>(*state, &s1) &&
        process_event_via_sub_sm(event, is_sm_state<get_state_t<S1>>{})) {
      handled = true;
    }
  }

  template <class TEvent>
  auto process_event_via_sub_sm(const TEvent &event, const aux::true_type &) const noexcept {
    return ((sm__<get_state_t<S1>> &)s1).process_event__(event);
  }

  template <class TEvent>
  auto process_event_via_sub_sm(const TEvent &, const aux::false_type &) const noexcept {
    return false;
  }

  template <class TVisitor>
  void visit_state(const state_base *state, const TVisitor &visitor, bool &visited) const noexcept {
    if (!visited && match<get_state_t<S1>>(&s1, state)) {
      visitor(static_cast<const S1 &>(s1));
      visited = true;
    }

    if (!visited && match<get_state_t<S2>>(&s2, state)) {
      visitor(static_cast<const S2 &>(s2));
      visited = true;
    }
  }

  const state_impl<S1> &s1;
  const state_impl<S2> &s2;
  event_impl<E> e;
  G g;
  A a;
};

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
  using transitions_t = decltype(aux::declval<T>().configure());
  using indexes_t = aux::make_index_sequence<aux::get_size<transitions_t>::value>;
  static constexpr auto regions_nr = aux::apply_t<init_states_nr, transitions_t>::value;

  static_assert(regions_nr > 0, "At least one init state is required, 'msm::init_state<> idle;'");

 public:
  using events = aux::apply_t<aux::unique_t, aux::apply_t<get_event, transitions_t>>;

  explicit sm_impl(const T &fsm, TDeps... deps) noexcept : fsm_(fsm), deps_{deps...}, transitions_(fsm_.configure()) {
    init_states(indexes_t{});
    process_event_impl(indexes_t{}, anonymous{});
    process_event_impl(indexes_t{}, on_entry{});
  }

  ~sm_impl() noexcept { process_event_impl(indexes_t{}, on_exit{}); }

  template <class TEvent>
  void process_event(const TEvent &event) noexcept {
    if (!process_event_impl(indexes_t{}, event)) {
      process_event_impl(indexes_t{}, not_handled{});
    }
  }

  template <class TVisitor>
  void visit_current_states(const TVisitor &visitor) noexcept {
    visit_current_states_impl(indexes_t{}, visitor);
  }

 protected:
  template <class TEvent>
  auto process_event__(const TEvent &event) noexcept {
    return process_event_impl(indexes_t{}, event);
  }

 private:
  template <int... Ns>
  void init_states(const aux::index_sequence<Ns...> &) noexcept {
    auto i = 0;
    int _[]{0, (aux::get<Ns - 1>(transitions_).init_state(current_states_, i), 0)...};
    (void)_;
  }

  template <int... Ns, class TEvent>
  auto process_event_impl(const aux::index_sequence<Ns...> &, const TEvent &event) noexcept {
    auto handled = false;
    for (auto i = 0; i < regions_nr; ++i) {
      int _[]{0, (aux::get<Ns - 1>(transitions_)
                      .process_event(&current_states_[i], event, deps_, static_cast<sm__<sm_impl> &>(*this), handled),
                  0)...};
      (void)_;
    }
    return handled;
  }

  template <int... Ns, class TVisitor>
  void visit_current_states_impl(const aux::index_sequence<Ns...> &, const TVisitor &visitor) noexcept {
    for (const auto *state : current_states_) {
      auto visited = false;
      int _[]{0, (aux::get<Ns - 1>(transitions_).visit_state(state, visitor, visited), 0)...};
      (void)_;
    }
  }

  const T &fsm_;
  aux::pool<TDeps...> deps_;
  struct t : transitions_t {
    explicit t(const transitions_t &o) : transitions_t(o) {}
  };
  t transitions_;
  const state_base *current_states_[!regions_nr ? 1 : regions_nr];
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
