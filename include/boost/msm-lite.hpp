//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once
#if (__cplusplus < 201305L && _MSC_VER < 1900)
#error "Boost.MSM-lite requires C++14 support (Clang-3.4+, GCC-5.1+, MSVC-2015+)"
#else
#define BOOST_MSM_LITE_VERSION 1'0'1
#if !defined(BOOST_MSM_LITE_LOG)
#define BOOST_MSM_LITE_LOG(...)
#else
#define BOOST_MSM_LITE_LOG_ENABLED
#endif
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
#define BOOST_MSM_LITE_NOEXCEPT noexcept
#define BOOST_MSM_LITE_NOEXCEPT_IF(...) noexcept(__VA_ARGS__)
#else
#define BOOST_MSM_LITE_NOEXCEPT
#define BOOST_MSM_LITE_NOEXCEPT_IF(...)
#endif
#if !defined(__has_builtin)
#define __has_builtin(...) 0
#endif
#if !defined(__has_extension)
#define __has_extension(...) 0
#endif
namespace boost {
namespace msm {
namespace lite {
inline namespace v_1_0_1 {
namespace aux {
using byte = unsigned char;
struct none_type {};
template <char... Chrs>
struct string {
  static auto c_str() BOOST_MSM_LITE_NOEXCEPT {
    static char str[] = {Chrs..., 0};
    return str;
  }
};
template <class...>
struct type {};
template <class T, T>
struct non_type {};
template <class...>
struct type_list {
  using type = type_list;
};
template <bool...>
struct bool_list {
  using type = bool_list;
};
template <class... Ts>
struct inherit : Ts... {
  using type = inherit;
};
template <class T>
T &&declval() BOOST_MSM_LITE_NOEXCEPT;
template <class T, T V>
struct integral_constant {
  using type = integral_constant;
  static constexpr T value = V;
};
using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;
template <class>
struct always : aux::true_type {};
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
using is_base_of = integral_constant<bool, __is_base_of(T, U)>;
#if __has_extension(is_constructible)
template <class T, class... TArgs>
using is_constructible = integral_constant<bool, __is_constructible(T, TArgs...)>;
#else
template <class T, class... TArgs>
decltype(void(T(declval<TArgs>()...)), true_type{}) test_is_constructible(int);
template <class, class...>
false_type test_is_constructible(...);
template <class T, class... TArgs>
using is_constructible = decltype(test_is_constructible<T, TArgs...>(0));
#endif
template <class T>
using is_trivially_constructible = integral_constant<bool, __is_trivially_constructible(T)>;
template <class T>
struct remove_reference {
  using type = T;
};
template <class T>
struct remove_reference<T &> {
  using type = T;
};
template <class T>
struct remove_reference<const T &> {
  using type = T;
};
template <class T>
struct remove_reference<T &&> {
  using type = T;
};
template <class T>
using remove_reference_t = typename remove_reference<T>::type;
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
#if __has_builtin(__make_integer_seq)
template <class T, T...>
struct integer_sequence;
template <int... Ns>
struct integer_sequence<int, Ns...> {
  using type = index_sequence<Ns...>;
};
template <int N>
struct make_index_sequence_impl {
  using type = typename __make_integer_seq<integer_sequence, int, N>::type;
};
#else
template <class, class>
struct concat;
template <int... I1, int... I2>
struct concat<index_sequence<I1...>, index_sequence<I2...>> : index_sequence<I1..., (sizeof...(I1) + I2)...> {};
template <int N>
struct make_index_sequence_impl
    : concat<typename make_index_sequence_impl<N / 2>::type, typename make_index_sequence_impl<N - N / 2>::type>::type {};
template <>
struct make_index_sequence_impl<0> : index_sequence<> {};
template <>
struct make_index_sequence_impl<1> : index_sequence<0> {};
#endif
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
struct apply<T, U<Ts...>> {
  using type = T<Ts...>;
};
template <template <class...> class T, class D>
using apply_t = typename apply<T, D>::type;
template <int, class T>
struct tuple_type {
  T value;
};
template <class, class...>
struct tuple_impl;
template <int... Ns, class... Ts>
struct tuple_impl<index_sequence<Ns...>, Ts...> : tuple_type<Ns, Ts>... {
  using boost_di_inject__ = aux::type_list<Ts...>;
  explicit tuple_impl(Ts... ts) BOOST_MSM_LITE_NOEXCEPT : tuple_type<Ns, Ts>{ts}... {}
};
template <class... Ts>
using tuple = tuple_impl<make_index_sequence<sizeof...(Ts)>, Ts...>;
template <int N, class T>
auto &get_by_id_impl(tuple_type<N, T> *object) BOOST_MSM_LITE_NOEXCEPT {
  return static_cast<tuple_type<N, T> &>(*object).value;
}
template <int N, class Tuple>
auto &get_by_id(Tuple &t) BOOST_MSM_LITE_NOEXCEPT {
  return get_by_id_impl<N>(&t);
}
template <class T>
struct pool_type {
  T value;
};
struct init {};
template <class T>
auto try_get_impl(...) BOOST_MSM_LITE_NOEXCEPT {
  return aux::remove_reference_t<T>{};
}
template <class T>
auto &try_get_impl(pool_type<T> *object) BOOST_MSM_LITE_NOEXCEPT {
  return static_cast<pool_type<T> &>(*object).value;
}
template <class T>
auto &try_get_impl(pool_type<T &> *object) BOOST_MSM_LITE_NOEXCEPT {
  return static_cast<pool_type<T &> &>(*object).value;
}
template <class T, class TPool>
auto try_get(TPool &p) BOOST_MSM_LITE_NOEXCEPT -> decltype(try_get_impl<T>(&p)) {
  return try_get_impl<T>(&p);
}
template <class T, class TPool>
decltype(auto) get(TPool &p) BOOST_MSM_LITE_NOEXCEPT {
  return static_cast<pool_type<T> &>(p).value;
}
template <class... Ts>
struct pool : pool_type<Ts>... {
  using boost_di_inject__ = aux::type_list<Ts...>;
  explicit pool(Ts... ts) BOOST_MSM_LITE_NOEXCEPT : pool_type<Ts>{ts}... {}
  template <class... TArgs>
  pool(init &&, pool<TArgs...> &&p) BOOST_MSM_LITE_NOEXCEPT : pool_type<Ts>{aux::try_get<Ts>(p)}... {}
};
template <class>
struct is_pool : aux::false_type {};
template <class... Ts>
struct is_pool<pool<Ts...>> : aux::true_type {};
template <int, class T>
struct type_id_type {};
template <class, class...>
struct type_id_impl;
template <int... Ns, class... Ts>
struct type_id_impl<index_sequence<Ns...>, Ts...> : type_id_type<Ns, Ts>... {};
template <class... Ts>
struct type_id : type_id_impl<make_index_sequence<sizeof...(Ts)>, Ts...> {};
template <class T, int, int N>
constexpr auto get_id_impl(type_id_type<N, T> *) BOOST_MSM_LITE_NOEXCEPT {
  return N;
}
template <class T, int D>
constexpr auto get_id_impl(...) BOOST_MSM_LITE_NOEXCEPT {
  return D;
}
template <class TIds, int D, class T>
constexpr auto get_id() BOOST_MSM_LITE_NOEXCEPT {
  return get_id_impl<T, D>((TIds *)0);
}
template <class>
struct get_size;
template <template <class...> class T, class... Ts>
struct get_size<T<Ts...>> {
  static constexpr auto value = sizeof...(Ts);
};
template <int... Ts>
constexpr auto max() noexcept {
  auto max = 0;
  int _[]{0, (Ts > max ? max = Ts : max)...};
  (void)_;
  return max;
}
template <class... Ts>
struct variant {
  using ids_t = type_id<Ts...>;
  alignas(max<alignof(Ts)...>()) byte data[max<sizeof(Ts)...>()];
  int id = -1;

  template <class T>
  void init(T object) noexcept {
    id = get_id<ids_t, -1, T>();
    new (&data) T(static_cast<T &&>(object));
  }
};
}  // aux
namespace detail {
struct on_entry;
struct on_exit;
struct terminate_state;
}  // detail
namespace concepts {
#define BOOST_MSM_LITE_REQUIRES(...) typename aux::enable_if<__VA_ARGS__, int>::type = 0
template <class T>
decltype(aux::declval<T>().configure()) configurable_impl(int);
template <class>
void configurable_impl(...);
template <class T>
struct configurable : aux::is_pool<decltype(configurable_impl<T>(0))> {};
struct callable_fallback {
  void operator()();
};
template <class T>
aux::false_type test_callable(aux::non_type<void (callable_fallback::*)(), &T::operator()> *);
template <class>
aux::true_type test_callable(...);
template <class, class>
struct callable_impl : aux::false_type {};
template <class R>
struct callable_impl<R, aux::true_type> : aux::true_type {};
template <class R, class T>
using callable = callable_impl<R, decltype(test_callable<aux::inherit<T, callable_fallback>>(0))>;
template <class...>
struct is_valid_transition : aux::true_type {};
template <class S1, class S2, class... Ts>
struct is_valid_transition<S1, S2, detail::on_entry, Ts...> : aux::is_same<S1, S2> {};
template <class S1, class S2, class... Ts>
struct is_valid_transition<S1, S2, detail::on_exit, Ts...> : aux::is_same<S1, S2> {};
template <class... Ts>
struct is_valid_transition<detail::terminate_state, Ts...> {};
aux::false_type transitional_impl(...);
template <class T>
auto transitional_impl(T &&t) -> is_valid_transition<typename T::src_state, typename T::dst_state, typename T::event,
                                                     typename T::deps, decltype(T::initial), decltype(T::history)>;
template <class T>
struct transitional : decltype(transitional_impl(aux::declval<T>())) {};
template <class>
aux::false_type dispatchable_impl(...);
template <class...>
struct is_valid_event : aux::true_type {};
template <class, class TEvent>
auto dispatchable_impl(TEvent && ) -> is_valid_event<decltype(TEvent::id), decltype(TEvent())>;
template <class T, class TEvent>
auto dispatchable_impl(TEvent && ) -> is_valid_event<decltype(TEvent::id), decltype(TEvent(aux::declval<T>()))>;
template <class, class>
struct dispatchable;
template <class T, class... TEvents>
struct dispatchable<T, aux::type_list<TEvents...>>
    : aux::is_same<aux::bool_list<aux::always<TEvents>::value...>,
                   aux::bool_list<decltype(dispatchable_impl<T>(aux::declval<TEvents>()))::value...>> {};
}  // concepts
namespace detail {
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
template <class>
class sm;
template <class>
struct is_sm : aux::false_type {};
template <class T>
struct is_sm<sm<T>> : aux::true_type {};
struct fsm {
  auto configure() BOOST_MSM_LITE_NOEXCEPT { return aux::pool<>{}; }
};
struct _ {};
struct operator_base {};
struct internal_event {};
struct anonymous : internal_event {};
struct on_entry : internal_event {};
struct on_exit : internal_event {};
struct always {
  auto operator()() const BOOST_MSM_LITE_NOEXCEPT { return true; }
};
struct none {
  void operator()() BOOST_MSM_LITE_NOEXCEPT {}
};
struct process_event {
  template <class TEvent>
  struct process_event_impl {
    template <class SM, class T>
    void operator()(sm<SM> &sm, const T &) BOOST_MSM_LITE_NOEXCEPT {
      sm.process_event(event);
    }

    TEvent event;
  };

  template <class TEvent>
  auto operator()(const TEvent &event) BOOST_MSM_LITE_NOEXCEPT {
    return process_event_impl<TEvent>{event};
  }
};
#if defined(BOOST_MSM_LITE_DEFER_LIMIT_SIZE)
struct defer {
  template <class TEvent>
  auto operator()(const TEvent &) BOOST_MSM_LITE_NOEXCEPT {}
};
#endif
template <class>
struct event {
  template <class T, BOOST_MSM_LITE_REQUIRES(concepts::callable<bool, T>::value)>
  auto operator[](const T &t) const BOOST_MSM_LITE_NOEXCEPT {
    return transition_eg<event, T>{*this, t};
  }

  template <class T, BOOST_MSM_LITE_REQUIRES(concepts::callable<void, T>::value)>
  auto operator/(const T &t) const BOOST_MSM_LITE_NOEXCEPT {
    return transition_ea<event, T>{*this, t};
  }
};
template <class T>
struct exception {
  using type = T;
  T exception;
};
template <class TEvent>
struct unexpected_event {
  using type = TEvent;
  TEvent event;
};
struct initial_state {};
struct terminate_state {};
struct history_state {};
template <class>
struct state;
template <class>
struct state_str {
  static auto c_str() BOOST_MSM_LITE_NOEXCEPT { return __PRETTY_FUNCTION__; }
};
template <>
struct state_str<state<terminate_state>> {
  static auto c_str() BOOST_MSM_LITE_NOEXCEPT { return "terminate"; }
};
template <char... Chrs>
struct state_str<state<aux::string<Chrs...>>> : aux::string<Chrs...> {};
template <char... Chrs, class T>
struct state_str<state<aux::string<Chrs...>(T)>> : state_str<state<aux::string<Chrs...>>> {};
template <class TState>
struct state_impl : state_str<TState> {
  using explicit_states = aux::type_list<>;
  template <class T>
  auto operator<=(const T &t) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<TState, T>{static_cast<const TState &>(*this), t};
  }
  template <class T>
  auto operator+(const T &t) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<TState, T>{static_cast<const TState &>(*this), t};
  }
  template <class T, BOOST_MSM_LITE_REQUIRES(concepts::callable<bool, T>::value)>
  auto operator[](const T &t) const BOOST_MSM_LITE_NOEXCEPT {
    return transition_sg<TState, T>{static_cast<const TState &>(*this), t};
  }
  template <class T, BOOST_MSM_LITE_REQUIRES(concepts::callable<void, T>::value)>
  auto operator/(const T &t) const BOOST_MSM_LITE_NOEXCEPT {
    return transition_sa<TState, T>{static_cast<const TState &>(*this), t};
  }
};
template <class TState>
struct state : state_impl<state<TState>> {
  using type = TState;
  static constexpr auto initial = false;
  static constexpr auto history = false;
  auto operator*() const BOOST_MSM_LITE_NOEXCEPT { return state<TState(initial_state)>{}; }
  auto operator()(const initial_state &) const BOOST_MSM_LITE_NOEXCEPT { return state<TState(initial_state)>{}; }
  auto operator()(const history_state &) const BOOST_MSM_LITE_NOEXCEPT { return state<TState(history_state)>{}; }
  template <class... Ts>
  auto operator()(const state<Ts> &...) const BOOST_MSM_LITE_NOEXCEPT {
    return state<TState(Ts...)>{};
  }
  template <class T>
  auto operator=(const T &t) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<T, state>{t, *this};
  }
};
template <class TState>
struct state<TState(initial_state)> : state_impl<state<TState(initial_state)>> {
  using type = TState;
  static constexpr auto initial = true;
  static constexpr auto history = false;
  template <class T>
  auto operator=(const T &t) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<T, state>{t, *this};
  }
};
template <class TState>
struct state<TState(history_state)> : state_impl<state<TState(history_state)>> {
  using type = TState;
  static constexpr auto initial = true;
  static constexpr auto history = true;
  template <class T>
  auto operator=(const T &t) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<T, state>{t, *this};
  }
};
template <class TState, class... TExplicitStates>
struct state<TState(TExplicitStates...)> : state_impl<state<TState(TExplicitStates...)>> {
  using type = TState;
  using explicit_states = aux::type_list<TExplicitStates...>;
  static constexpr auto initial = false;
  static constexpr auto history = false;
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
template <class T, class E>
auto args__(int) -> aux::function_traits_t<decltype(&T::template operator() < fsm, E > )>;
template <class T, class E>
using args_t = decltype(args__<T, E>(0));
template <class, class>
struct ignore;
template <class E, class... Ts>
struct ignore<E, aux::type_list<Ts...>> {
  using type = aux::join_t<aux::conditional_t<aux::is_same<E, aux::remove_reference_t<Ts>>::value ||
                                                  aux::is_same<sm<fsm>, aux::remove_reference_t<Ts>>::value,
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
          aux::enable_if_t<!aux::is_same<TEvent, aux::remove_reference_t<T>>::value &&
                               !aux::is_same<sm<fsm>, aux::remove_reference_t<T>>::value,
                           int> = 0>
decltype(auto) get_arg(const TEvent &, TDeps &deps, SM &) BOOST_MSM_LITE_NOEXCEPT {
  return aux::get<T>(deps);
}
template <class T, class TEvent, class TDeps, class SM,
          aux::enable_if_t<aux::is_same<TEvent, aux::remove_reference_t<T>>::value, int> = 0>
decltype(auto) get_arg(const TEvent &event, TDeps &, SM &) BOOST_MSM_LITE_NOEXCEPT {
  return event;
}
template <class T, class TEvent, class TDeps, class SM,
          aux::enable_if_t<aux::is_same<sm<fsm>, aux::remove_reference_t<T>>::value, int> = 0>
decltype(auto) get_arg(const TEvent &, TDeps &, SM &sm) BOOST_MSM_LITE_NOEXCEPT {
  return sm;
}
#if defined(BOOST_MSM_LITE_LOG_ENABLED)
template <class... Ts, class T, class TEvent, class TDeps, class SM>
auto call_impl(const aux::type<void> &, const aux::type_list<Ts...> &, T object, const TEvent &event, TDeps &deps, sm<SM> &self)
    BOOST_MSM_LITE_NOEXCEPT_IF(sm<SM>::is_noexcept) {
  object(get_arg<Ts>(event, deps, self)...);
  BOOST_MSM_LITE_LOG(action, SM, object, event);
}
template <class... Ts, class T, class TEvent, class TDeps, class SM>
auto call_impl(const aux::type<bool> &, const aux::type_list<Ts...> &, T object, const TEvent &event, TDeps &deps, sm<SM> &self)
    BOOST_MSM_LITE_NOEXCEPT_IF(sm<SM>::is_noexcept) {
  auto result = object(get_arg<Ts>(event, deps, self)...);
  BOOST_MSM_LITE_LOG(guard, SM, object, event, result);
  return result;
}
#endif
template <class... Ts, class T, class TEvent, class TDeps, class SM,
          aux::enable_if_t<!aux::is_base_of<operator_base, T>::value, int> = 0>
auto call_impl(const aux::type_list<Ts...> &args, T object, const TEvent &event, TDeps &deps, sm<SM> &self)
    BOOST_MSM_LITE_NOEXCEPT_IF(sm<SM>::is_noexcept) {
#if defined(BOOST_MSM_LITE_LOG_ENABLED)
  using result_type = decltype(object(get_arg<Ts>(event, deps, self)...));
  return call_impl(aux::type<result_type>{}, args, object, event, deps, self);
#else
  (void)args;
  return object(get_arg<Ts>(event, deps, self)...);
#endif
}
template <class... Ts, class T, class TEvent, class TDeps, class SM,
          aux::enable_if_t<aux::is_base_of<operator_base, T>::value, int> = 0>
auto call_impl(const aux::type_list<Ts...> &, T object, const TEvent &event, TDeps &deps, sm<SM> &self)
    BOOST_MSM_LITE_NOEXCEPT_IF(sm<SM>::is_noexcept) {
  return object(event, deps, self);
}
template <class T, class TEvent, class TDeps, class SM>
auto call(T object, const TEvent &event, TDeps &deps, sm<SM> &self) BOOST_MSM_LITE_NOEXCEPT_IF(sm<SM>::is_noexcept) {
  return call_impl(args_t<T, TEvent>{}, object, event, deps, self);
}
template <class... Ts>
class seq_ : operator_base {
 public:
  explicit seq_(Ts... ts) BOOST_MSM_LITE_NOEXCEPT : a(ts...) {}

  template <class TEvent, class TDeps, class SM>
  void operator()(const TEvent &event, TDeps &deps, SM &self) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, deps, self);
  }

 private:
  template <int... Ns, class TEvent, class TDeps, class SM>
  void for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TDeps &deps, SM &self)
      BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    int _[]{0, (call(aux::get_by_id<Ns>(a), event, deps, self), 0)...};
    (void)_;
  }

  aux::tuple<Ts...> a;
};
template <class... Ts>
class and_ : operator_base {
 public:
  explicit and_(Ts... ts) BOOST_MSM_LITE_NOEXCEPT : g(ts...) {}

  template <class TEvent, class TDeps, class SM>
  auto operator()(const TEvent &event, TDeps &deps, SM &self) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    return for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, deps, self);
  }

 private:
  template <int... Ns, class TEvent, class TDeps, class SM>
  auto for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TDeps &deps, SM &self)
      BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    auto result = true;
    int _[]{0, (call(aux::get_by_id<Ns>(g), event, deps, self) ? result : result = false)...};
    (void)_;
    return result;
  }

  aux::tuple<Ts...> g;
};
template <class... Ts>
class or_ : operator_base {
 public:
  explicit or_(Ts... ts) BOOST_MSM_LITE_NOEXCEPT : g(ts...) {}

  template <class TEvent, class TDeps, class SM>
  auto operator()(const TEvent &event, TDeps &deps, SM &self) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    return for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, deps, self);
  }

 private:
  template <int... Ns, class TEvent, class TDeps, class SM>
  auto for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TDeps &deps, SM &self)
      BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    auto result = false;
    int _[]{0, (call(aux::get_by_id<Ns>(g), event, deps, self) ? result = true : result)...};
    (void)_;
    return result;
  }

  aux::tuple<Ts...> g;
};
template <class T>
class not_ : operator_base {
 public:
  explicit not_(T t) BOOST_MSM_LITE_NOEXCEPT : g(t) {}

  template <class TEvent, class TDeps, class SM>
  auto operator()(const TEvent &event, TDeps &deps, SM &self) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    return !call(g, event, deps, self);
  }

 private:
  T g;
};
template <class E, class G>
struct transition<event<E>, G> {
  template <class T>
  auto operator/(const T &t) const BOOST_MSM_LITE_NOEXCEPT {
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
template <class S2, class G, class A>
struct transition<state<S2>, G, A> : transition<state<S2>, state<S2>, event<anonymous>, G, A> {
  using transition<state<S2>, state<S2>, event<anonymous>, G, A>::g;
  using transition<state<S2>, state<S2>, event<anonymous>, G, A>::a;
  transition(const G &g, const A &a) : transition<state<S2>, state<S2>, event<anonymous>, G, A>{g, a} {}
  template <class T>
  auto operator=(const T &) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<T, state<S2>, event<anonymous>, G, A>{g, a};
  }
};
template <class S1, class S2>
struct transition<state<S1>, state<S2>> : transition<state<S1>, state<S2>, event<anonymous>, always, none> {
  transition(const state<S1> &, const state<S2> &)
      : transition<state<S1>, state<S2>, event<anonymous>, always, none>{always{}, none{}} {}
};
template <class S2, class G>
struct transition_sg<state<S2>, G> : transition<state<S2>, state<S2>, event<anonymous>, G, none> {
  using transition<state<S2>, state<S2>, event<anonymous>, G, none>::g;
  transition_sg(const state<S2> &, const G &g) : transition<state<S2>, state<S2>, event<anonymous>, G, none>{g, none{}} {}
  template <class T>
  auto operator/(const T &t) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<state<S2>, G, T>{g, t};
  }
  template <class T>
  auto operator=(const T &) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<T, state<S2>, event<anonymous>, G, none>{g, none{}};
  }
};
template <class S2, class A>
struct transition_sa<state<S2>, A> : transition<state<S2>, state<S2>, event<anonymous>, always, A> {
  using transition<state<S2>, state<S2>, event<anonymous>, always, A>::a;
  transition_sa(const state<S2> &, const A &a) : transition<state<S2>, state<S2>, event<anonymous>, always, A>{always{}, a} {}
  template <class T>
  auto operator=(const T &) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<T, state<S2>, event<anonymous>, always, A>{always{}, a};
  }
};
template <class S2, class E>
struct transition<state<S2>, event<E>> {
  template <class T>
  auto operator=(const T &) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<T, state<S2>, event<E>, always, none>{always{}, none{}};
  }
  const state<S2> &s2;
  event<E> e;
};
template <class E, class G>
struct transition_eg<event<E>, G> {
  template <class T>
  auto operator/(const T &t) const BOOST_MSM_LITE_NOEXCEPT {
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
template <class S1, class S2, class G, class A>
struct transition<state<S1>, transition<state<S2>, G, A>> : transition<state<S1>, state<S2>, event<anonymous>, G, A> {
  transition(const state<S1> &, const transition<state<S2>, G, A> &t)
      : transition<state<S1>, state<S2>, event<anonymous>, G, A>{t.g, t.a} {}
};
template <class S1, class E, class G, class A>
struct transition<state<S1>, transition<event<E>, G, A>> : transition<state<S1>, state<S1>, event<E>, G, A> {
  using transition<state<S1>, state<S1>, event<E>, G, A>::g;
  using transition<state<S1>, state<S1>, event<E>, G, A>::a;
  transition(const state<S1> &, const transition<event<E>, G, A> &t)
      : transition<state<S1>, state<S1>, event<E>, G, A>{t.g, t.a} {}
  template <class T>
  auto operator=(const T &) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<T, state<S1>, event<E>, G, A>{g, a};
  }
};
template <class S1, class S2, class E>
struct transition<state<S1>, transition<state<S2>, event<E>>> : transition<state<S1>, state<S2>, event<E>, always, none> {
  transition(const state<S1> &, const transition<state<S2>, event<E>> &)
      : transition<state<S1>, state<S2>, event<E>, always, none>{always{}, none{}} {}
};
template <class S1, class S2, class G>
struct transition<state<S1>, transition_sg<state<S2>, G>> : transition<state<S1>, state<S2>, event<anonymous>, G, none> {
  transition(const state<S1> &, const transition_sg<state<S2>, G> &t)
      : transition<state<S1>, state<S2>, event<anonymous>, G, none>{t.g, none{}} {}
};
template <class S1, class S2, class A>
struct transition<state<S1>, transition_sa<state<S2>, A>> : transition<state<S1>, state<S2>, event<anonymous>, always, A> {
  transition(const state<S1> &, const transition_sa<state<S2>, A> &t)
      : transition<state<S1>, state<S2>, event<anonymous>, always, A>{always{}, t.a} {}
};
template <class S2, class E, class G>
struct transition<state<S2>, transition_eg<event<E>, G>> : transition<state<S2>, state<S2>, event<E>, G, none> {
  using transition<state<S2>, state<S2>, event<E>, G, none>::g;
  transition(const state<S2> &, const transition_eg<event<E>, G> &t)
      : transition<state<S2>, state<S2>, event<E>, G, none>{t.g, none{}} {}
  template <class T>
  auto operator=(const T &) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<T, state<S2>, event<E>, G, none>{g, none{}};
  }
};
template <class S1, class S2, class E, class G>
struct transition<state<S1>, transition<state<S2>, transition_eg<event<E>, G>>>
    : transition<state<S1>, state<S2>, event<E>, G, none> {
  transition(const state<S1> &, const transition<state<S2>, transition_eg<event<E>, G>> &t)
      : transition<state<S1>, state<S2>, event<E>, G, none>{t.g, none{}} {}
};
template <class S2, class E, class A>
struct transition<state<S2>, transition_ea<event<E>, A>> : transition<state<S2>, state<S2>, event<E>, always, A> {
  using transition<state<S2>, state<S2>, event<E>, always, A>::a;
  transition(const state<S2> &, const transition_ea<event<E>, A> &t)
      : transition<state<S2>, state<S2>, event<E>, always, A>{always{}, t.a} {}
  template <class T>
  auto operator=(const T &) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<T, state<S2>, event<E>, always, A>{always{}, a};
  }
};
template <class S1, class S2, class E, class A>
struct transition<state<S1>, transition<state<S2>, transition_ea<event<E>, A>>>
    : transition<state<S1>, state<S2>, event<E>, always, A> {
  transition(const state<S1> &, const transition<state<S2>, transition_ea<event<E>, A>> &t)
      : transition<state<S1>, state<S2>, event<E>, always, A>{always{}, t.a} {}
};
template <class S1, class S2, class E, class G, class A>
struct transition<state<S1>, transition<state<S2>, transition<event<E>, G, A>>>
    : transition<state<S1>, state<S2>, event<E>, G, A> {
  transition(const state<S1> &, const transition<state<S2>, transition<event<E>, G, A>> &t)
      : transition<state<S1>, state<S2>, event<E>, G, A>{t.g, t.a} {}
};
template <class S1, class S2, class E, class G, class A>
struct transition<state<S1>, state<S2>, event<E>, G, A> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = G;
  using action = A;
  using deps = aux::apply_t<aux::unique_t, aux::join_t<get_deps_t<G, E>, get_deps_t<A, E>>>;

  transition(const G &g, const A &a) : g(g), a(a) {}

  template <class SM>
  auto execute(SM &self, const E &event, aux::byte &current_state) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    if (call(g, event, self.deps_, self)) {
      call(a, event, self.deps_, self);
      self.template update_current_state<typename state<S1>::explicit_states>(
          current_state, aux::get_id<typename SM::states_ids_t, -1, dst_state>(), state<src_state>{}, state<dst_state>{});
      return true;
    }
    return false;
  }

  G g;
  A a;
};
template <class S1, class S2, class E, class A>
struct transition<state<S1>, state<S2>, event<E>, always, A> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = always;
  using action = A;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<A, E>>;

  transition(const always &, const A &a) : a(a) {}

  template <class SM>
  auto execute(SM &self, const E &event, aux::byte &current_state) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    call(a, event, self.deps_, self);
    self.template update_current_state<typename state<S1>::explicit_states>(
        current_state, aux::get_id<typename SM::states_ids_t, -1, dst_state>(), state<src_state>{}, state<dst_state>{});
    return true;
  }

  A a;
};
template <class S1, class S2, class E, class G>
struct transition<state<S1>, state<S2>, event<E>, G, none> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = G;
  using action = none;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<G, E>>;

  transition(const G &g, const none &) : g(g) {}

  template <class SM>
  auto execute(SM &self, const E &event, aux::byte &current_state) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    if (call(g, event, self.deps_, self)) {
      self.template update_current_state<typename state<S1>::explicit_states>(
          current_state, aux::get_id<typename SM::states_ids_t, -1, dst_state>(), state<src_state>{}, state<dst_state>{});
      return true;
    }
    return false;
  }

  G g;
};
template <class S1, class S2, class E>
struct transition<state<S1>, state<S2>, event<E>, always, none> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = always;
  using action = none;
  using deps = aux::type_list<>;

  transition(const always &, const none &) {}

  template <class SM>
  auto execute(SM &self, const E &, aux::byte &current_state) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    self.template update_current_state<typename state<S1>::explicit_states>(
        current_state, aux::get_id<typename SM::states_ids_t, -1, dst_state>(), state<src_state>{}, state<dst_state>{});
    return true;
  }
};
template <class...>
struct transition_impl;
template <class T, class... Ts>
struct transition_impl<T, Ts...> {
  template <class SM, class TEvent>
  static bool execute(SM &self, const TEvent &event, aux::byte &current_state) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    if (aux::get<T>(self.transitions_).execute(self, event, current_state)) {
      return true;
    }
    return transition_impl<Ts...>::execute(self, event, current_state);
  }
};
template <class T>
struct transition_impl<T> {
  template <class SM, class TEvent>
  static bool execute(SM &self, const TEvent &event, aux::byte &current_state) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    return aux::get<T>(self.transitions_).execute(self, event, current_state);
  }
};
template <>
struct transition_impl<> {
  template <class SM, class TEvent>
  static bool execute(SM &, const TEvent &, aux::byte &) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    return false;
  }
};
template <class...>
struct transition_sub_impl;
template <class TSM, class T, class... Ts>
struct transition_sub_impl<TSM, T, Ts...> {
  template <class SM, class TEvent>
  static bool execute(SM &self, const TEvent &event, aux::byte &current_state) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    return aux::try_get<TSM>(self.deps_).process_event(event) ? true
                                                              : transition_impl<T, Ts...>::execute(self, event, current_state);
  }
};
template <class TSM>
struct transition_sub_impl<TSM> {
  template <class SM, class TEvent>
  static bool execute(SM &self, const TEvent &event, aux::byte &) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    return aux::try_get<TSM>(self.deps_).process_event(event);
  }
};
template <class, class>
struct state_mappings;
template <class S, class... Ts>
struct state_mappings<S, aux::type_list<Ts...>> {
  using element_type = state<S>;
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
    : aux::type_list<event_mappings<E, aux::apply_t<unique_mappings_t, aux::join_t<typename R::types, typename T::types>>>> {};
template <class S, class T, class R>
struct get_mapping<state<S>, state<S>, T, R>
    : aux::type_list<state_mappings<S, aux::join_t<typename R::types, typename T::types>>> {};
template <class T, class... Ts>
struct extend_mapping : aux::join_t<typename get_mapping<typename T::element_type, typename Ts::element_type, T, Ts>::type...> {
};
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
      unique_mappings_impl<aux::type<aux::inherit<T1, aux::type<typename T2::element_type>>, aux::inherit<T2, Rs...>>, Ts...>;
};
template <class T1, class T2, class... Rs, class... Ts>
struct unique_mappings_impl<aux::type<T1, aux::inherit<Rs...>>, T2, Ts...>
    : conditional_mapping<aux::is_base_of<aux::type<typename T2::element_type>, T1>::value, aux::type<T1, aux::inherit<Rs...>>,
                          T2, Ts...>::type {};
template <class T1, class Rs>
struct unique_mappings_impl<aux::type<T1, Rs>> : aux::apply_t<aux::inherit, Rs> {};
template <class... Ts>
struct unique_mappings : unique_mappings_impl<aux::type<aux::none_type, aux::inherit<>>, Ts...> {};
template <class T>
struct unique_mappings<T> : aux::inherit<T> {};
template <class, class...>
struct mappings;
template <class... Ts>
struct mappings<aux::pool<Ts...>>
    : unique_mappings_t<
          event_mappings<typename Ts::event, aux::inherit<state_mappings<typename Ts::src_state, aux::type_list<Ts>>>>...> {};
template <class T>
using mappings_t = typename mappings<T>::type;
template <class>
transition_impl<> get_state_mapping_impl(...);
template <class T, class... Ts>
transition_impl<Ts...> get_state_mapping_impl(state_mappings<T, aux::type_list<Ts...>> *);
template <class S>
transition_sub_impl<S> get_sub_state_mapping_impl(...);
template <class T, class... Ts>
transition_sub_impl<T, Ts...> get_sub_state_mapping_impl(state_mappings<T, aux::type_list<Ts...>> *);
template <class T, class U>
struct get_state_mapping {
  using type = decltype(get_state_mapping_impl<T>((U *)0));
};
template <class T, class U>
struct get_state_mapping<sm<T>, U> {
  using type = decltype(get_sub_state_mapping_impl<sm<T>>((U *)0));
};
template <class T, class U>
using get_state_mapping_t = typename get_state_mapping<T, U>::type;
template <class>
transition_impl<> get_event_mapping_impl(...);
template <class T, class TMappings>
TMappings get_event_mapping_impl(event_mappings<T, TMappings> *);
template <class T, class U>
using get_event_mapping_t = decltype(get_event_mapping_impl<T>((U *)0));
template <class, class TEvent>
struct get_all_events_impl {
  using type = aux::conditional_t<aux::is_base_of<internal_event, TEvent>::value, aux::type_list<>, aux::type_list<TEvent>>;
};
template <class T, class TEvent>
struct get_all_events_impl<sm<T>, TEvent> {
  using type = aux::join_t<aux::type_list<TEvent>, typename sm<T>::events>;
};
template <class... Ts>
using get_all_events = aux::join_t<typename get_all_events_impl<typename Ts::src_state, typename Ts::event>::type...>;
template <class... Ts>
using get_events = aux::type_list<typename Ts::event...>;
template <class T>
struct get_exception : aux::type_list<> {};
template <class T>
struct get_exception<exception<T>> : aux::type_list<exception<T>> {};
template <class... Ts>
using get_exceptions = aux::join_t<typename get_exception<Ts>::type...>;
template <class... Ts>
using get_states = aux::join_t<aux::type_list<typename Ts::src_state, typename Ts::dst_state>...>;
template <class... Ts>
using get_initial_states =
    aux::join_t<aux::conditional_t<Ts::initial, aux::type_list<typename Ts::src_state>, aux::type_list<>>...>;
template <class... Ts>
using get_history_states =
    aux::join_t<aux::conditional_t<!Ts::history && Ts::initial, aux::type_list<typename Ts::src_state>, aux::type_list<>>...>;
template <class T, class U = T>
using get_sm = aux::conditional_t<aux::is_trivially_constructible<T>::value, aux::type_list<U>, aux::type_list<U &>>;
template <class>
struct get_sub_sm : aux::type_list<> {};
template <class T>
struct get_sub_sm<sm<T>> : get_sm<T, sm<T>> {};
template <class... Ts>
using get_sub_sms = aux::join_t<typename get_sub_sm<Ts>::type...>;
template <class... Ts>
using merge_deps = aux::apply_t<aux::unique_t, aux::join_t<typename Ts::deps...>>;
template <class SM>
class sm {
  template <class>
  friend class sm;
  template <class>
  friend struct state;
  template <class...>
  friend struct transition;
  template <class...>
  friend struct transition_impl;
  template <class...>
  friend struct transition_sub_impl;

  using transitions_t = decltype(aux::declval<SM>().configure());
  using mappings_t = detail::mappings_t<transitions_t>;
  using states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_states, transitions_t>>;
  using states_ids_t = aux::apply_t<aux::type_id, states_t>;
  using initial_states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_initial_states, transitions_t>>;
  using initial_states_ids_t = aux::apply_t<aux::type_id, initial_states_t>;
  using initial_but_not_history_states_t = aux::apply_t<get_history_states, transitions_t>;
  using has_history_states = aux::integral_constant<bool, aux::get_size<initial_states_t>::value !=
                                                              aux::get_size<initial_but_not_history_states_t>::value>;
  using sub_sms_t = aux::apply_t<get_sub_sms, states_t>;
  using events_t = aux::apply_t<aux::unique_t, aux::apply_t<get_events, transitions_t>>;
  using events_ids_t = aux::apply_t<aux::pool, events_t>;
  using deps_t = aux::apply_t<aux::pool, aux::join_t<get_sm<SM>, sub_sms_t, aux::apply_t<detail::merge_deps, transitions_t>>>;
  static constexpr auto regions = aux::get_size<initial_states_t>::value > 0 ? aux::get_size<initial_states_t>::value : 1;
  static_assert(regions > 0, "At least one initial state is required");

  template <class... TDeps>
  using dependable = aux::is_same<aux::bool_list<aux::always<TDeps>::value...>,
                                  aux::bool_list<aux::is_base_of<aux::pool_type<TDeps>, deps_t>::value...>>;

  template <class... TStates>
  using get_ids = aux::index_sequence<aux::get_id<states_ids_t, -1, TStates>()...>;

 public:
  using states = states_t;
  using events = aux::apply_t<aux::unique_t, aux::apply_t<get_all_events, transitions_t>>;
  using transitions = aux::apply_t<aux::type_list, transitions_t>;

#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
  using exceptions = aux::apply_t<aux::unique_t, aux::apply_t<get_exceptions, events_t>>;
  static constexpr auto is_noexcept = BOOST_MSM_LITE_NOEXCEPT_IF(aux::declval<SM>().configure());
#endif

  sm(sm &&) BOOST_MSM_LITE_NOEXCEPT = default;
  sm(const sm &) BOOST_MSM_LITE_NOEXCEPT = delete;
  sm &operator=(const sm &) BOOST_MSM_LITE_NOEXCEPT = delete;

  template <class... TDeps, BOOST_MSM_LITE_REQUIRES(dependable<TDeps...>::value)>
  explicit sm(TDeps &&... deps) BOOST_MSM_LITE_NOEXCEPT : deps_{aux::init{}, aux::pool<TDeps...>{deps...}},
                                                          transitions_(aux::try_get<SM>(deps_).configure()) {
    initialize(initial_states_t{});
  }

  explicit sm(deps_t &&deps) BOOST_MSM_LITE_NOEXCEPT : deps_(deps), transitions_(aux::try_get<SM>(deps_).configure()) {
    initialize(initial_states_t{});
  }

  template <class TEvent>
  bool process_event(const TEvent &event) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    BOOST_MSM_LITE_LOG(process_event, SM, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    return process_event_noexcept(event, aux::integral_constant<bool, is_noexcept>{});
#else
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, states_t{}, aux::make_index_sequence<regions>{});
#endif
  }

  template <class TEvent>
  bool process_event(const event<TEvent> &) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    return process_event(TEvent{});
  }

  template <class TVisitor, BOOST_MSM_LITE_REQUIRES(concepts::callable<void, TVisitor>::value)>
  void visit_current_states(const TVisitor &visitor) const
      BOOST_MSM_LITE_NOEXCEPT_IF(BOOST_MSM_LITE_NOEXCEPT_IF(visitor(state<initial_state>{}))) {
    visit_current_states_impl(visitor, states_t{}, aux::make_index_sequence<regions>{});
  }

  template <class TState>
  bool is(const state<TState> &) const BOOST_MSM_LITE_NOEXCEPT {
    auto result = false;
    visit_current_states([&](auto state) { result |= aux::is_same<TState, typename decltype(state)::type>::value; });
    return result;
  }

  template <class... TStates, BOOST_MSM_LITE_REQUIRES(sizeof...(TStates) == regions)>
  bool is(const state<TStates> &...) const BOOST_MSM_LITE_NOEXCEPT {
    auto result = true;
    auto i = 0;
    int state_ids[] = {aux::get_id<states_ids_t, 0, TStates>()...};
    visit_current_states(
        [&](auto state) { result &= (aux::get_id<states_ids_t, 0, typename decltype(state)::type>() == state_ids[i++]); });
    return result;
  }

 private:
  template <class... TStates>
  void initialize(const aux::type_list<TStates...> &) BOOST_MSM_LITE_NOEXCEPT {
    auto region = 0, i = region;
    int _[]{0, (region = i, current_state_[region] = aux::get_id<states_ids_t, 0, TStates>(), ++i, 0)...};
    (void)_;
    process_internal_event(anonymous{});
  }

  void initialize(const aux::type_list<> &) BOOST_MSM_LITE_NOEXCEPT {}

  template <class TEvent, BOOST_MSM_LITE_REQUIRES(aux::is_base_of<aux::pool_type<TEvent>, events_ids_t>::value)>
  bool process_internal_event(const TEvent &event) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    return process_event(event);
  }

  bool process_internal_event(...) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) { return false; }

  template <class TMappings, class TEvent, class... TStates>
  auto process_event_impl(const TEvent &event, const aux::type_list<TStates...> &, const aux::index_sequence<0> &)
      BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    static bool (*dispatch_table[!sizeof...(TStates) ? 1 : sizeof...(TStates)])(
        sm &, const TEvent &, aux::byte &) = {&get_state_mapping_t<TStates, TMappings>::template execute<sm, TEvent>...};
    return dispatch_table[current_state_[0]](*this, event, current_state_[0]);
  }

  template <class TMappings, class TEvent, class... TStates, int... Ns>
  auto process_event_impl(const TEvent &event, const aux::type_list<TStates...> &, const aux::index_sequence<Ns...> &)
      BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    static bool (*dispatch_table[!sizeof...(TStates) ? 1 : sizeof...(TStates)])(
        sm &, const TEvent &, aux::byte &) = {&get_state_mapping_t<TStates, TMappings>::template execute<sm, TEvent>...};
    auto handled = false;
    int _[]{0, (handled |= dispatch_table[current_state_[Ns]](*this, event, current_state_[Ns]), 0)...};
    (void)_;
    return handled;
  }

#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
  template <class TEvent>
  bool process_event_noexcept(const TEvent &event, const aux::true_type &) noexcept {
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, states_t{}, aux::make_index_sequence<regions>{});
  }

  template <class TEvent>
  bool process_event_noexcept(const TEvent &event, const aux::false_type &) {
    try {
      return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, states_t{},
                                                                         aux::make_index_sequence<regions>{});
    } catch (...) {
      return process_exception(exceptions{});
    }
  }

  bool process_exception(const aux::type_list<> &) { return process_event(exception<_>{}); }

  template <class E, class... Es>
  bool process_exception(const aux::type_list<E, Es...> &) {
    try {
      throw;
    } catch (const typename E::type &e) {
      return process_event(E{e});
    } catch (...) {
      return process_exception(aux::type_list<Es...>{});
    }
  }
#endif

  template <class TVisitor, class... TStates>
  void visit_current_states_impl(const TVisitor &visitor, const aux::type_list<TStates...> &,
                                 const aux::index_sequence<0> &) const BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    static void (*dispatch_table[!sizeof...(TStates) ? 1 : sizeof...(TStates)])(const TVisitor &) = {
        &sm::visit_state<TVisitor, TStates>...};
    dispatch_table[current_state_[0]](visitor);
  }

  template <class TVisitor, class... TStates, int... Ns>
  void visit_current_states_impl(const TVisitor &visitor, const aux::type_list<TStates...> &,
                                 const aux::index_sequence<Ns...> &) const BOOST_MSM_LITE_NOEXCEPT {
    static void (*dispatch_table[!sizeof...(TStates) ? 1 : sizeof...(TStates)])(const TVisitor &) = {
        &sm::visit_state<TVisitor, TStates>...};
    int _[]{0, (dispatch_table[current_state_[Ns]](visitor), 0)...};
    (void)_;
  }

  template <class TVisitor, class TState>
  static void visit_state(const TVisitor &visitor) BOOST_MSM_LITE_NOEXCEPT {
    visitor(state<TState>{});
  }

  template <class, class TState>
  void update_current_state(aux::byte &, const aux::byte &, const TState &, const TState &)
      BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {}

  template <class, class TSrcState, class TDstState>
  void update_current_state(aux::byte &current_state, const aux::byte &new_state, const TSrcState &src_state,
                            const TDstState &dst_state) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    process_internal_event(on_exit{});
    BOOST_MSM_LITE_LOG(state_change, SM, src_state, dst_state);
    (void)src_state;
    (void)dst_state;
    current_state = new_state;
    process_internal_event(on_entry{});
  }

  template <class TExplicit, class TSrcState, class T>
  void update_current_state(aux::byte &current_state, const aux::byte &new_state, const TSrcState &src_state,
                            const state<sm<T>> &dst_state) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    process_internal_event(on_exit{});
    BOOST_MSM_LITE_LOG(state_change, SM, src_state, dst_state);
    (void)src_state;
    (void)dst_state;
    current_state = new_state;
    process_internal_event(on_entry{});
    update_composite_states<sm<T>>(TExplicit{}, typename sm<T>::has_history_states{},
                                   typename sm<T>::initial_but_not_history_states_t{});
  }

  template <class T, class... Ts>  // explicit
  void update_composite_states(const aux::type_list<Ts...> &, ...) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    auto &sm = aux::try_get<T>(deps_);
    int _[]{0, (sm.current_state_[sm.template get_region<Ts>()] = aux::get_id<typename T::states_ids_t, -1, Ts>(), 0)...};
    (void)_;
    // TODO
  }

  template <class T, class... Ts, class... THs>  // history states, no explicit
  void update_composite_states(const aux::type_list<> &, const aux::true_type &, const aux::type_list<THs...> &)
      BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    auto &sm = aux::try_get<T>(deps_);
    int _[]{0, (sm.current_state_[aux::get_id<typename T::initial_states_ids_t, -1, THs>()] =
                    aux::get_id<typename T::states_ids_t, -1, THs>(),
                0)...};
    (void)_;
  }

  template <class T>  // just initials, no explicit
  void update_composite_states(const aux::type_list<> &, const aux::false_type &, ...) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    aux::try_get<T>(deps_).initialize(typename T::initial_states_t{});
  }

  template <class TState>
  static constexpr auto get_region() noexcept {
    return get_region_impl(aux::get_id<states_ids_t, -1, TState>(), aux::apply_t<get_ids, initial_states_t>{});
  }

  template <int... Ids>
  static constexpr auto get_region_impl(int id, const aux::index_sequence<Ids...> &) noexcept {
    auto region = 0, i = 0;
    int _[]{0, (id < Ids ? region : region = i, ++i)...};
    (void)_;
    return region;
  }

  deps_t deps_;
  transitions_t transitions_;

#if defined(BOOST_MSM_LITE_DEFER_LIMIT_SIZE)
#endif

 protected:
  aux::byte current_state_[regions];
};
template <class TEvent = void>
struct dispatch_event_impl {
  template <class SM, class T>
  static bool execute(SM &sm, const T &data) BOOST_MSM_LITE_NOEXCEPT {
    return execute_impl(sm, data, aux::is_constructible<TEvent, T>{});
  }

  template <class SM, class T>
  static bool execute_impl(SM &sm, const T &data, const aux::true_type &) BOOST_MSM_LITE_NOEXCEPT {
    return sm.process_event(TEvent(data));
  }

  template <class SM, class T>
  static bool execute_impl(SM &sm, const T &, const aux::false_type &) BOOST_MSM_LITE_NOEXCEPT {
    return sm.process_event(TEvent());
  }
};
template <>
struct dispatch_event_impl<void> {
  template <class SM, class T>
  static bool execute(SM &, const T &) BOOST_MSM_LITE_NOEXCEPT {
    return false;
  }
};
template <int, class>
struct events_ids_impl {};
template <class... Ts>
struct event_id : events_ids_impl<Ts::id, Ts>... {};
template <int>
dispatch_event_impl<> get_event_impl(...);
template <int N, class T>
dispatch_event_impl<T> get_event_impl(events_ids_impl<N, T> *);
template <int N, class T>
struct get_event {
  using type = decltype(get_event_impl<N>((T *)0));
};
template <int N, class T>
using get_event_t = typename get_event<N, T>::type;
template <class TEvent, int EventRangeBegin, class SM, int... Ns>
auto make_dispatch_table(sm<SM> &fsm, const aux::index_sequence<Ns...> &) BOOST_MSM_LITE_NOEXCEPT {
  using events_ids = aux::apply_t<event_id, typename sm<SM>::events>;
  return [&](const TEvent &event, int id) {
    static bool (*dispatch_table[])(
        sm<SM> &, const TEvent &) = {&get_event_t<Ns + EventRangeBegin, events_ids>::template execute<sm<SM>, TEvent>...};
    return dispatch_table[id - EventRangeBegin](fsm, event);
  };
}
}  // detail
namespace testing {
template <class T>
class sm : public detail::sm<T> {
  using states_ids_t = aux::apply_t<aux::type_id, typename detail::sm<T>::states>;

 public:
  using detail::sm<T>::sm;

  template <class... TStates>
  void set_current_states(const detail::state<TStates> &...) BOOST_MSM_LITE_NOEXCEPT {
    auto region = 0, i = region;
    int _[]{0, (region = i, detail::sm<T>::current_state_[region] = aux::get_id<states_ids_t, 0, TStates>(), ++i, 0)...};
    (void)_;
  }
};
}  // testing
template <class T, BOOST_MSM_LITE_REQUIRES(concepts::callable<bool, T>::value)>
auto operator!(const T &t)BOOST_MSM_LITE_NOEXCEPT {
  return detail::not_<T>(t);
}
template <class T1, class T2,
          BOOST_MSM_LITE_REQUIRES(concepts::callable<bool, T1>::value &&concepts::callable<bool, T2>::value)>
auto operator&&(const T1 &t1, const T2 &t2) BOOST_MSM_LITE_NOEXCEPT {
  return detail::and_<T1, T2>(t1, t2);
}
template <class T1, class T2,
          BOOST_MSM_LITE_REQUIRES(concepts::callable<bool, T1>::value &&concepts::callable<bool, T2>::value)>
auto operator||(const T1 &t1, const T2 &t2) BOOST_MSM_LITE_NOEXCEPT {
  return detail::or_<T1, T2>(t1, t2);
}
template <class T1, class T2,
          BOOST_MSM_LITE_REQUIRES(concepts::callable<void, T1>::value &&concepts::callable<void, T2>::value)>
auto operator, (const T1 &t1, const T2 &t2)BOOST_MSM_LITE_NOEXCEPT {
  return detail::seq_<T1, T2>(t1, t2);
}
template <class TEvent>
detail::event<TEvent> event{};
auto on_entry = event<detail::on_entry>;
auto on_exit = event<detail::on_exit>;
template <class T = detail::_>
detail::event<detail::exception<T>> exception{};
template <class T>
using state = detail::state<T>;
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wgnu-string-literal-operator-template"
#endif
#if !defined(_MSC_VER)
template <class T, T... Chrs>
auto operator""_s() BOOST_MSM_LITE_NOEXCEPT {
  return state<aux::string<Chrs...>>{};
}
template <class T, T... Chrs>
auto operator""_t() BOOST_MSM_LITE_NOEXCEPT {
  return event<aux::string<Chrs...>>;
}
#endif
detail::state<detail::terminate_state> X;
detail::history_state H;
detail::process_event process_event;
template <class... Ts, BOOST_MSM_LITE_REQUIRES(aux::is_same<aux::bool_list<aux::always<Ts>::value...>,
                                                            aux::bool_list<concepts::transitional<Ts>::value...>>::value)>
auto make_transition_table(Ts... ts) BOOST_MSM_LITE_NOEXCEPT {
  return aux::pool<Ts...>{ts...};
}
template <class T, BOOST_MSM_LITE_REQUIRES(concepts::configurable<T>::value)>
using sm = detail::sm<T>;
template <class TEvent, int EventRangeBegin, int EventRangeEnd, class SM,
          BOOST_MSM_LITE_REQUIRES(concepts::dispatchable<TEvent, typename sm<SM>::events>::value)>
auto make_dispatch_table(sm<SM> &fsm) BOOST_MSM_LITE_NOEXCEPT {
  static_assert(EventRangeEnd - EventRangeBegin > 0, "Event ids range difference has to be greater than 0");
  return detail::make_dispatch_table<TEvent, EventRangeBegin>(fsm,
                                                              aux::make_index_sequence<EventRangeEnd - EventRangeBegin + 1>{});
}
}  // v_1_0_1
}  // lite
}  // msm
}  // boost
#endif
