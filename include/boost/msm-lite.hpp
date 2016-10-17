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
#define BOOST_MSM_LITE_VERSION 1'1'0
#define BOOST_MSM_LITE_NAMESPACE_BEGIN \
  namespace boost {                    \
  namespace msm {                      \
  namespace lite {                     \
  inline namespace v1_1_0 {
#define BOOST_MSM_LITE_NAMESPACE_END \
  }                                  \
  }                                  \
  }                                  \
  }
#if !defined(__has_builtin)
#define __has_builtin(...) 0
#endif
#if !defined(__has_extension)
#define __has_extension(...) 0
#endif
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-string-literal-operator-template"
#pragma clang diagnostic ignored "-Wzero-length-array"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
BOOST_MSM_LITE_NAMESPACE_BEGIN
#define BOOST_MSM_LITE_REQUIRES(...) typename aux::enable_if<__VA_ARGS__, int>::type = 0
namespace aux {
using byte = unsigned char;
struct none_type {};
template <char... Chrs>
struct string {
  static auto c_str() {
    static constexpr char str[] = {Chrs..., 0};
    return str;
  }
};
template <class...>
struct type {};
template <class T, T>
struct non_type {};
template <class, class>
struct pair {};
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
T &&declval();
template <class T, T V>
struct integral_constant {
  using type = integral_constant;
  static constexpr T value = V;
};
using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;
template <class...>
using void_t = void;
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
template <class T, class... TArgs>
decltype(void(T(declval<TArgs>()...)), true_type{}) test_is_constructible(int);
template <class, class...>
false_type test_is_constructible(...);
template <class T, class... TArgs>
using is_constructible = decltype(test_is_constructible<T, TArgs...>(0));
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
}
namespace aux {
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
    : concat<typename make_index_sequence_impl<N / 2>::type, typename make_index_sequence_impl<N - N / 2>::type>::type {};
template <>
struct make_index_sequence_impl<0> : index_sequence<> {};
template <>
struct make_index_sequence_impl<1> : index_sequence<0> {};
template <int N>
using make_index_sequence = typename make_index_sequence_impl<N>::type;
template <class... Ts>
struct join {
  using type = type_list<>;
};
template <class T>
struct join<T> {
  using type = T;
};
template <class... T1s, class... T2s, class... Ts>
struct join<type_list<T1s...>, type_list<T2s...>, Ts...> : join<type_list<T1s..., T2s...>, Ts...> {};
template <class... Ts, class... T1s, class... T2s, class... T3s, class... T4s, class... T5s, class... T6s, class... T7s,
          class... T8s, class... T9s, class... T10s, class... T11s, class... T12s, class... T13s, class... T14s, class... T15s,
          class... T16s, class... Us>
struct join<type_list<Ts...>, type_list<T1s...>, type_list<T2s...>, type_list<T3s...>, type_list<T4s...>, type_list<T5s...>,
            type_list<T6s...>, type_list<T7s...>, type_list<T8s...>, type_list<T9s...>, type_list<T10s...>, type_list<T11s...>,
            type_list<T12s...>, type_list<T13s...>, type_list<T14s...>, type_list<T15s...>, type_list<T16s...>, Us...>
    : join<type_list<Ts..., T1s..., T2s..., T3s..., T4s..., T5s..., T6s..., T7s..., T8s..., T9s..., T10s..., T11s..., T12s...,
                     T13s..., T14s..., T15s..., T16s...>,
           Us...> {};
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
  explicit tuple_impl(Ts... ts) : tuple_type<Ns, Ts>{ts}... {}
};
template <>
struct tuple_impl<index_sequence<0>> {
  aux::byte _[0];
};
template <class... Ts>
using tuple = tuple_impl<make_index_sequence<sizeof...(Ts)>, Ts...>;
template <int N, class T>
auto &get_by_id_impl(tuple_type<N, T> *object) {
  return static_cast<tuple_type<N, T> &>(*object).value;
}
template <int N, class Tuple>
auto &get_by_id(Tuple &t) {
  return get_by_id_impl<N>(&t);
}
template <class T>
struct pool_type {
  T value;
};
struct init {};
template <class T>
aux::remove_reference_t<T> try_get(...) {
  return {};
}
template <class T>
T &try_get(pool_type<T> *object) {
  return static_cast<pool_type<T> &>(*object).value;
}
template <class T>
T &try_get(pool_type<T &> *object) {
  return static_cast<pool_type<T &> &>(*object).value;
}
template <class T, class TPool>
decltype(auto) get(TPool &p) {
  return static_cast<pool_type<T> &>(p).value;
}
template <class... Ts>
struct pool : pool_type<Ts>... {
  using boost_di_inject__ = aux::type_list<Ts...>;
  explicit pool(Ts... ts) : pool_type<Ts>{ts}... {}
  template <class... TArgs>
  pool(init &&, pool<TArgs...> &&p) : pool_type<Ts>{aux::try_get<Ts>(&p)}... {}
  template <class... TArgs>
  pool(const pool<TArgs...> &p) : pool_type<Ts>{{init{}, &p}}... {}
};
template <>
struct pool<> {
  explicit pool(...) {}
  aux::byte _[0];
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
constexpr auto get_id_impl(type_id_type<N, T> *) {
  return N;
}
template <class T, int D>
constexpr auto get_id_impl(...) {
  return D;
}
template <class TIds, int D, class T>
constexpr auto get_id() {
  return get_id_impl<T, D>((TIds *)0);
}
template <class>
struct size;
template <template <class...> class T, class... Ts>
struct size<T<Ts...>> {
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
  template <class T>
  variant(T object) noexcept {
    id = get_id<ids_t, -1, T>();
    new (&data) T(static_cast<T &&>(object));
  }
  int id = -1;
};
template <class TExpr, class = void>
struct zero_wrapper : TExpr {
  explicit zero_wrapper(const TExpr &expr) : TExpr(expr) {}
};
template <class, class>
struct zero_wrapper_impl;
template <class TExpr, class... TArgs>
struct zero_wrapper_impl<TExpr, aux::type_list<TArgs...>> {
  auto operator()(TArgs... args) const { return reinterpret_cast<const TExpr &>(*this)(args...); }
  aux::byte _[0];
};
template <class TExpr>
struct zero_wrapper<TExpr, void_t<decltype(+declval<TExpr>())>>
    : zero_wrapper_impl<TExpr, aux::function_traits_t<decltype(&TExpr::operator())>> {
  zero_wrapper(...) {}
};
}
namespace concepts {
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
}
namespace concepts {
template <class T>
decltype(aux::declval<T>()()) configurable_impl(int);
template <class>
void configurable_impl(...);
template <class T>
struct configurable : aux::is_pool<decltype(configurable_impl<T>(0))> {};
}
namespace concepts {
template <class T, class = decltype(T::c_str())>
aux::true_type test_stringable(const T &);
aux::false_type test_stringable(...);
template <class T, class = void>
struct stringable : aux::false_type {};
template <class T>
struct stringable<T, decltype(void(sizeof(T)))> : decltype(test_stringable(aux::declval<T>())) {};
}
namespace detail {
struct on_entry;
struct on_exit;
struct terminate_state;
}
namespace concepts {
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
}
namespace detail {
struct internal_event {
  static auto c_str() { return "internal_event"; }
};
struct anonymous : internal_event {
  static auto c_str() { return "anonymous"; }
};
struct on_entry : internal_event {
  static auto c_str() { return "on_entry"; }
};
struct on_exit : internal_event {
  static auto c_str() { return "on_exit"; }
};
}
namespace detail {
template <class>
class sm;
template <class>
class sm_impl;
template <class>
struct state;
template <class>
struct event;
template <class>
struct exception;
template <class...>
struct transitions;
template <class...>
struct transitions_sub;
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
transitions<> get_state_mapping_impl(...);
template <class T, class... Ts>
transitions<Ts...> get_state_mapping_impl(state_mappings<T, aux::type_list<Ts...>> *);
template <class S>
transitions_sub<S> get_sub_state_mapping_impl(...);
template <class T, class... Ts>
transitions_sub<T, Ts...> get_sub_state_mapping_impl(state_mappings<T, aux::type_list<Ts...>> *);
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
transitions<> get_event_mapping_impl(...);
template <class T, class TMappings>
TMappings get_event_mapping_impl(event_mappings<T, TMappings> *);
template <class T, class U>
using get_event_mapping_t = decltype(get_event_mapping_impl<T>((U *)0));
}
namespace detail {
struct thread_safety_policy__ {};
struct defer_queue_policy__ {};
struct logger_policy__ {};
template <class, class, class TDeps, class TEvent>
void log_process_event(const aux::false_type &, TDeps &, const TEvent &) {}
template <class TLogger, class SM, class TDeps, class TEvent>
void log_process_event(const aux::true_type &, TDeps &deps, const TEvent &event) {
  return static_cast<aux::pool_type<TLogger> &>(deps).value.template log_process_event<SM>(event);
}
template <class, class, class TDeps, class TSrcState, class TDstState>
void log_state_change(const aux::false_type &, TDeps &, const TSrcState &, const TDstState &) {}
template <class TLogger, class SM, class TDeps, class TSrcState, class TDstState>
void log_state_change(const aux::true_type &, TDeps &deps, const TSrcState &src, const TDstState &dst) {
  return static_cast<aux::pool_type<TLogger> &>(deps).value.template log_state_change<SM>(src, dst);
}
template <class, class, class TDeps, class TAction, class TEvent>
void log_action(const aux::false_type &, TDeps &, const TAction &, const TEvent &) {}
template <class TLogger, class SM, class TDeps, class TAction, class TEvent>
void log_action(const aux::true_type &, TDeps &deps, const TAction &action, const TEvent &event) {
  return static_cast<aux::pool_type<TLogger> &>(deps).value.template log_action<SM>(action, event);
}
template <class, class, class TDeps, class TGuard, class TEvent>
void log_guard(const aux::false_type &, TDeps &, const TGuard &, const TEvent &, bool) {}
template <class TLogger, class SM, class TDeps, class TGuard, class TEvent>
void log_guard(const aux::true_type &, TDeps &deps, const TGuard &guard, const TEvent &event, bool result) {
  return static_cast<aux::pool_type<TLogger> &>(deps).value.template log_guard<SM>(guard, event, result);
}
struct no_policy {
  using type = no_policy;
  template <class>
  using rebind = no_policy;
  aux::byte _[0];
};
}
namespace detail {
template <class...>
struct transitions;
template <class...>
struct transitions_sub;
template <class T, class... Ts>
struct transitions<T, Ts...> {
  template <class SM, class TEvent>
  static bool execute(SM &self, const TEvent &event, aux::byte &current_state) {
    if (aux::get<T>(self.me_.transitions_).execute(self, event, current_state)) {
      return true;
    }
    return transitions<Ts...>::execute(self, event, current_state);
  }
};
template <class T>
struct transitions<T> {
  template <class SM, class TEvent>
  static bool execute(SM &self, const TEvent &event, aux::byte &current_state) {
    return aux::get<T>(self.me_.transitions_).execute(self, event, current_state);
  }
  template <class SM, class>
  static bool execute(SM &self, const on_entry &event, aux::byte &current_state) {
    aux::get<T>(self.me_.transitions_).execute(self, event, current_state);
    return false;
  }
  template <class SM, class>
  static bool execute(SM &self, const on_exit &event, aux::byte &current_state) {
    aux::get<T>(self.me_.transitions_).execute(self, event, current_state);
    return false;
  }
};
template <>
struct transitions<> {
  template <class SM, class TEvent>
  static bool execute(SM &, const TEvent &, aux::byte &) {
    return false;
  }
};
template <class TSM, class T, class... Ts>
struct transitions_sub<sm<TSM>, T, Ts...> {
  template <class SM, class TEvent>
  static bool execute(SM &self, const TEvent &event, aux::byte &current_state) {
    const auto handled = aux::try_get<sm_impl<TSM>>(&self.sub_sms_).process_event(event, self.deps_, self.sub_sms_);
    return handled ? handled : transitions<T, Ts...>::execute(self, event, current_state);
  }
};
template <class TSM>
struct transitions_sub<sm<TSM>> {
  template <class SM, class TEvent>
  static bool execute(SM &self, const TEvent &event, aux::byte &) {
    return aux::try_get<sm_impl<TSM>>(&self.sub_sms_).process_event(event, self.deps_, self.sub_sms_);
  }
};
}
namespace detail {
template <class T>
struct exception {
  using type = T;
  T exception;
};
struct _ {};
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
template <class>
no_policy get_policy(...);
template <class T, class TPolicy>
TPolicy get_policy(aux::pair<T, TPolicy> *);
template <class SM, class... TPolicies>
struct sm_policy {
  using sm = SM;
  using thread_safety_policy = decltype(get_policy<detail::thread_safety_policy__>((aux::inherit<TPolicies...> *)0));
  using defer_queue_policy = decltype(get_policy<defer_queue_policy__>((aux::inherit<TPolicies...> *)0));
  using logger_policy = decltype(get_policy<logger_policy__>((aux::inherit<TPolicies...> *)0));
};
template <class>
struct get_sub_sm : aux::type_list<> {};
template <class T>
struct get_sub_sm<sm<T>> : aux::join_t<aux::type_list<T>, typename sm<T>::sub_sms> {};
template <class... Ts>
using get_sub_sms = aux::join_t<typename get_sub_sm<Ts>::type...>;
template <class... Ts>
using get_sm_t = aux::type_list<typename Ts::sm...>;
template <class... Ts>
using merge_deps = aux::join_t<typename Ts::deps...>;
template <class TSM>
class sm_impl {
  template <class>
  friend class sm_impl;
  template <class>
  friend class sm;
  template <class>
  friend struct state;
  template <class...>
  friend struct transition;
  template <class...>
  friend struct transitions;
  template <class...>
  friend struct transitions_sub;

 public:
  template <class T>
  using rebind = sm<sm_policy<T>>;
  using sm_t = typename TSM::sm;
  using sm_raw_t = aux::remove_reference_t<sm_t>;
  using thread_safety_t = typename TSM::thread_safety_policy::type;
  template <class T>
  using defer_queue_t = typename TSM::defer_queue_policy::template rebind<T>;
  using logger_t = typename TSM::logger_policy::type;
  using has_logger = aux::integral_constant<bool, !aux::is_same<logger_t, no_policy>::value>;
  using transitions_t = decltype(aux::declval<sm_raw_t>()());
  using mappings_t = detail::mappings_t<transitions_t>;
  using states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_states, transitions_t>>;
  using states_ids_t = aux::apply_t<aux::type_id, states_t>;
  using initial_states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_initial_states, transitions_t>>;
  using initial_states_ids_t = aux::apply_t<aux::type_id, initial_states_t>;
  using initial_but_not_history_states_t = aux::apply_t<get_history_states, transitions_t>;
  using has_history_states =
      aux::integral_constant<bool, aux::size<initial_states_t>::value != aux::size<initial_but_not_history_states_t>::value>;
  using events_t = aux::apply_t<aux::unique_t, aux::apply_t<get_events, transitions_t>>;
  using events_ids_t = aux::apply_t<aux::pool, events_t>;
  using defer = aux::apply_t<aux::variant, events_t>;
  using defer_t = defer_queue_t<defer>;
  using deps = aux::apply_t<merge_deps, transitions_t>;
  static constexpr auto regions = aux::size<initial_states_t>::value > 0 ? aux::size<initial_states_t>::value : 1;
  static_assert(regions > 0, "At least one initial state is required");
  template <class... TStates>
  using get_ids = aux::index_sequence<aux::get_id<states_ids_t, -1, TStates>()...>;

 public:
  sm_impl(const aux::init &, const aux::pool_type<sm_raw_t &> *t) : transitions_((t->value)()) {
    initialize(typename sm_impl<TSM>::initial_states_t{});
  }
  sm_impl(const aux::init &, ...) : transitions_(sm_t{}()) { initialize(typename sm_impl<TSM>::initial_states_t{}); }
  sm_impl(sm_impl &&) = default;
  sm_impl(const sm_impl &) = delete;
  sm_impl &operator=(const sm_impl &) = delete;
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
  using exceptions = aux::apply_t<aux::unique_t, aux::apply_t<get_exceptions, events_t>>;
  using has_exceptions = aux::integral_constant<bool, (aux::size<exceptions>::value > 0)>;
#endif
  template <class TEvent, class TDeps, class TSub>
  bool process_event(const TEvent &event, TDeps &deps, TSub &sub) {
    log_process_event<logger_t, sm_raw_t>(has_logger{}, deps, event);
    struct self {
      using type __attribute__((unused)) = sm_impl;
      TDeps &deps_;
      sm_impl &me_;
      TSub &sub_sms_;
      defer_t &defer_;
    } self_{deps, *this, sub, defer_};
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    const auto handled = process_event_noexcept(event, self_, has_exceptions{});
#else
    const auto handled = process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self_, states_t{},
                                                                                     aux::make_index_sequence<regions>{});
#endif
    process_internal_event(self_, anonymous{});
    process_defer_events(self_, handled, aux::type<defer_queue_t<TEvent>>{}, events_t{});
    return handled;
  }
  template <class TDeps, class TSub>
  void start(TDeps &deps, TSub &sub) {
    struct self {
      using type __attribute__((unused)) = sm_impl;
      TDeps &deps_;
      sm_impl &me_;
      TSub &sub_sms_;
      defer_t &defer_;
    } self_{deps, *this, sub, defer_};
    process_internal_event(self_, anonymous{});
    process_internal_event(self_, on_entry{});
  }

 private:
  void initialize(const aux::type_list<> &) {}
  template <class... TStates>
  void initialize(const aux::type_list<TStates...> &) {
    auto region = 0, i = region;
    int _[]{0, (region = i, current_state_[region] = aux::get_id<states_ids_t, 0, TStates>(), ++i, 0)...};
    (void)_;
  }
  bool process_internal_event(...) { return false; }
  template <class TSelf, class TEvent, BOOST_MSM_LITE_REQUIRES(aux::is_base_of<aux::pool_type<TEvent>, events_ids_t>::value)>
  bool process_internal_event(TSelf &self, const TEvent &event) {
    log_process_event<logger_t, sm_raw_t>(has_logger{}, self.deps_, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    return process_event_noexcept(event, self, has_exceptions{});
#else
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self, states_t{},
                                                                       aux::make_index_sequence<regions>{});
#endif
  }
  template <class TSelf, class TEvent, BOOST_MSM_LITE_REQUIRES(aux::is_base_of<aux::pool_type<TEvent>, events_ids_t>::value)>
  bool process_internal_event(TSelf &self, const TEvent &event, aux::byte &current_state) {
    log_process_event<logger_t, sm_raw_t>(has_logger{}, self.deps_, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    return process_event_noexcept(event, self, current_state, has_exceptions{});
#else
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self, states_t{}, current_state);
#endif
  }
  template <class TMappings, class TEvent, class TSelf, class... TStates>
  bool process_event_impl(const TEvent &event, TSelf &self, const aux::type_list<TStates...> &,
                          const aux::index_sequence<0> &) {
    static bool (*dispatch_table[sizeof...(TStates)])(
        TSelf &, const TEvent &, aux::byte &) = {&get_state_mapping_t<TStates, TMappings>::template execute<TSelf, TEvent>...};
    const auto lock = create_lock(aux::type<thread_safety_t>{});
    (void)lock;
    return dispatch_table[current_state_[0]](self, event, current_state_[0]);
  }
  template <class TMappings, class TEvent, class TSelf, class... TStates, int... Ns>
  bool process_event_impl(const TEvent &event, TSelf &self, const aux::type_list<TStates...> &,
                          const aux::index_sequence<Ns...> &) {
    static bool (*dispatch_table[sizeof...(TStates)])(
        TSelf &, const TEvent &, aux::byte &) = {&get_state_mapping_t<TStates, TMappings>::template execute<TSelf, TEvent>...};
    auto handled = false;
    const auto lock = create_lock(aux::type<thread_safety_t>{});
    (void)lock;
    int _[]{0, (handled |= dispatch_table[current_state_[Ns]](self, event, current_state_[Ns]), 0)...};
    (void)_;
    return handled;
  }
  template <class TMappings, class TEvent, class TSelf, class... TStates>
  bool process_event_impl(const TEvent &event, TSelf &self, const aux::type_list<TStates...> &, aux::byte &current_state) {
    static bool (*dispatch_table[sizeof...(TStates)])(
        TSelf &, const TEvent &, aux::byte &) = {&get_state_mapping_t<TStates, TMappings>::template execute<TSelf, TEvent>...};
    const auto lock = create_lock(aux::type<thread_safety_t>{});
    (void)lock;
    return dispatch_table[current_state](self, event, current_state);
  }
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
  template <class TEvent, class TSelf>
  bool process_event_noexcept(const TEvent &event, TSelf &self, const aux::false_type &) noexcept {
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self, states_t{},
                                                                       aux::make_index_sequence<regions>{});
  }
  template <class TEvent, class TSelf>
  bool process_event_noexcept(const TEvent &event, TSelf &self, aux::byte &current_state, const aux::false_type &) noexcept {
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self, states_t{}, current_state);
  }
  template <class TEvent, class TSelf>
  bool process_event_noexcept(const TEvent &event, TSelf &self, const aux::true_type &) {
    try {
      return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self, states_t{},
                                                                         aux::make_index_sequence<regions>{});
    } catch (...) {
      return process_exception(self, exceptions{});
    }
  }
  template <class TSelf>
  bool process_exception(TSelf &self, const aux::type_list<> &) {
    return process_event(exception<_>{}, self.deps_, self.sub_sms_);
  }
  template <class TSelf, class E, class... Es>
  bool process_exception(TSelf &self, const aux::type_list<E, Es...> &) {
    try {
      throw;
    } catch (const typename E::type &e) {
      return process_event(E{e}, self.deps_, self.sub_sms_);
    } catch (...) {
      return process_exception(self, aux::type_list<Es...>{});
    }
  }
#endif
  template <class TSelf, class... TEvents>
  void process_defer_events(TSelf &, const bool, const aux::type<detail::no_policy> &, const aux::type_list<TEvents...> &) {}
  template <class TSelf, class TEvent>
  bool process_event_no_deffer(TSelf &self, const void *data) {
    const auto &event = *static_cast<const TEvent *>(data);
    log_process_event<logger_t, sm_raw_t>(has_logger{}, self.deps_, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    const auto handled = process_event_noexcept(event, self, has_exceptions{});
#else
    const auto handled = process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self, states_t{},
                                                                                     aux::make_index_sequence<regions>{});
#endif
    if (handled) {
      event.~TEvent();
      defer_.pop();
    }
    return handled;
  }
  template <class TSelf, class T, class... TEvents>
  void process_defer_events(TSelf &self, const bool handled, const aux::type<T> &, const aux::type_list<TEvents...> &) {
    if (handled) {
      auto size = defer_.size();
      static bool (sm_impl::*dispatch_event[sizeof...(TEvents)])(
          TSelf &, const void *) = {&sm_impl::process_event_no_deffer<TSelf, TEvents>...};
      while (size-- && (this->*dispatch_event[defer_.front().id])(self, defer_.front().data))
        ;
    }
  }
  template <class TVisitor, class... TStates>
  void visit_current_states_impl(const TVisitor &visitor, const aux::type_list<TStates...> &,
                                 const aux::index_sequence<0> &) const {
    static void (*dispatch_table[sizeof...(TStates)])(const TVisitor &) = {&sm_impl::visit_state<TVisitor, TStates>...};
    dispatch_table[current_state_[0]](visitor);
  }
  template <class TVisitor, class... TStates, int... Ns>
  void visit_current_states_impl(const TVisitor &visitor, const aux::type_list<TStates...> &,
                                 const aux::index_sequence<Ns...> &) const {
    static void (*dispatch_table[sizeof...(TStates)])(const TVisitor &) = {&sm_impl::visit_state<TVisitor, TStates>...};
    int _[]{0, (dispatch_table[current_state_[Ns]](visitor), 0)...};
    (void)_;
  }
  template <class TVisitor, class TState>
  static void visit_state(const TVisitor &visitor) {
    visitor(state<TState>{});
  }
  template <class, class TSelf, class TState>
  void update_current_state(TSelf &, aux::byte &, const aux::byte &, const TState &, const TState &) {}
  template <class TExplicit, class TSelf, class TSrcState, class TDstState>
  void update_current_state(TSelf &self, aux::byte &current_state, const aux::byte &new_state, const TSrcState &src_state,
                            const TDstState &dst_state) {
    update_current_state_impl<TExplicit>(self, current_state, new_state, src_state, dst_state);
  }
  template <class, class TSelf, class TSrcState, class TDstState>
  void update_current_state_impl(TSelf &self, aux::byte &current_state, const aux::byte &new_state, const TSrcState &src_state,
                                 const TDstState &dst_state) {
    process_internal_event(self, on_exit{}, current_state);
    log_state_change<logger_t, sm_raw_t>(has_logger{}, self.deps_, src_state, dst_state);
    (void)src_state;
    (void)dst_state;
    current_state = new_state;
    process_internal_event(self, on_entry{}, current_state);
  }
  template <class TExplicit, class TSelf, class TSrcState, class T>
  void update_current_state_impl(TSelf &self, aux::byte &current_state, const aux::byte &new_state, const TSrcState &src_state,
                                 const state<sm<T>> &dst_state) {
    process_internal_event(self, on_exit{}, current_state);
    log_state_change<logger_t, sm_raw_t>(has_logger{}, self.deps_, src_state, dst_state);
    (void)src_state;
    (void)dst_state;
    current_state = new_state;
    process_internal_event(self, on_entry{}, current_state);
    update_composite_states<sm_impl<T>>(self, TExplicit{}, typename sm_impl<T>::has_history_states{},
                                        typename sm_impl<T>::initial_but_not_history_states_t{});
  }
  template <class T, class TSelf, class... Ts>
  void update_composite_states(TSelf &self, const aux::type_list<Ts...> &, ...) {
    auto &sm = aux::try_get<T>(&self.sub_sms_);
    int _[]{0, (sm.current_state_[sm.template get_region<Ts>()] = aux::get_id<typename T::states_ids_t, -1, Ts>(), 0)...};
    (void)_;
  }
  template <class T, class TSelf, class... Ts, class... THs>
  void update_composite_states(TSelf &self, const aux::type_list<> &, const aux::true_type &, const aux::type_list<THs...> &) {
    auto &sm = aux::try_get<T>(&self.sub_sms_);
    int _[]{0, (sm.current_state_[aux::get_id<typename T::initial_states_ids_t, -1, THs>()] =
                    aux::get_id<typename T::states_ids_t, -1, THs>(),
                0)...};
    (void)_;
  }
  template <class T, class TSelf>
  void update_composite_states(TSelf &self, const aux::type_list<> &, const aux::false_type &, ...) {
    aux::try_get<T>(&self.sub_sms_).initialize(typename T::initial_states_t{});
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
  auto create_lock(const aux::type<detail::no_policy> &) { return detail::no_policy{}; }
  template <class T>
  auto create_lock(const aux::type<T> &) {
    struct lock_guard {
      explicit lock_guard(thread_safety_t &synch) : thread_safety_(synch) { thread_safety_.lock(); }
      ~lock_guard() { thread_safety_.unlock(); }
      thread_safety_t &thread_safety_;
    };
    return lock_guard{thread_safety_};
  }
  transitions_t transitions_;

 public:
  aux::conditional_t<(aux::size<states_t>::value > 0xFF), unsigned short, aux::byte> current_state_[regions];

 private:
  thread_safety_t thread_safety_;
  defer_t defer_;
};
template <class TSM>
class sm {
 public:
  using type = TSM;
  template <class T>
  using rebind = sm<sm_policy<T>>;
  using sm_t = typename TSM::sm;
  using sm_raw_t = aux::remove_reference_t<sm_t>;
  template <class T>
  using defer_queue_t = typename TSM::defer_queue_policy::template rebind<T>;
  using logger_t = typename TSM::logger_policy::type;
  using transitions_t = decltype(aux::declval<sm_raw_t>()());
  using states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_states, transitions_t>>;
  template <class>
  struct convert;
  template <class... Ts>
  struct convert<aux::type_list<Ts...>> {
    using type = aux::type_list<sm_impl<Ts>...>;
  };
  using sub_sms = aux::apply_t<get_sub_sms, states_t>;
  using sm_all_t = aux::apply_t<aux::inherit, aux::join_t<aux::type_list<sm_raw_t>, aux::apply_t<get_sm_t, sub_sms>>>;
  using sub_sms_t =
      aux::apply_t<aux::pool, typename convert<aux::join_t<aux::type_list<TSM>, aux::apply_t<get_sub_sms, states_t>>>::type>;
  using deps = aux::apply_t<merge_deps, transitions_t>;
  using deps_t = aux::apply_t<
      aux::pool, aux::apply_t<aux::unique_t, aux::join_t<deps, aux::type_list<logger_t>, aux::apply_t<merge_deps, sub_sms_t>>>>;
  template <class... TDeps>
  using dependable = aux::is_same<aux::bool_list<aux::always<TDeps>::value...>,
                                  aux::bool_list<aux::is_base_of<aux::remove_reference_t<TDeps>, sm_all_t>::value ||
                                                 aux::is_base_of<aux::pool_type<TDeps>, deps_t>::value...>>;

 public:
  using states = typename sm_impl<TSM>::states_t;
  using events = aux::apply_t<aux::unique_t, aux::apply_t<get_all_events, transitions_t>>;
  using transitions = aux::apply_t<aux::type_list, transitions_t>;
  sm(sm &&) = default;
  sm(const sm &) = delete;
  sm &operator=(const sm &) = delete;
  template <class... TDeps, BOOST_MSM_LITE_REQUIRES(dependable<TDeps...>::value)>
  explicit sm(TDeps &&... deps) : deps_{aux::init{}, aux::pool<TDeps...>{deps...}}, sub_sms_{aux::pool<TDeps...>{deps...}} {
    start(aux::type<sub_sms_t>{});
  }
  explicit sm(deps_t &deps) : deps_(deps), sub_sms_{deps} { start(aux::type<sub_sms_t>{}); }
  template <class TEvent>
  void process_event(const TEvent &event) {
    static_cast<aux::pool_type<sm_impl<TSM>> &>(sub_sms_).value.process_event(event, deps_, sub_sms_);
  }
  template <class TEvent>
  void process_event(const event<TEvent> &) {
    process_event(TEvent{});
  }
  template <class TVisitor, BOOST_MSM_LITE_REQUIRES(concepts::callable<void, TVisitor>::value)>
  void visit_current_states(const TVisitor &visitor) const {
    using states_t = typename sm_impl<TSM>::states_t;
    constexpr auto regions = sm_impl<TSM>::regions;
    static_cast<const aux::pool_type<sm_impl<TSM>> &>(sub_sms_).value.visit_current_states_impl(
        visitor, states_t{}, aux::make_index_sequence<regions>{});
  }
  template <class TState>
  bool is(const state<TState> &) const {
    auto result = false;
    visit_current_states([&](auto state) { result |= aux::is_same<TState, typename decltype(state)::type>::value; });
    return result;
  }
  template <class... TStates, BOOST_MSM_LITE_REQUIRES(sizeof...(TStates) == sm_impl<TSM>::regions)>
  bool is(const state<TStates> &...) const {
    auto result = true;
    auto i = 0;
    using states_ids_t = typename sm_impl<TSM>::states_ids_t;
    int state_ids[] = {aux::get_id<states_ids_t, 0, TStates>()...};
    visit_current_states(
        [&](auto state) { result &= (aux::get_id<states_ids_t, 0, typename decltype(state)::type>() == state_ids[i++]); });
    return result;
  }

 private:
  template <class... Ts>
  void start(const aux::type<aux::pool<Ts...>> &) {
    int _[]{0, (static_cast<aux::pool_type<Ts> &>(sub_sms_).value.start(deps_, sub_sms_), 0)...};
    (void)_;
  }

 protected:
  deps_t deps_;
  sub_sms_t sub_sms_;
};
}
namespace detail {
struct action_base {};
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
}
namespace detail {
struct defer : action_base {
  template <class TSelf, class TEvent>
  void operator()(TSelf &self, const TEvent &event) {
    self.defer_.push(event);
  }
};
}
namespace detail {
struct queue {
  template <class TEvent>
  class queue_impl : public action_base {
   public:
    explicit queue_impl(const TEvent &event) : event(event) {}
    template <class TSelf, class T>
    void operator()(TSelf &self, const T &) {
      self.me_.process_event(event, self.deps_, self.sub_sms_);
    }

   private:
    TEvent event;
  };
  template <class TEvent>
  auto operator()(const TEvent &event) {
    return queue_impl<TEvent>{event};
  }
};
}
namespace detail {
template <class>
struct event {
  template <class T, BOOST_MSM_LITE_REQUIRES(concepts::callable<bool, T>::value)>
  auto operator[](const T &t) const {
    return transition_eg<event, aux::zero_wrapper<T>>{*this, aux::zero_wrapper<T>{t}};
  }
  template <class T, BOOST_MSM_LITE_REQUIRES(concepts::callable<void, T>::value)>
  auto operator/(const T &t) const {
    return transition_ea<event, aux::zero_wrapper<T>>{*this, aux::zero_wrapper<T>{t}};
  }
};
template <class TEvent>
struct unexpected_event {
  using type = TEvent;
  TEvent event;
};
}
namespace detail {
struct operator_base {};
template <class TEvent>
struct event_type {
  using type = TEvent;
};
template <class TEvent>
struct event_type<exception<TEvent>> {
  using type = TEvent;
};
template <class TEvent>
using event_type_t = typename event_type<TEvent>::type;
template <class, class>
aux::type_list<> args__(...);
template <class T, class>
auto args__(int) -> aux::function_traits_t<T>;
template <class T, class E>
auto args__(int) -> aux::function_traits_t<decltype(&T::template operator() < event_type_t<E>>)>;
template <class T, class>
auto args__(int) -> aux::function_traits_t<decltype(&T::operator())>;
template <class T, class E>
using args_t = decltype(args__<T, E>(0));
template <class T, class TEvent, class TSelf,
          aux::enable_if_t<!aux::is_same<TEvent, aux::remove_reference_t<T>>::value, int> = 0>
decltype(auto) get_arg(const TEvent &, TSelf &self) {
  return aux::get<T>(self.deps_);
}
template <class, class TEvent, class TSelf>
decltype(auto) get_arg(const exception<TEvent> &event, TSelf &) {
  return event.exception;
}
template <class T, class TEvent, class TSelf,
          aux::enable_if_t<aux::is_same<TEvent, aux::remove_reference_t<T>>::value, int> = 0>
decltype(auto) get_arg(const TEvent &event, TSelf &) {
  return event;
}
template <class... Ts, class T, class TEvent, class TSelf>
auto call_impl(const aux::type<void> &, const aux::type_list<Ts...> &, T object, const TEvent &event, TSelf &self) {
  object(get_arg<Ts>(event, self)...);
  using sm = typename TSelf::type;
  log_action<typename sm::logger_t, typename sm::sm_raw_t>(typename sm::has_logger{}, self.deps_, object, event);
}
template <class... Ts, class T, class TEvent, class TSelf>
auto call_impl(const aux::type<bool> &, const aux::type_list<Ts...> &, T object, const TEvent &event, TSelf &self) {
  const auto result = object(get_arg<Ts>(event, self)...);
  using sm = typename TSelf::type;
  log_guard<typename sm::logger_t, typename sm::sm_raw_t>(typename sm::has_logger{}, self.deps_, object, event, result);
  return result;
}
template <class... Ts, class T, class TEvent, class TSelf, aux::enable_if_t<!aux::is_base_of<operator_base, T>::value, int> = 0>
auto call_impl(const aux::type_list<Ts...> &args, T object, const TEvent &event, TSelf &self) {
  using result_type = decltype(object(get_arg<Ts>(event, self)...));
  return call_impl(aux::type<result_type>{}, args, object, event, self);
}
template <class... Ts, class T, class TEvent, class TSelf, aux::enable_if_t<aux::is_base_of<operator_base, T>::value, int> = 0>
auto call_impl(const aux::type_list<Ts...> &, T object, const TEvent &event, TSelf &self) {
  return object(event, self);
}
template <class T, class TEvent, class TSelf>
auto call(T object, const TEvent &event, TSelf &self, const aux::false_type &) {
  return call_impl(args_t<T, TEvent>{}, object, event, self);
}
template <class T, class TEvent, class TSelf>
auto call(T object, const TEvent &event, TSelf &self, const aux::true_type &) {
  return object(self, event);
}
template <class T, class TEvent, class TSelf>
auto call(T object, const TEvent &event, TSelf &self) {
  return call(object, event, self, aux::is_base_of<action_base, T>{});
}
template <class... Ts>
class seq_ : operator_base {
 public:
  explicit seq_(Ts... ts) : a(ts...) {}
  template <class TEvent, class TSelf>
  void operator()(const TEvent &event, TSelf &self) {
    for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, self);
  }

 private:
  template <int... Ns, class TEvent, class TSelf>
  void for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TSelf &self) {
    int _[]{0, (call(aux::get_by_id<Ns>(a), event, self), 0)...};
    (void)_;
  }
  aux::tuple<Ts...> a;
};
template <class... Ts>
class and_ : operator_base {
 public:
  explicit and_(Ts... ts) : g(ts...) {}
  template <class TEvent, class TSelf>
  auto operator()(const TEvent &event, TSelf &self) {
    return for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, self);
  }

 private:
  template <int... Ns, class TEvent, class TSelf>
  auto for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TSelf &self) {
    auto result = true;
    int _[]{0, (call(aux::get_by_id<Ns>(g), event, self) ? result : result = false)...};
    (void)_;
    return result;
  }
  aux::tuple<Ts...> g;
};
template <class... Ts>
class or_ : operator_base {
 public:
  explicit or_(Ts... ts) : g(ts...) {}
  template <class TEvent, class TSelf>
  auto operator()(const TEvent &event, TSelf &self) {
    return for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, self);
  }

 private:
  template <int... Ns, class TEvent, class TSelf>
  auto for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TSelf &self) {
    auto result = false;
    int _[]{0, (call(aux::get_by_id<Ns>(g), event, self) ? result = true : result)...};
    (void)_;
    return result;
  }
  aux::tuple<Ts...> g;
};
template <class T>
class not_ : operator_base {
 public:
  explicit not_(T t) : g(t) {}
  template <class TEvent, class TSelf>
  auto operator()(const TEvent &event, TSelf &self) {
    return !call(g, event, self);
  }

 private:
  T g;
};
}
template <class T, BOOST_MSM_LITE_REQUIRES(concepts::callable<bool, T>::value)>
auto operator!(const T &t) {
  return detail::not_<aux::zero_wrapper<T>>(aux::zero_wrapper<T>{t});
}
template <class T1, class T2,
          BOOST_MSM_LITE_REQUIRES(concepts::callable<bool, T1>::value &&concepts::callable<bool, T2>::value)>
auto operator&&(const T1 &t1, const T2 &t2) {
  return detail::and_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}
template <class T1, class T2,
          BOOST_MSM_LITE_REQUIRES(concepts::callable<bool, T1>::value &&concepts::callable<bool, T2>::value)>
auto operator||(const T1 &t1, const T2 &t2) {
  return detail::or_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}
template <class T1, class T2,
          BOOST_MSM_LITE_REQUIRES(concepts::callable<void, T1>::value &&concepts::callable<void, T2>::value)>
auto operator,(const T1 &t1, const T2 &t2) {
  return detail::seq_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}
namespace detail {
struct initial_state {};
struct terminate_state {};
struct history_state {};
template <class>
struct state;
template <class>
class stringable {};
template <class TState>
struct stringable<state<TState>> {
  static constexpr bool value = concepts::stringable<TState>::value;
};
template <class S, bool is_stringable = stringable<S>::value>
struct state_str {
  static auto c_str() { return S::type::c_str(); }
};
template <class S>
struct state_str<S, false> {
  static auto c_str() { return __PRETTY_FUNCTION__; }
};
template <>
struct state_str<state<terminate_state>> {
  static auto c_str() { return "terminate"; }
};
template <char... Chrs>
struct state_str<state<aux::string<Chrs...>>, false> : aux::string<Chrs...> {};
template <char... Chrs, class T>
struct state_str<state<aux::string<Chrs...>(T)>, false> : state_str<state<aux::string<Chrs...>>> {};
template <class TState>
struct state_impl : state_str<TState> {
  using explicit_states = aux::type_list<>;
  template <class T>
  auto operator<=(const T &t) const {
    return transition<TState, T>{static_cast<const TState &>(*this), t};
  }
  template <class T>
  auto operator+(const T &t) const {
    return transition<TState, T>{static_cast<const TState &>(*this), t};
  }
  template <class T, BOOST_MSM_LITE_REQUIRES(concepts::callable<bool, T>::value)>
  auto operator[](const T &t) const {
    return transition_sg<TState, aux::zero_wrapper<T>>{static_cast<const TState &>(*this), aux::zero_wrapper<T>{t}};
  }
  template <class T, BOOST_MSM_LITE_REQUIRES(concepts::callable<void, T>::value)>
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
template <class TState, class... TExplicitStates>
struct state<TState(TExplicitStates...)> : state_impl<state<TState(TExplicitStates...)>> {
  using type = TState;
  using explicit_states = aux::type_list<TExplicitStates...>;
  static constexpr auto initial = false;
  static constexpr auto history = false;
};
}
namespace detail {
template <class, class>
struct ignore;
template <class E, class... Ts>
struct ignore<E, aux::type_list<Ts...>> {
  using type = aux::join_t<aux::conditional_t<aux::is_same<event_type_t<E>, aux::remove_reference_t<Ts>>::value,
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
struct always {
  bool operator()() const { return true; }
  aux::byte _[0];
};
struct none {
  void operator()() {}
  aux::byte _[0];
};
template <class E, class G>
struct transition<event<E>, G> {
  template <class T>
  auto operator/(const T &t) const {
    return transition<event<E>, G, aux::zero_wrapper<T>>{e, g, aux::zero_wrapper<T>{t}};
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
  auto operator=(const T &) const {
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
  auto operator/(const T &t) const {
    return transition<state<S2>, G, aux::zero_wrapper<T>>{g, aux::zero_wrapper<T>{t}};
  }
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, event<anonymous>, G, none>{g, none{}};
  }
};
template <class S2, class A>
struct transition_sa<state<S2>, A> : transition<state<S2>, state<S2>, event<anonymous>, always, A> {
  using transition<state<S2>, state<S2>, event<anonymous>, always, A>::a;
  transition_sa(const state<S2> &, const A &a) : transition<state<S2>, state<S2>, event<anonymous>, always, A>{always{}, a} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, event<anonymous>, always, A>{always{}, a};
  }
};
template <class S2, class E>
struct transition<state<S2>, event<E>> {
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, event<E>, always, none>{always{}, none{}};
  }
  const state<S2> &s2;
  event<E> e;
};
template <class E, class G>
struct transition_eg<event<E>, G> {
  template <class T>
  auto operator/(const T &t) const {
    return transition<event<E>, G, aux::zero_wrapper<T>>{e, g, aux::zero_wrapper<T>{t}};
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
  auto operator=(const T &) const {
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
  auto operator=(const T &) const {
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
  auto operator=(const T &) const {
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
  bool execute(SM &self, const E &event, aux::byte &current_state) {
    if (call(g, event, self)) {
      self.me_.template update_current_state<typename state<S1>::explicit_states>(
          self, current_state, aux::get_id<typename SM::type::states_ids_t, -1, dst_state>(), state<src_state>{},
          state<dst_state>{});
      call(a, event, self);
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
  bool execute(SM &self, const E &event, aux::byte &current_state) {
    self.me_.template update_current_state<typename state<S1>::explicit_states>(
        self, current_state, aux::get_id<typename SM::type::states_ids_t, -1, dst_state>(), state<src_state>{},
        state<dst_state>{});
    call(a, event, self);
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
  bool execute(SM &self, const E &event, aux::byte &current_state) {
    if (call(g, event, self)) {
      self.me_.template update_current_state<typename state<S1>::explicit_states>(
          self, current_state, aux::get_id<typename SM::type::states_ids_t, -1, dst_state>(), state<src_state>{},
          state<dst_state>{});
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
  bool execute(SM &self, const E &, aux::byte &current_state) {
    self.me_.template update_current_state<typename state<S1>::explicit_states>(
        self, current_state, aux::get_id<typename SM::type::states_ids_t, -1, dst_state>(), state<src_state>{},
        state<dst_state>{});
    return true;
  }
  aux::byte _[0];
};
}
template <class TEvent>
detail::event<TEvent> event{};
__attribute__((unused)) static const auto on_entry = event<detail::on_entry>;
__attribute__((unused)) static const auto on_exit = event<detail::on_exit>;
template <class T = detail::_>
detail::event<detail::exception<T>> exception{};
template <class T, class = void>
struct state2 {
  using type = detail::state<T>;
};
template <class T>
struct state2<T, aux::enable_if_t<concepts::configurable<T>::value>> {
  using type = detail::state<detail::sm<detail::sm_policy<T>>>;
};
template <class T>
typename state2<T>::type state{};
template <class T, T... Chrs>
auto operator""_s() {
  return detail::state<aux::string<Chrs...>>{};
}
template <class T, T... Chrs>
auto operator""_e() {
  return event<aux::string<Chrs...>>;
}
template <class T>
struct thread_safe : aux::pair<detail::thread_safety_policy__, thread_safe<T>> {
  using type = T;
};
template <template <class...> class T>
struct defer_queue : aux::pair<detail::defer_queue_policy__, defer_queue<T>> {
  template <class U>
  using rebind = T<U>;
};
template <class T>
struct logger : aux::pair<detail::logger_policy__, logger<T>> {
  using type = T;
};
__attribute__((unused)) static detail::state<detail::terminate_state> X;
__attribute__((unused)) static detail::history_state H;
__attribute__((unused)) static detail::defer defer;
__attribute__((unused)) static detail::queue queue;
template <class... Ts, BOOST_MSM_LITE_REQUIRES(aux::is_same<aux::bool_list<aux::always<Ts>::value...>,
                                                            aux::bool_list<concepts::transitional<Ts>::value...>>::value)>
auto make_transition_table(Ts... ts) {
  return aux::pool<Ts...>{ts...};
}
template <class T, class... TPolicies>
using sm = detail::sm<detail::sm_policy<T, TPolicies...>>;
BOOST_MSM_LITE_NAMESPACE_END
#endif
#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
