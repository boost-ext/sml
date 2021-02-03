//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_FRONT_OPERATORS_HPP
#define BOOST_SML_FRONT_OPERATORS_HPP

#include "boost/sml/aux_/utility.hpp"
#include "boost/sml/back/internals.hpp"
#include "boost/sml/concepts/callable.hpp"

namespace front {

struct operator_base {};
struct action_base {};

template <class TRootSM, class... TSubSMs>
TRootSM get_root_sm_impl(aux::pool<TRootSM, TSubSMs...> *);

template <class TSubs>
using get_root_sm_t = decltype(get_root_sm_impl((TSubs *)0));

template <class, class>
aux::type_list<action_base> args1__(...);
template <class T, class E>
auto args1__(int) -> aux::function_traits_t<decltype(&T::operator())>;
template <class T, class E>
auto args__(int) -> aux::function_traits_t<decltype(&T::__BOOST_SML_TEMPLATE_KEYWORD operator()<back::get_event_t<E>>)>;
template <class T, class E>
auto args__(...) -> decltype(args1__<T, E>(0));
template <class T, class E>
using args_t = decltype(args__<T, E>(0));

template <class T, class TEvent, class TSM, class TDeps>
decltype(auto) get_arg(const aux::type<T> &, const TEvent &, TSM &, TDeps &deps) {
  return aux::get<T>(deps);
}
template <class TEvent, class TSM, class TDeps>
decltype(auto) get_arg(const aux::type<TEvent> &, const TEvent &event, TSM &, TDeps &) {
  return event;
}
template <class TEvent, class TSM, class TDeps>  // "event"_e
decltype(auto) get_arg(const aux::type<const TEvent &> &, const TEvent &event, TSM &, TDeps &) {
  return event;
}
template <class T, class TEvent, class TSM, class TDeps>
decltype(auto) get_arg(const aux::type<const TEvent &> &, const back::unexpected_event<T, TEvent> &event, TSM &, TDeps &) {
  return event.event_;
}
template <class T, class TEvent, class TSM, class TDeps>
decltype(auto) get_arg(const aux::type<const TEvent &> &, const back::on_entry<T, TEvent> &event, TSM &, TDeps &) {
  return event.event_;
}
template <class T, class TEvent, class TSM, class TDeps>
decltype(auto) get_arg(const aux::type<const TEvent &> &, const back::on_exit<T, TEvent> &event, TSM &, TDeps &) {
  return event.event_;
}
template <class T, class TEvent, class TSM, class TDeps>
decltype(auto) get_arg(const aux::type<const TEvent &> &, const back::exception<T, TEvent> &event, TSM &, TDeps &) {
  return event.exception_;
}
template <class... TEvents, class TEvent, class TSM, class TDeps>
decltype(auto) get_arg(const aux::type<back::defer<TEvents...>> &, const TEvent, TSM &sm, TDeps &) {
  return back::defer<TEvents...>{sm.defer_};
}
template <class... TEvents, class TEvent, class TSM, class TDeps>
decltype(auto) get_arg(const aux::type<back::process<TEvents...>> &, const TEvent, TSM &sm, TDeps &) {
  return back::process<TEvents...>{sm.process_};
}

template <class, class, class>
struct call;

template <class TEvent>
struct call<TEvent, aux::type_list<>, back::no_policy> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &, TSM &, TDeps &, TSubs &) {
    return object();
  }
};

template <class TEvent, class TLogger>
struct call<TEvent, aux::type_list<>, TLogger> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &, TDeps &deps, TSubs &) {
    using result_type = decltype(object());
    return execute_impl<typename TSM::sm_t>(aux::type<result_type>{}, object, event, deps);
  }

  template <class TSM, class T, class TDeps>
  static auto execute_impl(const aux::type<bool> &, T object, const TEvent &event, TDeps &deps) {
    const auto result = object();
    back::policies::log_guard<TSM>(aux::type<TLogger>{}, deps, object, event, result);
    return result;
  }

  template <class TSM, class T, class TDeps>
  static auto execute_impl(const aux::type<void> &, T object, const TEvent &event, TDeps &deps) {
    back::policies::log_action<TSM>(aux::type<TLogger>{}, deps, object, event);
    object();
  }
};

template <class TEvent>
struct call<TEvent, aux::type_list<TEvent>, back::no_policy> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &, TDeps &, TSubs &) {
    return object(event);
  }
};

template <class TEvent, class TLogger>
struct call<TEvent, aux::type_list<TEvent>, TLogger> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &, TDeps &deps, TSubs &) {
    using result_type = decltype(object(event));
    return execute_impl<typename TSM::sm_t>(aux::type<result_type>{}, object, event, deps);
  }

  template <class TSM, class T, class TDeps>
  static auto execute_impl(const aux::type<bool> &, T object, const TEvent &event, TDeps &deps) {
    const auto result = object(event);
    back::policies::log_guard<TSM>(aux::type<TLogger>{}, deps, object, event, result);
    return result;
  }

  template <class TSM, class T, class TDeps>
  static auto execute_impl(const aux::type<void> &, T object, const TEvent &event, TDeps &deps) {
    back::policies::log_action<TSM>(aux::type<TLogger>{}, deps, object, event);
    object(event);
  }
};

template <class TEvent>
struct call<TEvent, aux::type_list<action_base>, back::no_policy> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    return object(event, sm, deps, subs);
  }
};

template <class TEvent, class TLogger>
struct call<TEvent, aux::type_list<action_base>, TLogger> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    return object(event, sm, deps, subs);
  }
};

template <class TEvent, class... Ts>
struct call<TEvent, aux::type_list<Ts...>, back::no_policy> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &sm, TDeps &deps, TSubs &) {
    return object(get_arg(aux::type<Ts>{}, event, sm, deps)...);
  }
};

template <class TEvent, class... Ts, class TLogger>
struct call<TEvent, aux::type_list<Ts...>, TLogger> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &sm, TDeps &deps, TSubs &) {
    using result_type = decltype(object(get_arg(aux::type<Ts>{}, event, sm, deps)...));
    return execute_impl<typename TSM::sm_t>(aux::type<result_type>{}, object, event, sm, deps);
  }
  template <class TSM, class T, class SM, class TDeps>
  static auto execute_impl(const aux::type<bool> &, T object, const TEvent &event, SM &sm, TDeps &deps) {
    const auto result = object(get_arg(aux::type<Ts>{}, event, sm, deps)...);
    back::policies::log_guard<TSM>(aux::type<TLogger>{}, deps, object, event, result);
    return result;
  }
  template <class TSM, class T, class SM, class TDeps>
  static auto execute_impl(const aux::type<void> &, T object, const TEvent &event, SM &sm, TDeps &deps) {
    back::policies::log_action<TSM>(aux::type<TLogger>{}, deps, object, event);
    object(get_arg(aux::type<Ts>{}, event, sm, deps)...);
  }
};

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
#if defined(__cpp_fold_expressions)  // __pph__
    (call<TEvent, args_t<Ts, TEvent>, typename TSM::logger_t>::execute(aux::get_by_id<Ns>(&a), event, sm, deps, subs), ...);
#else   // __pph__
    (void)aux::swallow{
        0, (call<TEvent, args_t<Ts, TEvent>, typename TSM::logger_t>::execute(aux::get_by_id<Ns>(&a), event, sm, deps, subs),
            0)...};
#endif  // __pph__
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
#if defined(__cpp_fold_expressions)  // __pph__
    return (call<TEvent, args_t<Ts, TEvent>, typename TSM::logger_t>::execute(aux::get_by_id<Ns>(&g), event, sm, deps, subs) &&
            ...);
#else   // __pph__
    auto result = true;
    (void)aux::swallow{0, (result = result && call<TEvent, args_t<Ts, TEvent>, typename TSM::logger_t>::execute(
                                                  aux::get_by_id<Ns>(&g), event, sm, deps, subs),
                           0)...};
    return result;
#endif  // __pph__
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
#if defined(__cpp_fold_expressions)  // __pph__
    return (call<TEvent, args_t<Ts, TEvent>, typename TSM::logger_t>::execute(aux::get_by_id<Ns>(&g), event, sm, deps, subs) ||
            ...);
#else   // __pph__
    auto result = false;
    (void)aux::swallow{0, (result = result || call<TEvent, args_t<Ts, TEvent>, typename TSM::logger_t>::execute(
                                                  aux::get_by_id<Ns>(&g), event, sm, deps, subs),
                           0)...};
    return result;
#endif  // __pph__
  }

  aux::tuple<Ts...> g;
};

template <class T>
class not_ : operator_base {
 public:
  explicit not_(T t) : g(t) {}

  template <class TEvent, class TSM, class TDeps, class TSubs>
  auto operator()(const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    return !call<TEvent, args_t<T, TEvent>, typename TSM::logger_t>::execute(g, event, sm, deps, subs);
  }

 private:
  T g;
};

}  // namespace front

template <class T, __BOOST_SML_REQUIRES(concepts::callable<bool, T>::value)>
auto operator!(const T &t) {
  return front::not_<aux::zero_wrapper<T>>(aux::zero_wrapper<T>{t});
}

template <class T1, class T2, __BOOST_SML_REQUIRES(concepts::callable<bool, T1>::value &&concepts::callable<bool, T2>::value)>
auto operator&&(const T1 &t1, const T2 &t2) {
  return front::and_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}

template <class T1, class T2, __BOOST_SML_REQUIRES(concepts::callable<bool, T1>::value &&concepts::callable<bool, T2>::value)>
auto operator||(const T1 &t1, const T2 &t2) {
  return front::or_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}

template <class T1, class T2, __BOOST_SML_REQUIRES(concepts::callable<void, T1>::value &&concepts::callable<void, T2>::value)>
auto operator,(const T1 &t1, const T2 &t2) {
  return front::seq_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}

#endif
