//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_FRONT_OPERATORS_HPP
#define BOOST_SML_FRONT_OPERATORS_HPP

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
decltype(auto) get_arg(const TEvent &, TDeps& deps) {
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
auto call_impl_with_logger(const aux::type<void> &, const aux::type_list<Ts...> &, T object, const TEvent &event, TSM&, TDeps& deps) {
  object(get_arg<Ts>(event, deps)...);
  log_action<typename TSM::logger_t, typename TSM::sm_t>(typename TSM::has_logger{}, deps, object, event);
}

template <class... Ts, class T, class TEvent, class TSM, class TDeps>
auto call_impl_with_logger(const aux::type<bool> &, const aux::type_list<Ts...> &, T object, const TEvent &event, TSM&, TDeps& deps) {
  const auto result = object(get_arg<Ts>(event, deps)...);
  log_guard<typename TSM::logger_t, typename TSM::sm_t>(typename TSM::has_logger{}, deps, object, event, result);
  return result;
}

template <class... Ts, class T, class TEvent, class TSM, class TDeps>
auto call_impl(const aux::type_list<Ts...> &args, T object, const TEvent &event, TSM& sm, TDeps& deps, const aux::false_type &,
               const aux::true_type &) {
  using result_type = decltype(object(get_arg<Ts>(event, deps)...));
  using type = aux::type<result_type>;
  return call_impl_with_logger(type{}, args, object, event, sm, deps);
}

template <class... Ts, class T, class TEvent, class TSM, class TDeps>
auto call_impl(const aux::type_list<Ts...> &, T object, const TEvent &event, TSM&, TDeps& deps, const aux::false_type &,
               const aux::false_type &) {
  return object(get_arg<Ts>(event, deps)...);
}

template <class... Ts, class T, class TEvent, class TSM, class TDeps, class TLogger>
auto call_impl(const aux::type_list<Ts...> &, T object, const TEvent &event, TSM& sm, TDeps& deps, const aux::true_type &,
               const TLogger &) {
  return object(event, sm, deps);
}

template <class T, class TEvent, class TSM, class TDeps>
auto call_check_special(T object, const TEvent &event, TSM& sm, TDeps& deps, const aux::false_type &) {
  return call_impl(args_t<T, TEvent>{}, object, event, sm, deps, aux::is_base_of<operator_base, T>{}, typename TSM::has_logger{});
}

template <class T, class TEvent, class TSM, class TDeps>
auto call_check_special(T object, const TEvent &event, TSM& sm, TDeps& deps, const aux::true_type &) {
  return object(event, sm, deps);
}

template <class T, class TEvent, class TSM, class TDeps>
auto call(T object, const TEvent &event, TSM& sm, TDeps& deps) {
  return call_check_special(object, event, sm, deps, aux::is_base_of<action_base, T>{});
}

template <class... Ts>
class seq_ : operator_base {
 public:
  explicit seq_(Ts... ts) : a(ts...) {}

  template <class TEvent, class TSM, class TDeps>
  void operator()(const TEvent &event, TSM& sm, TDeps& deps) {
    for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, sm, deps);
  }

 private:
  template <int... Ns, class TEvent, class TSM, class TDeps>
  void for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TSM& sm, TDeps& deps) {
    int _[]{0, (call(aux::get_by_id<Ns>(a), event, sm, deps), 0)...};
    (void)_;
  }

  aux::tuple<Ts...> a;
};
template <class... Ts>
class and_ : operator_base {
 public:
  explicit and_(Ts... ts) : g(ts...) {}

  template <class TEvent, class TSM, class TDeps>
  auto operator()(const TEvent &event, TSM& sm, TDeps& deps) {
    return for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, sm, deps);
  }

 private:
  template <int... Ns, class TEvent, class TSM, class TDeps>
  auto for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TSM& sm, TDeps& deps) {
    auto result = true;
    int _[]{0, (call(aux::get_by_id<Ns>(g), event, sm, deps) ? result : result = false)...};
    (void)_;
    return result;
  }

  aux::tuple<Ts...> g;
};
template <class... Ts>
class or_ : operator_base {
 public:
  explicit or_(Ts... ts) : g(ts...) {}

  template <class TEvent, class TSM, class TDeps>
  auto operator()(const TEvent &event, TSM& sm, TDeps& deps) {
    return for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, sm, deps);
  }

 private:
  template <int... Ns, class TEvent, class TSM, class TDeps>
  auto for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TSM& sm, TDeps& deps) {
    auto result = false;
    int _[]{0, (call(aux::get_by_id<Ns>(g), event, sm, deps) ? result = true : result)...};
    (void)_;
    return result;
  }

  aux::tuple<Ts...> g;
};
template <class T>
class not_ : operator_base {
 public:
  explicit not_(T t) : g(t) {}

  template <class TEvent, class TSM, class TDeps>
  auto operator()(const TEvent &event, TSM& sm, TDeps& deps) {
    return !call(g, event, sm, deps);
  }

 private:
  T g;
};

}  // detail

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

#endif
