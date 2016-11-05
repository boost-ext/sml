//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
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

template <class...>
struct is_valid_event : aux::true_type {};

template <class>
aux::false_type dispatchable_impl(...);

template <class, class TEvent>
auto dispatchable_impl(TEvent &&) -> is_valid_event<decltype(TEvent::id), decltype(TEvent())>;

template <class T, class TEvent>
auto dispatchable_impl(TEvent &&) -> is_valid_event<decltype(TEvent::id), decltype(TEvent(aux::declval<T>()))>;

template <class, class>
struct dispatchable;

template <class T, class TEvent>
struct is_dispatchable : decltype(dispatchable_impl<T>(aux::declval<TEvent>())) {};

template <class T, class... TEvents>
struct dispatchable<T, aux::type_list<TEvents...>>
    : aux::is_same<aux::bool_list<aux::always<TEvents>::value...>, aux::bool_list<is_dispatchable<T, TEvents>::value...>> {};
}  // concepts

namespace detail {

template <class TEvent = void>
struct dispatch_event_impl {
  template <class T, class = void>
  struct execute_impl {
    template <class SM>
    static void execute(SM &sm, const T &) {
      sm.process_event(TEvent());
    }
  };

  template <class T>
  struct execute_impl<T, aux::void_t<decltype(TEvent{aux::declval<T>()})>> {
    template <class SM>
    static void execute(SM &sm, const T &data) {
      sm.process_event(TEvent(data));
    }
  };

  template <class SM, class T>
  static void execute(SM &sm, const T &data) {
    execute_impl<T>::execute(sm, data);
  }
};

template <>
struct dispatch_event_impl<void> {
  template <class SM, class T>
  static void execute(SM &, const T &) {}
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
auto make_dispatch_table(sm<SM> &fsm, const aux::index_sequence<Ns...> &) {
  using events_ids = aux::apply_t<event_id, typename sm<SM>::events>;
  return [&](const TEvent &event, int id) {
    using dispatch_table_t = void (*)(sm<SM> &, const TEvent &);
    const static dispatch_table_t dispatch_table[sizeof...(Ns) ? sizeof...(Ns) : 1] = {
        &get_event_t<Ns + EventRangeBegin, events_ids>::template execute<sm<SM>, TEvent>...};
    dispatch_table[id - EventRangeBegin](fsm, event);
  };
}
}  // detail

template <class TEvent, int EventRangeBegin, int EventRangeEnd, class SM,
          __BOOST_SML_REQUIRES(concepts::dispatchable<TEvent, typename sm<SM>::events>::value)>
auto make_dispatch_table(sm<SM> &fsm) {
  static_assert(EventRangeEnd - EventRangeBegin > 0, "Event ids range difference has to be greater than 0");
  return detail::make_dispatch_table<TEvent, EventRangeBegin>(fsm,
                                                              aux::make_index_sequence<EventRangeEnd - EventRangeBegin + 1>{});
}

BOOST_SML_NAMESPACE_END

}  // utility

#endif
