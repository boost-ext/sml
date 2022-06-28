//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_UTILITY_DISPATCH_TABLE_HPP
#define BOOST_SML_UTILITY_DISPATCH_TABLE_HPP

#include "boost/sml.hpp"

BOOST_SML_NAMESPACE_BEGIN

namespace utility {
namespace concepts {

template <class>
aux::false_type dispatchable_impl(...);

template <class, class TEvent>
auto dispatchable_impl(TEvent &&) -> aux::always<decltype(TEvent::id), decltype(TEvent())>;

template <class T, class TEvent>
auto dispatchable_impl(TEvent &&) -> aux::always<decltype(TEvent::id), decltype(TEvent(aux::declval<T>()))>;

template <class T, class TEvent>
auto dispatchable_impl(TEvent &&) -> aux::always<decltype(TEvent::id), decltype(TEvent(aux::declval<T>(), TEvent::id))>;

template <class, class>
struct dispatchable;

template <class T, class TEvent>
struct is_dispatchable : decltype(dispatchable_impl<T>(aux::declval<TEvent>())) {};

template <class T, class... TEvents>
struct dispatchable<T, aux::type_list<TEvents...>>
    : aux::is_same<aux::bool_list<aux::always<TEvents>::value...>, aux::bool_list<is_dispatchable<T, TEvents>::value...>> {};
}  // namespace concepts

namespace detail {

template <int Id, class TEvent = void>
struct dispatch_event_impl {
  template <class SM, class T, __BOOST_SML_REQUIRES(aux::is_constructible<TEvent>::value &&aux::always<T>::value)>
  static bool execute(SM &sm, const T &) {
    return sm.process_event(TEvent());
  }

  template <class SM, class T, __BOOST_SML_REQUIRES(aux::is_constructible<TEvent, T>::value)>
  static bool execute(SM &sm, const T &data) {
    return sm.process_event(TEvent(data));
  }

  template <class SM, class T, __BOOST_SML_REQUIRES(aux::is_constructible<TEvent, T, int>::value)>
  static bool execute(SM &sm, const T &data) {
    return sm.process_event(TEvent(data, Id));
  }
};

template <int N>
struct dispatch_event_impl<N, void> {
  template <class SM, class T>
  static bool execute(SM &, const T &) {
    return false;
  }
};

template <int, class>
struct events_ids_impl {};

template <class, class>
struct inherit_events_ids_impl;

template <class T, class... Ts>
struct inherit_events_ids_impl<T, aux::type_list<Ts...>> : events_ids_impl<Ts::value, T>... {};

template <class T>
struct events_ids_impl<-1, T> : inherit_events_ids_impl<T, typename T::ids> {};

template <class... Ts>
struct event_id : events_ids_impl<Ts::id, Ts>... {};

template <int N>
dispatch_event_impl<N> get_event_impl(...);

template <int N, class T>
dispatch_event_impl<N, T> get_event_impl(events_ids_impl<N, T> *);

template <int N, class T>
struct get_event {
  using type = decltype(get_event_impl<N>((T *)0));
};

template <int N, class T>
using get_event_t = typename get_event<N, T>::type;

template <class TEvent, int EventRangeBegin, class SM, int... Ns>
auto make_dispatch_table(SM &fsm, const aux::index_sequence<Ns...> &) {
  using events_ids_t = aux::apply_t<event_id, typename SM::events>;
  return [&](const TEvent &e, int id) -> bool {
    using dispatch_table_t = bool (*)(SM &, const TEvent &);
    const static dispatch_table_t dispatch_table[sizeof...(Ns) ? sizeof...(Ns) : 1] = {
        &get_event_t<Ns + EventRangeBegin, events_ids_t>::template execute<SM, TEvent>...};
    return dispatch_table[id - EventRangeBegin](fsm, e);
  };
}
}  // namespace detail

template <int... Ids>
struct id_impl {
  static constexpr auto id = -1 /*dynamic*/;
  using ids = aux::type_list<aux::integral_constant<int, Ids>...>;
};

template <int Id>
struct id_impl<Id> {
  static constexpr auto id = Id;
};

template <int... Ids>
using id = id_impl<Ids...>;

template <class TEvent, int EventRangeBegin, int EventRangeEnd, class SM,
          __BOOST_SML_REQUIRES(concepts::dispatchable<TEvent, typename SM::events>::value)>
auto make_dispatch_table(SM &fsm) {
  static_assert(EventRangeEnd - EventRangeBegin > 0, "Event ids range difference has to be greater than 0");
  return detail::make_dispatch_table<TEvent, EventRangeBegin>(fsm,
                                                              aux::make_index_sequence<EventRangeEnd - EventRangeBegin + 1>{});
}

BOOST_SML_NAMESPACE_END

}  // namespace utility

#endif
