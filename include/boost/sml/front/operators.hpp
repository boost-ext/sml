//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_FRONT_OPERATORS_HPP
#define BOOST_SML_FRONT_OPERATORS_HPP

#include "boost/sml/back/internals.hpp"

namespace detail {

struct operator_base {};
struct action_base {};

#if defined(_MSC_VER)  // __pph__
template <class, class>
aux::type_list<> args1__(...);
template <class T, class E>
auto args1__(int) -> aux::function_traits_t<decltype(&T::operator())>;
template <class T, class E>
auto args__(...) -> decltype(args1__<T, E>(0));
template <class T, class E>
auto args__(int) -> aux::function_traits_t<decltype(&T::operator() < get_event_t<E>>)>;
template <class T, class E>
using args_t = decltype(args__<T, E>(0));
#else   // __pph__
template <class, class>
aux::type_list<> args__(...);
template <class T, class E>
auto args__(int) -> aux::function_traits_t<decltype(&T::template operator() < get_event_t<E>>)>;
template <class T, class>
auto args__(int) -> aux::function_traits_t<decltype(&T::operator())>;
template <class T, class E>
using args_t = decltype(args__<T, E>(0));
#endif  // __pph__

template <class T, class TEvent, class TDeps>
decltype(auto) get_arg(const aux::type<T> &, const TEvent &, TDeps &deps) {
  return aux::get<T>(deps);
}
template <class TEvent, class TDeps>
decltype(auto) get_arg(const aux::type<TEvent> &, const TEvent &event, TDeps &) {
  return event;
}
template <class TEvent, class TDeps>  // "event"_e
decltype(auto) get_arg(const aux::type<const TEvent &> &, const TEvent &event, TDeps &) {
  return event;
}
template <class T, class TEvent, class TDeps>
decltype(auto) get_arg(const aux::type<const TEvent &> &, const unexpected_event<T, TEvent> &event, TDeps &) {
  return event.event_;
}
template <class T, class TEvent, class TDeps>
decltype(auto) get_arg(const aux::type<const TEvent &> &, const on_entry<T, TEvent> &event, TDeps &) {
  return event.event_;
}
template <class T, class TEvent, class TDeps>
decltype(auto) get_arg(const aux::type<const TEvent &> &, const on_exit<T, TEvent> &event, TDeps &) {
  return event.event_;
}
template <class T, class TEvent, class TDeps>
decltype(auto) get_arg(const aux::type<T> &, const exception<TEvent> &event, TDeps &) {
  return event.exception_;
}

template <class... Ts, class T, class TEvent, class TSM, class TDeps>
auto call_impl_with_logger(const aux::type<void> &, const aux::type_list<Ts...> &, T object, const TEvent &event, TSM &,
                           TDeps &deps) {
  object(get_arg(aux::type<Ts>{}, event, deps)...);
  log_action<typename TSM::logger_t, typename TSM::sm_t>(typename TSM::has_logger{}, deps, object, event);
}

template <class... Ts, class T, class TEvent, class TSM, class TDeps>
auto call_impl_with_logger(const aux::type<bool> &, const aux::type_list<Ts...> &, T object, const TEvent &event, TSM &,
                           TDeps &deps) {
  const auto result = object(get_arg(aux::type<Ts>{}, event, deps)...);
  log_guard<typename TSM::logger_t, typename TSM::sm_t>(typename TSM::has_logger{}, deps, object, event, result);
  return result;
}

template <class... Ts, class T, class TEvent, class TSM, class TDeps, class TSubs>
auto call_impl(const aux::type_list<Ts...> &args, T object, const TEvent &event, TSM &sm, TDeps &deps, TSubs &,
               const aux::false_type &, const aux::true_type &) {
  using result_type = decltype(object(get_arg(aux::type<Ts>{}, event, deps)...));
  using type = aux::type<result_type>;
  return call_impl_with_logger(type{}, args, object, event, sm, deps);
}

template <class... Ts, class T, class TEvent, class TSM, class TDeps, class TSubs>
auto call_impl(const aux::type_list<Ts...> &, T object, const TEvent &event, TSM &, TDeps &deps, TSubs &,
               const aux::false_type &, const aux::false_type &) {
  return object(get_arg(aux::type<Ts>{}, event, deps)...);
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
    int _[]{0, (call(aux::get_by_id<Ns>(&a), event, sm, deps, subs), 0)...};
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
    int _[]{0, (call(aux::get_by_id<Ns>(&g), event, sm, deps, subs) ? result : result = false)...};
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
    int _[]{0, (call(aux::get_by_id<Ns>(&g), event, sm, deps, subs) ? result = true : result)...};
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
