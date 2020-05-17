//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_BACK_UTILITY_HPP
#define BOOST_SML_BACK_UTILITY_HPP

#include "boost/sml/aux_/type_traits.hpp"
#include "boost/sml/aux_/utility.hpp"
#include "boost/sml/back/internals.hpp"
#include "boost/sml/back/policies.hpp"

namespace back {

template <class>
class sm;

template <class>
struct sm_impl;

template <class, class...>
struct sm_policy;

template <class TEvent>
using get_event = aux::conditional_t<aux::is_base_of<internal_event, TEvent>::value, aux::type_list<>, aux::type_list<TEvent>>;

template <class, class, class TEvent>
struct get_all_events_impl {
  using type = get_event<TEvent>;
};

template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<TSrc, TDst, unexpected_event<TEvent>> {
  using type = aux::type_list<TEvent>;
};

template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<sm<TSrc>, TDst, TEvent> {
  using type = aux::join_t<get_event<TEvent>, typename sm<TSrc>::events>;
};

template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<TSrc, sm<TDst>, TEvent> {
  using type = aux::join_t<get_event<TEvent>, typename sm<TDst>::events>;
};

template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<sm<TSrc>, sm<TDst>, TEvent> {
  using type = aux::join_t<get_event<TEvent>, typename sm<TSrc>::events, typename sm<TDst>::events>;
};

template <class, class TEvent>
struct get_sub_internal_events_impl {
  using type = aux::conditional_t<aux::is_base_of<internal_event, TEvent>::value, aux::type_list<TEvent>, aux::type_list<>>;
};

template <class T, class TEvent>
struct get_sub_internal_events_impl<sm<T>, TEvent> {
  using type = aux::join_t<aux::type_list<TEvent>, typename sm_impl<T>::sub_internal_events_t>;
};

template <class... Ts>
using get_all_events =
    aux::join_t<typename get_all_events_impl<typename Ts::src_state, typename Ts::dst_state, typename Ts::event>::type...>;

template <class... Ts>
using get_sub_internal_events =
    aux::join_t<typename get_sub_internal_events_impl<typename Ts::src_state, typename Ts::event>::type...,
                typename get_sub_internal_events_impl<typename Ts::dst_state, typename Ts::event>::type...>;

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
struct get_sub_sm : aux::type_list<> {};

template <class T>
struct get_sub_sm<sm<T>> : aux::join_t<aux::type_list<T>, typename sm<T>::state_machines> {};

template <class... Ts>
using get_sub_sms = aux::join_t<typename get_sub_sm<Ts>::type...>;

template <class... Ts>
using get_sm_t = aux::type_list<typename Ts::sm...>;

template <class... Ts>
using get_non_empty_t =
    aux::join_t<typename aux::conditional<aux::is_empty<Ts>::value, aux::type_list<>, aux::type_list<Ts>>::type...>;

template <class... Ts>
using merge_deps = aux::join_t<typename Ts::deps...>;

template <class>
struct sub_sm;

template <class T>
struct sub_sm<sm_impl<sm_policy<T>>> {
  template <class U, class... TPolicies>
  static sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>> &get(
      aux::pool_type<sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>>> *object) {
    return static_cast<aux::pool_type<sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>>> &>(*object).value;
  }

  template <class... TPolicies>
  static sm_impl<sm_policy<T, TPolicies...>> &get(aux::pool_type<sm_impl<sm_policy<T, TPolicies...>>> *object) {
    return static_cast<aux::pool_type<sm_impl<sm_policy<T, TPolicies...>>> &>(*object).value;
  }

  template <class... TPolicies>
  static const sm_impl<sm_policy<T, TPolicies...>> &cget(const aux::pool_type<sm_impl<sm_policy<T, TPolicies...>>> *object) {
    return static_cast<const aux::pool_type<sm_impl<sm_policy<T, TPolicies...>>> &>(*object).value;
  }
};

template <class T, class U>
struct sub_sm<sm_impl<sm_policy<T, aux::identity<U>>>> {
  template <class... TPolicies>
  static sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>> &get(
      aux::pool_type<sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>>> *object) {
    return static_cast<aux::pool_type<sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>>> &>(*object).value;
  }

  template <class... TPolicies>
  static const sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>> &cget(
      const aux::pool_type<sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>>> *object) {
    return static_cast<const aux::pool_type<sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>>> &>(*object).value;
  }
};

template <class T, class... TPolicies>
struct rebind_impl {
  using type = sm_policy<T, TPolicies...>;
};

template <class T, class... TDetails, class... TPolicies>
struct rebind_impl<sm_policy<T, TDetails...>, TPolicies...> {
  using type = sm_policy<T, TDetails..., TPolicies...>;
};

template <class T, class... TDetails, class... TPolicies>
struct rebind_impl<sm<sm_policy<T, TDetails...>>, TPolicies...> {
  using type = sm_policy<T, TDetails..., TPolicies...>;
};

template <class, class>
struct convert_to_sm;

template <class T, class... Ts>
struct convert_to_sm<T, aux::type_list<Ts...>> {
  using type = aux::type_list<sm_impl<T>, sm_impl<typename T::template rebind<Ts>>...>;
};

}  // namespace back

#endif
