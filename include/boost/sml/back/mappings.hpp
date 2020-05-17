//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_BACK_MAPPINGS_HPP
#define BOOST_SML_BACK_MAPPINGS_HPP

#include "boost/sml/back/transitions.hpp"

namespace back {

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

template <class, class TUnexpected>
transitions<TUnexpected> get_state_mapping_impl(...);

template <class T, class, class... Ts>
transitions<Ts...> get_state_mapping_impl(state_mappings<T, aux::type_list<Ts...>> *);

template <class T, class TMappings, class TUnexpected>
struct get_state_mapping {
  using type = decltype(get_state_mapping_impl<T, TUnexpected>((TMappings *)0));
};

template <class S>
transitions_sub<S> get_sub_state_mapping_impl(...);

template <class T, class... Ts>
transitions_sub<T, Ts...> get_sub_state_mapping_impl(state_mappings<T, aux::type_list<Ts...>> *);

template <class T, class TMappings, class TUnexpected>
struct get_state_mapping<sm<T>, TMappings, TUnexpected> {
  using type = decltype(get_sub_state_mapping_impl<sm<T>>((TMappings *)0));
};

template <class T, class TMappings, class TUnexpected>
using get_state_mapping_t = typename get_state_mapping<T, TMappings, TUnexpected>::type;

template <class>
transitions<aux::true_type> get_event_mapping_impl(...);

template <class T, class TMappings>
TMappings get_event_mapping_impl(event_mappings<T, TMappings> *);

template <class T, class... T1Mappings, class... T2Mappings>
unique_mappings_t<T1Mappings..., T2Mappings...> get_event_mapping_impl(event_mappings<T, aux::inherit<T1Mappings...>> *,
                                                                       event_mappings<_, aux::inherit<T2Mappings...>> *);

template <class T, class TMappings>
struct get_event_mapping_impl_helper
    : aux::conditional<aux::is_same<transitions<aux::true_type>, decltype(get_event_mapping_impl<_>((TMappings *)0))>::value,
                       decltype(get_event_mapping_impl<T>((TMappings *)0)),
                       decltype(get_event_mapping_impl<T>((TMappings *)0, (TMappings *)0))>::type {};

template <class T, class TMappings>
struct get_event_mapping_impl_helper<exception<T>, TMappings> : decltype(get_event_mapping_impl<exception<T>>((TMappings *)0)) {
};

template <class T1, class T2, class TMappings>
struct get_event_mapping_impl_helper<unexpected_event<T1, T2>, TMappings>
    : decltype(get_event_mapping_impl<unexpected_event<T1, T2>>((TMappings *)0)) {};

template <class T1, class T2, class TMappings>
struct get_event_mapping_impl_helper<on_entry<T1, T2>, TMappings>
    : decltype(get_event_mapping_impl<on_entry<T1, T2>>((TMappings *)0)) {};

template <class T1, class T2, class TMappings>
struct get_event_mapping_impl_helper<on_exit<T1, T2>, TMappings>
    : decltype(get_event_mapping_impl<on_exit<T1, T2>>((TMappings *)0)) {};

template <class T, class TMappings>
using get_event_mapping_t = get_event_mapping_impl_helper<T, TMappings>;

}  // namespace back

#endif
