//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once
#if (__cplusplus < 201305L && _MSC_VER < 1900)
#error "Boost.SML requires C++14 support (Clang-3.4+, GCC-5.1+, MSVC-2015+)"
#else
#define BOOST_SML_VERSION 1'1'0
#define BOOST_SML_NAMESPACE_BEGIN \
  namespace boost {               \
  namespace sml {                 \
  inline namespace v1_1_0 {
#define BOOST_SML_NAMESPACE_END \
  }                             \
  }                             \
  }
#if defined(__clang__)
#define __BOOST_SML_UNUSED __attribute__((unused))
#define __BOOST_SML_VT_INIT \
  {}
#define __BOOST_SML_ZERO_SIZE_ARRAY(...) __VA_ARGS__ _[0]
#define __BOOST_SML_ZERO_SIZE_ARRAY_CREATE(...)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-string-literal-operator-template"
#pragma clang diagnostic ignored "-Wzero-length-array"
#elif defined(__GNUC__)
#define __has_builtin(...) 0
#define __BOOST_SML_UNUSED __attribute__((unused))
#define __BOOST_SML_VT_INIT \
  {}
#define __BOOST_SML_ZERO_SIZE_ARRAY(...) __VA_ARGS__ _[0]
#define __BOOST_SML_ZERO_SIZE_ARRAY_CREATE(...) __VA_ARGS__ ? __VA_ARGS__ : 1
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#elif defined(_MSC_VER)
#define __has_builtin(...) 0
#define __BOOST_SML_UNUSED
#define __BOOST_SML_VT_INIT
#define __BOOST_SML_ZERO_SIZE_ARRAY(...)
#define __BOOST_SML_ZERO_SIZE_ARRAY_CREATE(...) __VA_ARGS__ ? __VA_ARGS__ : 1
#pragma warning(disable : 4503)
#pragma warning(disable : 4200)
#endif
BOOST_SML_NAMESPACE_BEGIN
#define __BOOST_SML_REQUIRES(...) typename aux::enable_if<__VA_ARGS__, int>::type = 0
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
#if defined(_MSC_VER)
struct is_base_of : integral_constant<bool, __is_base_of(T, U)> {
};
#else
using is_base_of = integral_constant<bool, __is_base_of(T, U)>;
#endif
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
  explicit tuple_type(const T &object) : value(object) {}
  T value;
};
template <class, class...>
struct tuple_impl;
template <int... Ns, class... Ts>
struct tuple_impl<index_sequence<Ns...>, Ts...> : tuple_type<Ns, Ts>... {
  using boost_di_inject__ = aux::type_list<Ts...>;
  explicit tuple_impl(Ts... ts) : tuple_type<Ns, Ts>(ts)... {}
};
template <>
struct tuple_impl<index_sequence<0>> {
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
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
struct init {};
template <class T>
struct pool_type {
  explicit pool_type(const T &object) : value(object) {}
  template <class U>
  pool_type(const init &i, const U &object) : value(i, object) {}
  T value;
};
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
  explicit pool(Ts... ts) : pool_type<Ts>(ts)... {}
  template <class... TArgs>
  pool(init &&, pool<TArgs...> &&p) : pool_type<Ts>(aux::try_get<Ts>(&p))... {}
  template <class... TArgs>
  pool(const pool<TArgs...> &p) : pool_type<Ts>(init{}, &p)... {}
};
template <>
struct pool<> {
  explicit pool(...) {}
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
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
constexpr auto max() {
  auto max = 0;
  int _[]{0, (Ts > max ? max = Ts : max)...};
  (void)_;
  return max;
}
template <class... Ts>
class variant {
  using ids_t = type_id<Ts...>;
  static constexpr auto alignment = max<alignof(Ts)...>();
  static constexpr auto size = max<sizeof(Ts)...>();

 public:
  template <class T>
  variant(T object) {
    id = get_id<ids_t, -1, T>();
    new (&data) T(static_cast<T &&>(object));
  }
  alignas(alignment) byte data[size];
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
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};
template <class TExpr>
struct zero_wrapper<TExpr, void_t<decltype(+declval<TExpr>())>>
    : zero_wrapper_impl<TExpr, aux::function_traits_t<decltype(&TExpr::operator())>> {
  zero_wrapper(...) {}
};
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
struct state;
template <class>
struct event;
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
template <class T, class U>
struct get_state_mapping {
  using type = decltype(get_state_mapping_impl<T>((U *)0));
};
template <class S>
transitions_sub<S> get_sub_state_mapping_impl(...);
template <class T, class... Ts>
transitions_sub<T, Ts...> get_sub_state_mapping_impl(state_mappings<T, aux::type_list<Ts...>> *);
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
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};
}
namespace detail {
template <class>
class sm_impl;
struct _ {};
template <class TException>
struct exception : internal_event {
  using type = TException;
  explicit exception(const TException &exception = {}) : exception_(exception) {}
  TException exception_;
};
template <class TEvent = _>
struct unexpected_event : internal_event {
  using type = TEvent;
  explicit unexpected_event(const TEvent &event = {}) : event_(event) {}
  TEvent event_;
};
template <class...>
struct transitions;
template <class...>
struct transitions_sub;
template <class T, class... Ts>
struct transitions<T, Ts...> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    if (aux::get<T>(sm.transitions_).execute(event, sm, deps, subs, current_state)) {
      return true;
    }
    return transitions<Ts...>::execute(event, sm, deps, subs, current_state);
  }
};
template <class T>
struct transitions<T> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    return aux::get<T>(sm.transitions_).execute(event, sm, deps, subs, current_state);
  }
  template <class, class SM, class TDeps, class TSubs>
  static bool execute(const on_exit &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    aux::get<T>(sm.transitions_).execute(event, sm, deps, subs, current_state);
    return false;
  }
};
template <>
struct transitions<> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    sm.process_internal_event(deps, subs, unexpected_event<TEvent>{event}, current_state);
    return false;
  }
};
template <class TSM, class T, class... Ts>
struct transitions_sub<sm<TSM>, T, Ts...> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    const auto handled = aux::try_get<sm_impl<TSM>>(&subs).process_event(event, deps, subs);
    return handled ? handled : transitions<T, Ts...>::execute(event, sm, deps, subs, current_state);
  }
  template <class, class SM, class TDeps, class TSubs>
  static bool execute(const on_entry &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    transitions<T, Ts...>::execute(event, sm, deps, subs, current_state);
    aux::try_get<sm_impl<TSM>>(&subs).process_event(event, deps, subs);
    return true;
  }
};
template <class TSM>
struct transitions_sub<sm<TSM>> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent &event, SM &, TDeps &deps, TSubs &subs, typename SM::state_t &) {
    aux::try_get<sm_impl<TSM>>(&subs).process_event(event, deps, subs);
    return true;
  }
  template <class, class SM, class TDeps, class TSubs>
  static bool execute(const on_entry &event, SM &, TDeps &deps, TSubs &subs, typename SM::state_t &) {
    return aux::try_get<sm_impl<TSM>>(&subs).process_event(event, deps, subs);
  }
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
struct callable : callable_impl<R, decltype(test_callable<aux::inherit<T, callable_fallback>>(0))> {};
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
struct internal;
}
namespace concepts {
template <class...>
struct is_valid_transition : aux::true_type {};
template <class S1, class S2, class... Ts>
struct is_valid_transition<S1, S2, detail::on_entry, Ts...>
    : aux::integral_constant<bool, aux::is_same<S1, detail::internal>::value || aux::is_same<S1, S2>::value> {};
template <class S1, class S2, class... Ts>
struct is_valid_transition<S1, S2, detail::on_exit, Ts...>
    : aux::integral_constant<bool, aux::is_same<S1, detail::internal>::value || aux::is_same<S1, S2>::value> {};
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
template <class, class, class TEvent>
struct get_all_events_impl {
  using type = aux::conditional_t<aux::is_base_of<internal_event, TEvent>::value, aux::type_list<>, aux::type_list<TEvent>>;
};
template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<TSrc, TDst, unexpected_event<TEvent>> {
  using type = aux::type_list<TEvent>;
};
template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<sm<TSrc>, TDst, TEvent> {
  using type = aux::join_t<aux::type_list<TEvent>, typename sm<TSrc>::events>;
};
template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<TSrc, sm<TDst>, TEvent> {
  using type = aux::join_t<aux::type_list<TEvent>, typename sm<TDst>::events>;
};
template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<sm<TSrc>, sm<TDst>, TEvent> {
  using type = aux::join_t<aux::type_list<TEvent>, typename sm<TSrc>::events, typename sm<TDst>::events>;
};
template <class, class TEvent>
struct get_sub_internal_events_impl {
  using type = aux::conditional_t<aux::is_base_of<internal_event, TEvent>::value, aux::type_list<TEvent>, aux::type_list<>>;
};
template <class T, class TEvent>
struct get_sub_internal_events_impl<sm<T>, TEvent> {
  using type = aux::join_t<aux::type_list<TEvent>, typename sm_impl<T>::sub_internal_events>;
};
template <class... Ts>
using get_all_events =
    aux::join_t<typename get_all_events_impl<typename Ts::src_state, typename Ts::dst_state, typename Ts::event>::type...>;
template <class... Ts>
using get_sub_internal_events =
    aux::join_t<typename get_sub_internal_events_impl<typename Ts::src_state, typename Ts::event>::type...>;
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
    aux::join_t<typename aux::conditional<Ts::initial, aux::type_list<typename Ts::src_state>, aux::type_list<>>::type...>;
template <class... Ts>
using get_history_states = aux::join_t<
    typename aux::conditional<!Ts::history && Ts::initial, aux::type_list<typename Ts::src_state>, aux::type_list<>>::type...>;
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
  template <class T>
  using rebind = sm_policy<T, TPolicies...>;
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
  using thread_safety_t = typename TSM::thread_safety_policy::type;
  template <class T>
  using defer_queue_t = typename TSM::defer_queue_policy::template rebind<T>;
  using logger_t = typename TSM::logger_policy::type;
  using has_logger = aux::integral_constant<bool, !aux::is_same<logger_t, no_policy>::value>;
  using transitions_t = decltype(aux::declval<sm_t>().operator()());
  using mappings_t = detail::mappings_t<transitions_t>;
  using states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_states, transitions_t>>;
  using states_ids_t = aux::apply_t<aux::type_id, states_t>;
  using initial_states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_initial_states, transitions_t>>;
  using initial_states_ids_t = aux::apply_t<aux::type_id, initial_states_t>;
  using initial_but_not_history_states_t = aux::apply_t<get_history_states, transitions_t>;
  using has_history_states =
      aux::integral_constant<bool, aux::size<initial_states_t>::value != aux::size<initial_but_not_history_states_t>::value>;
  using events_t = aux::apply_t<aux::unique_t, aux::apply_t<get_events, transitions_t>>;
  using sub_internal_events = aux::apply_t<get_sub_internal_events, transitions_t>;
  using events_ids_t = aux::apply_t<aux::pool, aux::apply_t<aux::unique_t, aux::join_t<sub_internal_events, events_t>>>;
  using defer = aux::apply_t<aux::variant, events_t>;
  using defer_t = defer_queue_t<defer>;
  using deps = aux::apply_t<merge_deps, transitions_t>;
  using state_t = aux::conditional_t<(aux::size<states_t>::value > 0xFF), unsigned short, aux::byte>;
  static constexpr auto regions = aux::size<initial_states_t>::value > 0 ? aux::size<initial_states_t>::value : 1;
  static_assert(regions > 0, "At least one initial state is required");
  template <class... TStates>
  using get_ids = aux::index_sequence<aux::get_id<states_ids_t, -1, TStates>()...>;

 public:
  sm_impl(const aux::init &, const aux::pool_type<sm_t &> *t) : transitions_((t->value)()) {
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
  template <class TEvent, class TDeps, class TSubs>
  bool process_event(const TEvent &event, TDeps &deps, TSubs &subs) {
    log_process_event<logger_t, sm_t>(has_logger{}, deps, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    const auto handled = process_event_noexcept(event, deps, subs, has_exceptions{});
#else
    const auto handled = process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, deps, subs, states_t{},
                                                                                     aux::make_index_sequence<regions>{});
#endif
    process_internal_event(deps, subs, anonymous{});
    process_defer_events(deps, subs, handled, aux::type<defer_queue_t<TEvent>>{}, events_t{});
    return handled;
  }
  template <class TDeps, class TSubs>
  void start(TDeps &deps, TSubs &subs) {
    if (!process_internal_event(deps, subs, anonymous{})) {
      process_internal_event(deps, subs, on_entry{});
    }
  }

 private:
  void initialize(const aux::type_list<> &) {}
  template <class... TStates>
  void initialize(const aux::type_list<TStates...> &) {
    auto region = 0, i = region;
    int _[]{0, (region = i, current_state_[region] = aux::get_id<states_ids_t, 0, TStates>(), ++i, 0)...};
    (void)_;
  }
  template <class TDeps, class TSubs, class TEvent,
            __BOOST_SML_REQUIRES(!aux::is_base_of<aux::pool_type<TEvent>, events_ids_t>::value)>
  bool process_internal_event(TDeps &, TSubs &, const TEvent &, ...) {
    return false;
  }
  template <class TDeps, class TSubs, class TEvent,
            __BOOST_SML_REQUIRES(aux::is_base_of<aux::pool_type<TEvent>, events_ids_t>::value)>
  bool process_internal_event(TDeps &deps, TSubs &subs, const TEvent &event) {
    log_process_event<logger_t, sm_t>(has_logger{}, deps, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    return process_event_noexcept(event, deps, subs, has_exceptions{});
#else
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, deps, subs, states_t{},
                                                                       aux::make_index_sequence<regions>{});
#endif
  }
  template <class TDeps, class TSubs, class TEvent,
            __BOOST_SML_REQUIRES(aux::is_base_of<aux::pool_type<TEvent>, events_ids_t>::value)>
  bool process_internal_event(TDeps &deps, TSubs &subs, const TEvent &event, state_t &current_state) {
    log_process_event<logger_t, sm_t>(has_logger{}, deps, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    return process_event_noexcept(event, deps, subs, current_state, has_exceptions{});
#else
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, deps, subs, states_t{}, current_state);
#endif
  }
  template <class TMappings, class TEvent, class TDeps, class TSubs, class... TStates>
  bool process_event_impl(const TEvent &event, TDeps &deps, TSubs &subs, const aux::type_list<TStates...> &,
                          const aux::index_sequence<0> &) {
    using dispatch_table_t = bool (*)(const TEvent &, sm_impl &, TDeps &, TSubs &, state_t &);
    static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TStates))] = {
        &get_state_mapping_t<TStates, TMappings>::template execute<TEvent, sm_impl, TDeps, TSubs>...};
    const auto lock = create_lock(aux::type<thread_safety_t>{});
    (void)lock;
    return dispatch_table[current_state_[0]](event, *this, deps, subs, current_state_[0]);
  }
  template <class TMappings, class TEvent, class TDeps, class TSubs, class... TStates, int... Ns>
  bool process_event_impl(const TEvent &event, TDeps &deps, TSubs &subs, const aux::type_list<TStates...> &,
                          const aux::index_sequence<Ns...> &) {
    using dispatch_table_t = bool (*)(const TEvent &, sm_impl &, TDeps &, TSubs &, state_t &);
    static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TStates))] = {
        &get_state_mapping_t<TStates, TMappings>::template execute<TEvent, sm_impl, TDeps, TSubs>...};
    auto handled = false;
    const auto lock = create_lock(aux::type<thread_safety_t>{});
    (void)lock;
    int _[]{0, (handled |= dispatch_table[current_state_[Ns]](event, *this, deps, subs, current_state_[Ns]), 0)...};
    (void)_;
    return handled;
  }
  template <class TMappings, class TEvent, class TDeps, class TSubs, class... TStates>
  bool process_event_impl(const TEvent &event, TDeps &deps, TSubs &subs, const aux::type_list<TStates...> &,
                          state_t &current_state) {
    using dispatch_table_t = bool (*)(const TEvent &, sm_impl &, TDeps &, TSubs &, state_t &);
    static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TStates))] = {
        &get_state_mapping_t<TStates, TMappings>::template execute<TEvent, sm_impl, TDeps, TSubs>...};
    const auto lock = create_lock(aux::type<thread_safety_t>{});
    (void)lock;
    return dispatch_table[current_state](event, *this, deps, subs, current_state);
  }
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
  template <class TEvent, class TDeps, class TSubs>
  bool process_event_noexcept(const TEvent &event, TDeps &deps, TSubs &subs, const aux::false_type &) noexcept {
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, deps, subs, states_t{},
                                                                       aux::make_index_sequence<regions>{});
  }
  template <class TEvent, class TDeps, class TSubs>
  bool process_event_noexcept(const TEvent &event, TDeps &deps, TSubs &subs, state_t &current_state,
                              const aux::false_type &) noexcept {
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, deps, subs, states_t{}, current_state);
  }
  template <class TEvent, class TDeps, class TSubs>
  bool process_event_noexcept(const TEvent &event, TDeps &deps, TSubs &subs, state_t &current_state,
                              const aux::true_type &) noexcept {
    try {
      return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, deps, subs, states_t{}, current_state);
    } catch (...) {
      return process_exception(deps, subs, exceptions{});
    }
  }
  template <class TEvent, class TDeps, class TSubs>
  bool process_event_noexcept(const TEvent &event, TDeps &deps, TSubs &subs, const aux::true_type &) {
    try {
      return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, deps, subs, states_t{},
                                                                         aux::make_index_sequence<regions>{});
    } catch (...) {
      return process_exception(deps, subs, exceptions{});
    }
  }
  template <class TDeps, class TSubs>
  bool process_exception(TDeps &deps, TSubs &subs, const aux::type_list<> &) {
    return process_internal_event(deps, subs, exception<_>{});
  }
  template <class TDeps, class TSubs, class E, class... Es>
  bool process_exception(TDeps &deps, TSubs &subs, const aux::type_list<E, Es...> &) {
    try {
      throw;
    } catch (const typename E::type &e) {
      return process_internal_event(deps, subs, E{e});
    } catch (...) {
      return process_exception(deps, subs, aux::type_list<Es...>{});
    }
  }
#endif
  template <class TDeps, class TSubs, class... TEvents>
  void process_defer_events(TDeps &, TSubs &, const bool, const aux::type<detail::no_policy> &,
                            const aux::type_list<TEvents...> &) {}
  template <class TDeps, class TSubs, class TEvent>
  bool process_event_no_deffer(TDeps &deps, TSubs &subs, const void *data) {
    const auto &event = *static_cast<const TEvent *>(data);
    log_process_event<logger_t, sm_t>(has_logger{}, deps, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    const auto handled = process_event_noexcept(event, deps, subs, has_exceptions{});
#else
    const auto handled = process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, deps, subs, states_t{},
                                                                                     aux::make_index_sequence<regions>{});
#endif
    if (handled) {
      event.~TEvent();
      defer_.pop();
    }
    return handled;
  }
  template <class TDeps, class TSubs, class T, class... TEvents>
  void process_defer_events(TDeps &deps, TSubs &subs, const bool handled, const aux::type<T> &,
                            const aux::type_list<TEvents...> &) {
    if (handled) {
      auto size = defer_.size();
      static bool (sm_impl::*dispatch_event[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TEvents))])(
          TDeps &, TSubs &, const void *) = {&sm_impl::process_event_no_deffer<TDeps, TSubs, TEvents>...};
      while (size-- && (this->*dispatch_event[defer_.front().id])(deps, subs, defer_.front().data))
        ;
    }
  }
  template <class TVisitor, class... TStates>
  void visit_current_states_impl(const TVisitor &visitor, const aux::type_list<TStates...> &,
                                 const aux::index_sequence<0> &) const {
    using dispatch_table_t = void (*)(const TVisitor &);
    static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TStates))] = {
        &sm_impl::visit_state<TVisitor, TStates>...};
    dispatch_table[current_state_[0]](visitor);
  }
  template <class TVisitor, class... TStates, int... Ns>
  void visit_current_states_impl(const TVisitor &visitor, const aux::type_list<TStates...> &,
                                 const aux::index_sequence<Ns...> &) const {
    using dispatch_table_t = void (*)(const TVisitor &);
    static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TStates))] = {
        &sm_impl::visit_state<TVisitor, TStates>...};
    int _[]{0, (dispatch_table[current_state_[Ns]](visitor), 0)...};
    (void)_;
  }
  template <class TVisitor, class TState>
  static void visit_state(const TVisitor &visitor) {
    visitor(state<TState>{});
  }
  template <class, class TDeps, class TSubs, class TSrcState, class TDstState>
  void update_current_state(TDeps &, TSubs &, state_t &, const state_t &, const TSrcState &, const TDstState &,
                            const aux::true_type &) {}
  template <class TExplicit, class TDeps, class TSubs, class TSrcState, class TDstState>
  void update_current_state(TDeps &deps, TSubs &subs, state_t &current_state, const state_t &new_state,
                            const TSrcState &src_state, const TDstState &dst_state, const aux::false_type &) {
    update_current_state_impl<TExplicit>(deps, subs, current_state, new_state, src_state, dst_state);
  }
  template <class, class TDeps, class TSubs, class TSrcState, class TDstState>
  void update_current_state_impl(TDeps &deps, TSubs &subs, state_t &current_state, const state_t &new_state,
                                 const TSrcState &src_state, const TDstState &dst_state) {
    process_internal_event(deps, subs, on_exit{}, current_state);
    log_state_change<logger_t, sm_t>(has_logger{}, deps, src_state, dst_state);
    current_state = new_state;
    process_internal_event(deps, subs, on_entry{}, current_state);
  }
  template <class TExplicit, class TDeps, class TSubs, class TSrcState, class T>
  void update_current_state_impl(TDeps &deps, TSubs &subs, state_t &current_state, const state_t &new_state,
                                 const TSrcState &src_state, const state<sm<T>> &dst_state) {
    process_internal_event(deps, subs, on_exit{}, current_state);
    log_state_change<logger_t, sm_t>(has_logger{}, deps, src_state, dst_state);
    current_state = new_state;
    update_composite_states<sm_impl<T>>(subs, TExplicit{}, typename sm_impl<T>::has_history_states{},
                                        typename sm_impl<T>::initial_but_not_history_states_t{});
    process_internal_event(deps, subs, on_entry{}, current_state);
  }
  template <class T, class TSubs, class... Ts>
  void update_composite_states(TSubs &subs, const aux::type_list<Ts...> &, ...) {
    auto &sm = aux::try_get<T>(&subs);
    int _[]{0, (sm.current_state_[sm.template get_region<Ts>()] = aux::get_id<typename T::states_ids_t, -1, Ts>(), 0)...};
    (void)_;
  }
  template <class T, class TSubs, class... Ts, class... THs>
  void update_composite_states(TSubs &subs, const aux::type_list<> &, const aux::true_type &, const aux::type_list<THs...> &) {
    auto &sm = aux::try_get<T>(&subs);
    int _[]{0, (sm.current_state_[aux::get_id<typename T::initial_states_ids_t, -1, THs>()] =
                    aux::get_id<typename T::states_ids_t, -1, THs>(),
                0)...};
    (void)_;
  }
  template <class T, class TSubs>
  void update_composite_states(TSubs &subs, const aux::type_list<> &, const aux::false_type &, ...) {
    aux::try_get<T>(&subs).initialize(typename T::initial_states_t{});
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
  state_t current_state_[regions];
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
  template <class T>
  using defer_queue_t = typename TSM::defer_queue_policy::template rebind<T>;
  using logger_t = typename TSM::logger_policy::type;
  using transitions_t = decltype(aux::declval<sm_t>().operator()());
  using states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_states, transitions_t>>;
  template <class>
  struct convert;
  template <class... Ts>
  struct convert<aux::type_list<Ts...>> {
    using type = aux::type_list<sm_impl<Ts>...>;
  };
  using sub_sms = aux::apply_t<get_sub_sms, states_t>;
  using sm_all_t = aux::apply_t<aux::inherit, aux::join_t<aux::type_list<sm_t>, aux::apply_t<get_sm_t, sub_sms>>>;
  using sub_sms_t =
      aux::apply_t<aux::pool, typename convert<aux::join_t<aux::type_list<TSM>, aux::apply_t<get_sub_sms, states_t>>>::type>;
  using deps = aux::apply_t<merge_deps, transitions_t>;
  using deps_t = aux::apply_t<
      aux::pool, aux::apply_t<aux::unique_t, aux::join_t<deps, aux::type_list<logger_t>, aux::apply_t<merge_deps, sub_sms_t>>>>;
  template <class T>
  struct is_required : aux::integral_constant<bool, aux::is_base_of<aux::remove_reference_t<T>, sm_all_t>::value ||
                                                        aux::is_base_of<aux::pool_type<T>, deps_t>::value> {};
  template <class... TDeps>
  struct dependable : aux::is_same<aux::bool_list<aux::always<TDeps>::value...>, aux::bool_list<is_required<TDeps>::value...>> {
  };

 public:
  using states = typename sm_impl<TSM>::states_t;
  using events = aux::apply_t<aux::unique_t, aux::apply_t<get_all_events, transitions_t>>;
  using transitions = aux::apply_t<aux::type_list, transitions_t>;

 private:
  using events_ids_t = aux::apply_t<aux::pool, events>;

 public:
  sm(sm &&) = default;
  sm(const sm &) = delete;
  sm &operator=(const sm &) = delete;
  template <class... TDeps, __BOOST_SML_REQUIRES(dependable<TDeps...>::value)>
  explicit sm(TDeps &&... deps) : deps_{aux::init{}, aux::pool<TDeps...>{deps...}}, sub_sms_{aux::pool<TDeps...>{deps...}} {
    static_cast<aux::pool_type<sm_impl<TSM>> &>(sub_sms_).value.start(deps_, sub_sms_);
  }
  explicit sm(deps_t &deps) : deps_(deps), sub_sms_{deps} { start(aux::type<sub_sms_t>{}); }
  template <class TEvent, __BOOST_SML_REQUIRES(aux::is_base_of<aux::pool_type<TEvent>, events_ids_t>::value)>
  void process_event(const TEvent &event) {
    static_cast<aux::pool_type<sm_impl<TSM>> &>(sub_sms_).value.process_event(event, deps_, sub_sms_);
  }
  template <class TEvent, __BOOST_SML_REQUIRES(!aux::is_base_of<aux::pool_type<TEvent>, events_ids_t>::value)>
  void process_event(const TEvent &) {
    static_cast<aux::pool_type<sm_impl<TSM>> &>(sub_sms_).value.process_event(unexpected_event<>{}, deps_, sub_sms_);
  }
  template <class TEvent>
  void process_event(const event<TEvent> &) {
    process_event(TEvent{});
  }
  template <class T = sm_t, class TVisitor, __BOOST_SML_REQUIRES(concepts::callable<void, TVisitor>::value)>
  void visit_current_states(const TVisitor &visitor) const {
    using sm = sm_impl<typename TSM::template rebind<T>>;
    using states_t = typename sm::states_t;
    constexpr auto regions = sm::regions;
    static_cast<const aux::pool_type<sm> &>(sub_sms_).value.visit_current_states_impl(visitor, states_t{},
                                                                                      aux::make_index_sequence<regions>{});
  }
  template <class T = sm_t, class TState>
  bool is(const state<TState> &) const {
    auto result = false;
    visit_current_states<T>([&](auto state) { result |= aux::is_same<TState, typename decltype(state)::type>::value; });
    return result;
  }
  template <class T = sm_t, class... TStates,
            __BOOST_SML_REQUIRES(sizeof...(TStates) == sm_impl<typename TSM::template rebind<T>>::regions)>
  bool is(const state<TStates> &...) const {
    auto result = true;
    auto i = 0;
    using states_ids_t = typename sm_impl<TSM>::states_ids_t;
    int state_ids[] = {aux::get_id<states_ids_t, 0, TStates>()...};
    visit_current_states<T>(
        [&](auto state) { result &= (aux::get_id<states_ids_t, 0, typename decltype(state)::type>() == state_ids[i++]); });
    return result;
  }

 protected:
  deps_t deps_;
  sub_sms_t sub_sms_;
};
}
namespace detail {
struct operator_base {};
struct action_base {};
template <class TEvent>
struct event_type {
  using type = TEvent;
};
template <class TEvent>
struct event_type<exception<TEvent>> {
  using type = TEvent;
};
template <class TEvent>
struct event_type<unexpected_event<TEvent>> {
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
template <class T, class TEvent, class TDeps,
          aux::enable_if_t<!aux::is_same<TEvent, aux::remove_reference_t<T>>::value, int> = 0>
decltype(auto) get_arg(const TEvent &, TDeps &deps) {
  return aux::get<T>(deps);
}
template <class, class TEvent, class TDeps>
decltype(auto) get_arg(const exception<TEvent> &event, TDeps &) {
  return event.exception_;
}
template <class, class TEvent, class TDeps>
decltype(auto) get_arg(const unexpected_event<TEvent> &event, TDeps &) {
  return event.event_;
}
template <class T, class TEvent, class TDeps,
          aux::enable_if_t<aux::is_same<TEvent, aux::remove_reference_t<T>>::value, int> = 0>
decltype(auto) get_arg(const TEvent &event, TDeps &) {
  return event;
}
template <class... Ts, class T, class TEvent, class TSM, class TDeps>
auto call_impl_with_logger(const aux::type<void> &, const aux::type_list<Ts...> &, T object, const TEvent &event, TSM &,
                           TDeps &deps) {
  object(get_arg<Ts>(event, deps)...);
  log_action<typename TSM::logger_t, typename TSM::sm_t>(typename TSM::has_logger{}, deps, object, event);
}
template <class... Ts, class T, class TEvent, class TSM, class TDeps>
auto call_impl_with_logger(const aux::type<bool> &, const aux::type_list<Ts...> &, T object, const TEvent &event, TSM &,
                           TDeps &deps) {
  const auto result = object(get_arg<Ts>(event, deps)...);
  log_guard<typename TSM::logger_t, typename TSM::sm_t>(typename TSM::has_logger{}, deps, object, event, result);
  return result;
}
template <class... Ts, class T, class TEvent, class TSM, class TDeps, class TSubs>
auto call_impl(const aux::type_list<Ts...> &args, T object, const TEvent &event, TSM &sm, TDeps &deps, TSubs &,
               const aux::false_type &, const aux::true_type &) {
  using result_type = decltype(object(get_arg<Ts>(event, deps)...));
  using type = aux::type<result_type>;
  return call_impl_with_logger(type{}, args, object, event, sm, deps);
}
template <class... Ts, class T, class TEvent, class TSM, class TDeps, class TSubs>
auto call_impl(const aux::type_list<Ts...> &, T object, const TEvent &event, TSM &, TDeps &deps, TSubs &,
               const aux::false_type &, const aux::false_type &) {
  return object(get_arg<Ts>(event, deps)...);
}
template <class... Ts, class T, class TEvent, class TSM, class TDeps, class TSubs, class TLogger>
auto call_impl(const aux::type_list<Ts...> &, T object, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs,
               const aux::true_type &, const TLogger &) {
  return object(event, sm, deps, subs);
}
template <class T, class TEvent, class TSM, class TDeps, class TSubs>
auto call_check_special(T object, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs, const aux::false_type &) {
  return call_impl(args_t<T, TEvent>{}, object, event, sm, deps, subs, aux::is_base_of<operator_base, T>{},
                   typename TSM::has_logger{});
}
template <class T, class TEvent, class TSM, class TDeps, class TSubs>
auto call_check_special(T object, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs, const aux::true_type &) {
  return object(event, sm, deps, subs);
}
template <class T, class TEvent, class TSM, class TDeps, class TSubs>
auto call(T object, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
  return call_check_special(object, event, sm, deps, subs, aux::is_base_of<action_base, T>{});
}
template <class... Ts>
class seq_ : operator_base {
 public:
  explicit seq_(Ts... ts) : a(ts...) {}
  template <class TEvent, class TSM, class TDeps, class TSubs>
  void operator()(const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, sm, deps, subs);
  }

 private:
  template <int... Ns, class TEvent, class TSM, class TDeps, class TSubs>
  void for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    int _[]{0, (call(aux::get_by_id<Ns>(a), event, sm, deps, subs), 0)...};
    (void)_;
  }
  aux::tuple<Ts...> a;
};
template <class... Ts>
class and_ : operator_base {
 public:
  explicit and_(Ts... ts) : g(ts...) {}
  template <class TEvent, class TSM, class TDeps, class TSubs>
  auto operator()(const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    return for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, sm, deps, subs);
  }

 private:
  template <int... Ns, class TEvent, class TSM, class TDeps, class TSubs>
  auto for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    auto result = true;
    int _[]{0, (call(aux::get_by_id<Ns>(g), event, sm, deps, subs) ? result : result = false)...};
    (void)_;
    return result;
  }
  aux::tuple<Ts...> g;
};
template <class... Ts>
class or_ : operator_base {
 public:
  explicit or_(Ts... ts) : g(ts...) {}
  template <class TEvent, class TSM, class TDeps, class TSubs>
  auto operator()(const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    return for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, sm, deps, subs);
  }

 private:
  template <int... Ns, class TEvent, class TSM, class TDeps, class TSubs>
  auto for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    auto result = false;
    int _[]{0, (call(aux::get_by_id<Ns>(g), event, sm, deps, subs) ? result = true : result)...};
    (void)_;
    return result;
  }
  aux::tuple<Ts...> g;
};
template <class T>
class not_ : operator_base {
 public:
  explicit not_(T t) : g(t) {}
  template <class TEvent, class TSM, class TDeps, class TSubs>
  auto operator()(const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    return !call(g, event, sm, deps, subs);
  }

 private:
  T g;
};
}
template <class T, __BOOST_SML_REQUIRES(concepts::callable<bool, T>::value)>
auto operator!(const T &t) {
  return detail::not_<aux::zero_wrapper<T>>(aux::zero_wrapper<T>{t});
}
template <class T1, class T2, __BOOST_SML_REQUIRES(concepts::callable<bool, T1>::value &&concepts::callable<bool, T2>::value)>
auto operator&&(const T1 &t1, const T2 &t2) {
  return detail::and_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}
template <class T1, class T2, __BOOST_SML_REQUIRES(concepts::callable<bool, T1>::value &&concepts::callable<bool, T2>::value)>
auto operator||(const T1 &t1, const T2 &t2) {
  return detail::or_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}
template <class T1, class T2, __BOOST_SML_REQUIRES(concepts::callable<void, T1>::value &&concepts::callable<void, T2>::value)>
auto operator,(const T1 &t1, const T2 &t2) {
  return detail::seq_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}
namespace detail {
struct defer : action_base {
  template <class TEvent, class TSM, class TDeps, class TSubs>
  void operator()(const TEvent &event, TSM &sm, TDeps &, TSubs &) {
    sm.defer_.push(event);
  }
};
}
namespace detail {
struct process {
  template <class TEvent>
  class process_impl : public action_base {
   public:
    explicit process_impl(const TEvent &event) : event(event) {}
    template <class T, class TSM, class TDeps, class TSubs>
    void operator()(const T &, TSM &sm, TDeps &deps, TSubs &subs) {
      sm.process_event(event, deps, subs);
    }

   private:
    TEvent event;
  };
  template <class TEvent>
  auto operator()(const TEvent &event) {
    return process_impl<TEvent>{event};
  }
};
}
namespace detail {
template <class, class>
struct transition_eg;
template <class, class>
struct transition_ea;
template <class>
struct event {
  template <class T, __BOOST_SML_REQUIRES(concepts::callable<bool, T>::value)>
  auto operator[](const T &t) const {
    return transition_eg<event, aux::zero_wrapper<T>>{*this, aux::zero_wrapper<T>{t}};
  }
  template <class T, __BOOST_SML_REQUIRES(concepts::callable<void, T>::value)>
  auto operator/(const T &t) const {
    return transition_ea<event, aux::zero_wrapper<T>>{*this, aux::zero_wrapper<T>{t}};
  }
};
}
namespace detail {
struct internal {};
struct initial_state {};
struct terminate_state {};
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
  template <class T>
  auto operator=(const T &t) const {
    return transition<T, state>{t, *this};
  }
};
}
namespace detail {
template <class, class>
struct ignore;
template <class E, class... Ts>
struct ignore<E, aux::type_list<Ts...>> {
  template <class T>
  struct non_events {
    using type = aux::conditional_t<aux::is_same<event_type_t<E>, aux::remove_reference_t<T>>::value, aux::type_list<>,
                                    aux::type_list<T>>;
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
  bool operator()() const { return true; }
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};
struct none {
  void operator()() {}
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};
template <class...>
struct transition;
template <class E, class G>
struct transition<detail::event<E>, G> {
  template <class T>
  auto operator/(const T &t) const {
    return transition<detail::event<E>, G, aux::zero_wrapper<T>>{e, g, aux::zero_wrapper<T>{t}};
  }
  detail::event<E> e;
  G g;
};
template <class E, class G, class A>
struct transition<detail::event<E>, G, A> {
  detail::event<E> e;
  G g;
  A a;
};
template <class S2, class G, class A>
struct transition<detail::state<S2>, G, A>
    : transition<detail::state<internal>, detail::state<S2>, detail::event<anonymous>, G, A> {
  using transition<detail::state<internal>, detail::state<S2>, detail::event<anonymous>, G, A>::g;
  using transition<detail::state<internal>, detail::state<S2>, detail::event<anonymous>, G, A>::a;
  transition(const G &g, const A &a)
      : transition<detail::state<internal>, detail::state<S2>, detail::event<anonymous>, G, A>{g, a} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, detail::state<S2>, detail::event<anonymous>, G, A>{g, a};
  }
};
template <class S1, class S2>
struct transition<detail::state<S1>, detail::state<S2>>
    : transition<detail::state<S1>, detail::state<S2>, detail::event<anonymous>, always, none> {
  transition(const detail::state<S1> &, const detail::state<S2> &)
      : transition<detail::state<S1>, detail::state<S2>, detail::event<anonymous>, always, none>{always{}, none{}} {}
};
template <class S2, class G>
struct transition_sg<detail::state<S2>, G>
    : transition<detail::state<internal>, detail::state<S2>, detail::event<anonymous>, G, none> {
  using transition<detail::state<internal>, detail::state<S2>, detail::event<anonymous>, G, none>::g;
  transition_sg(const detail::state<S2> &, const G &g)
      : transition<detail::state<internal>, detail::state<S2>, detail::event<anonymous>, G, none>{g, none{}} {}
  template <class T>
  auto operator/(const T &t) const {
    return transition<detail::state<S2>, G, aux::zero_wrapper<T>>{g, aux::zero_wrapper<T>{t}};
  }
  template <class T>
  auto operator=(const T &) const {
    return transition<T, detail::state<S2>, detail::event<anonymous>, G, none>{g, none{}};
  }
};
template <class S2, class A>
struct transition_sa<detail::state<S2>, A>
    : transition<detail::state<internal>, detail::state<S2>, detail::event<anonymous>, always, A> {
  using transition<detail::state<internal>, detail::state<S2>, detail::event<anonymous>, always, A>::a;
  transition_sa(const detail::state<S2> &, const A &a)
      : transition<detail::state<internal>, detail::state<S2>, detail::event<anonymous>, always, A>{always{}, a} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, detail::state<S2>, detail::event<anonymous>, always, A>{always{}, a};
  }
};
template <class S2, class E>
struct transition<detail::state<S2>, detail::event<E>> {
  template <class T>
  auto operator=(const T &) const {
    return transition<T, detail::state<S2>, detail::event<E>, always, none>{always{}, none{}};
  }
  const detail::state<S2> &s2;
  detail::event<E> e;
};
template <class E, class G>
struct transition_eg<detail::event<E>, G> {
  template <class T>
  auto operator/(const T &t) const {
    return transition<detail::event<E>, G, aux::zero_wrapper<T>>{e, g, aux::zero_wrapper<T>{t}};
  }
  detail::event<E> e;
  G g;
};
template <class E, class A>
struct transition_ea<detail::event<E>, A> {
  detail::event<E> e;
  A a;
};
template <class S1, class S2, class G, class A>
struct transition<detail::state<S1>, transition<detail::state<S2>, G, A>>
    : transition<detail::state<S1>, detail::state<S2>, detail::event<anonymous>, G, A> {
  transition(const detail::state<S1> &, const transition<detail::state<S2>, G, A> &t)
      : transition<detail::state<S1>, detail::state<S2>, detail::event<anonymous>, G, A>{t.g, t.a} {}
};
template <class S1, class E, class G, class A>
struct transition<detail::state<S1>, transition<detail::event<E>, G, A>>
    : transition<detail::state<internal>, detail::state<S1>, detail::event<E>, G, A> {
  using transition<detail::state<internal>, detail::state<S1>, detail::event<E>, G, A>::g;
  using transition<detail::state<internal>, detail::state<S1>, detail::event<E>, G, A>::a;
  transition(const detail::state<S1> &, const transition<detail::event<E>, G, A> &t)
      : transition<detail::state<internal>, detail::state<S1>, detail::event<E>, G, A>{t.g, t.a} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, detail::state<S1>, detail::event<E>, G, A>{g, a};
  }
};
template <class S1, class S2, class E>
struct transition<detail::state<S1>, transition<detail::state<S2>, detail::event<E>>>
    : transition<detail::state<S1>, detail::state<S2>, detail::event<E>, always, none> {
  transition(const detail::state<S1> &, const transition<detail::state<S2>, detail::event<E>> &)
      : transition<detail::state<S1>, detail::state<S2>, detail::event<E>, always, none>{always{}, none{}} {}
};
template <class S1, class S2, class G>
struct transition<detail::state<S1>, transition_sg<detail::state<S2>, G>>
    : transition<detail::state<S1>, detail::state<S2>, detail::event<anonymous>, G, none> {
  transition(const detail::state<S1> &, const transition_sg<detail::state<S2>, G> &t)
      : transition<detail::state<S1>, detail::state<S2>, detail::event<anonymous>, G, none>{t.g, none{}} {}
};
template <class S1, class S2, class A>
struct transition<detail::state<S1>, transition_sa<detail::state<S2>, A>>
    : transition<detail::state<S1>, detail::state<S2>, detail::event<anonymous>, always, A> {
  transition(const detail::state<S1> &, const transition_sa<detail::state<S2>, A> &t)
      : transition<detail::state<S1>, detail::state<S2>, detail::event<anonymous>, always, A>{always{}, t.a} {}
};
template <class S2, class E, class G>
struct transition<detail::state<S2>, transition_eg<detail::event<E>, G>>
    : transition<detail::state<internal>, detail::state<S2>, detail::event<E>, G, none> {
  using transition<detail::state<internal>, detail::state<S2>, detail::event<E>, G, none>::g;
  transition(const detail::state<S2> &, const transition_eg<detail::event<E>, G> &t)
      : transition<detail::state<internal>, detail::state<S2>, detail::event<E>, G, none>{t.g, none{}} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, detail::state<S2>, detail::event<E>, G, none>{g, none{}};
  }
};
template <class S1, class S2, class E, class G>
struct transition<detail::state<S1>, transition<detail::state<S2>, transition_eg<detail::event<E>, G>>>
    : transition<detail::state<S1>, detail::state<S2>, detail::event<E>, G, none> {
  transition(const detail::state<S1> &, const transition<detail::state<S2>, transition_eg<detail::event<E>, G>> &t)
      : transition<detail::state<S1>, detail::state<S2>, detail::event<E>, G, none>{t.g, none{}} {}
};
template <class S2, class E, class A>
struct transition<detail::state<S2>, transition_ea<detail::event<E>, A>>
    : transition<detail::state<internal>, detail::state<S2>, detail::event<E>, always, A> {
  using transition<detail::state<internal>, detail::state<S2>, detail::event<E>, always, A>::a;
  transition(const detail::state<S2> &, const transition_ea<detail::event<E>, A> &t)
      : transition<detail::state<internal>, detail::state<S2>, detail::event<E>, always, A>{always{}, t.a} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, detail::state<S2>, detail::event<E>, always, A>{always{}, a};
  }
};
template <class S1, class S2, class E, class A>
struct transition<detail::state<S1>, transition<detail::state<S2>, transition_ea<detail::event<E>, A>>>
    : transition<detail::state<S1>, detail::state<S2>, detail::event<E>, always, A> {
  transition(const detail::state<S1> &, const transition<detail::state<S2>, transition_ea<detail::event<E>, A>> &t)
      : transition<detail::state<S1>, detail::state<S2>, detail::event<E>, always, A>{always{}, t.a} {}
};
template <class S1, class S2, class E, class G, class A>
struct transition<detail::state<S1>, transition<detail::state<S2>, transition<detail::event<E>, G, A>>>
    : transition<detail::state<S1>, detail::state<S2>, detail::event<E>, G, A> {
  transition(const detail::state<S1> &, const transition<detail::state<S2>, transition<detail::event<E>, G, A>> &t)
      : transition<detail::state<S1>, detail::state<S2>, detail::event<E>, G, A>{t.g, t.a} {}
};
template <class S1, class S2, class E, class G, class A>
struct transition<detail::state<S1>, detail::state<S2>, detail::event<E>, G, A> {
  static constexpr auto initial = detail::state<S2>::initial;
  static constexpr auto history = detail::state<S2>::history;
  using is_internal = aux::is_same<S1, internal>;
  using dst = aux::conditional_t<is_internal::value, S2, S1>;
  using src_state = typename detail::state<S2>::type;
  using dst_state = typename detail::state<dst>::type;
  using event = E;
  using guard = G;
  using action = A;
  using deps = aux::apply_t<aux::unique_t, aux::join_t<get_deps_t<G, E>, get_deps_t<A, E>>>;
  transition(const G &g, const A &a) : g(g), a(a) {}
  template <class SM, class TDeps, class TSubs>
  bool execute(const E &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    if (call(g, event, sm, deps, subs)) {
      sm.template update_current_state<typename detail::state<S1>::explicit_states>(
          deps, subs, current_state, aux::get_id<typename SM::states_ids_t, -1, dst_state>(), detail::state<src_state>{},
          detail::state<dst_state>{}, is_internal{});
      call(a, event, sm, deps, subs);
      return true;
    }
    return false;
  }
  G g;
  A a;
};
template <class S1, class S2, class E, class A>
struct transition<detail::state<S1>, detail::state<S2>, detail::event<E>, always, A> {
  static constexpr auto initial = detail::state<S2>::initial;
  static constexpr auto history = detail::state<S2>::history;
  using is_internal = aux::is_same<S1, internal>;
  using dst = aux::conditional_t<is_internal::value, S2, S1>;
  using src_state = typename detail::state<S2>::type;
  using dst_state = typename detail::state<dst>::type;
  using event = E;
  using guard = always;
  using action = A;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<A, E>>;
  transition(const always &, const A &a) : a(a) {}
  template <class SM, class TDeps, class TSubs>
  bool execute(const E &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    sm.template update_current_state<typename detail::state<S1>::explicit_states>(
        deps, subs, current_state, aux::get_id<typename SM::states_ids_t, -1, dst_state>(), detail::state<src_state>{},
        detail::state<dst_state>{}, is_internal{});
    call(a, event, sm, deps, subs);
    return true;
  }
  A a;
};
template <class S1, class S2, class E, class G>
struct transition<detail::state<S1>, detail::state<S2>, detail::event<E>, G, none> {
  static constexpr auto initial = detail::state<S2>::initial;
  static constexpr auto history = detail::state<S2>::history;
  using is_internal = aux::is_same<S1, internal>;
  using dst = aux::conditional_t<is_internal::value, S2, S1>;
  using src_state = typename detail::state<S2>::type;
  using dst_state = typename detail::state<dst>::type;
  using event = E;
  using guard = G;
  using action = none;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<G, E>>;
  transition(const G &g, const none &) : g(g) {}
  template <class SM, class TDeps, class TSubs>
  bool execute(const E &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    if (call(g, event, sm, deps, subs)) {
      sm.template update_current_state<typename detail::state<S1>::explicit_states>(
          deps, subs, current_state, aux::get_id<typename SM::states_ids_t, -1, dst_state>(), detail::state<src_state>{},
          detail::state<dst_state>{}, is_internal{});
      return true;
    }
    return false;
  }
  G g;
};
template <class S1, class S2, class E>
struct transition<detail::state<S1>, detail::state<S2>, detail::event<E>, always, none> {
  static constexpr auto initial = detail::state<S2>::initial;
  static constexpr auto history = detail::state<S2>::history;
  using is_internal = aux::is_same<S1, internal>;
  using dst = aux::conditional_t<is_internal::value, S2, S1>;
  using src_state = typename detail::state<S2>::type;
  using dst_state = typename detail::state<dst>::type;
  using event = E;
  using guard = always;
  using action = none;
  using deps = aux::type_list<>;
  transition(const always &, const none &) {}
  template <class SM, class TDeps, class TSubs>
  bool execute(const E &, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    sm.template update_current_state<typename detail::state<S1>::explicit_states>(
        deps, subs, current_state, aux::get_id<typename SM::states_ids_t, -1, dst_state>(), detail::state<src_state>{},
        detail::state<dst_state>{}, is_internal{});
    return true;
  }
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};
}
using _ = detail::_;
template <class TEvent>
detail::event<TEvent> event __BOOST_SML_VT_INIT;
__BOOST_SML_UNUSED static detail::event<detail::on_entry> on_entry;
__BOOST_SML_UNUSED static detail::event<detail::on_exit> on_exit;
template <class T = _>
detail::event<detail::exception<T>> exception __BOOST_SML_VT_INIT;
template <class T = _>
detail::event<detail::unexpected_event<T>> unexpected_event __BOOST_SML_VT_INIT;
template <class T, class = void>
struct state_impl {
  using type = detail::state<T>;
};
template <class T>
struct state_impl<T, aux::enable_if_t<concepts::configurable<T>::value>> {
  using type = detail::state<detail::sm<detail::sm_policy<T>>>;
};
template <class T>
typename state_impl<T>::type state __BOOST_SML_VT_INIT;
#if !defined(_MSC_VER)
template <class T, T... Chrs>
auto operator""_s() {
  return detail::state<aux::string<Chrs...>>{};
}
template <class T, T... Chrs>
auto operator""_e() {
  return event<aux::string<Chrs...>>;
}
#endif
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
__BOOST_SML_UNUSED static detail::state<detail::terminate_state> X;
__BOOST_SML_UNUSED static detail::history_state H;
__BOOST_SML_UNUSED static detail::defer defer;
__BOOST_SML_UNUSED static detail::process process;
template <class... Ts, __BOOST_SML_REQUIRES(aux::is_same<aux::bool_list<aux::always<Ts>::value...>,
                                                         aux::bool_list<concepts::transitional<Ts>::value...>>::value)>
auto make_transition_table(Ts... ts) {
  return aux::pool<Ts...>{ts...};
}
template <class T, class... TPolicies>
using sm = detail::sm<detail::sm_policy<T, TPolicies...>>;
template <class TExpr, class... TArgs>
auto make_sm(TExpr expr, TArgs &&... args) {
  return sml::sm<TExpr>{expr, args...};
};
BOOST_SML_NAMESPACE_END
#undef __BOOST_SML_UNUSED
#undef __BOOST_SML_VT_INIT
#undef __BOOST_SML_ZERO_SIZE_ARRAY
#undef __BOOST_SML_ZERO_SIZE_ARRAY_CREATE
#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#undef __has_builtin
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#undef __has_builtin
#endif
#endif
