//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_CONCEPTS_TRANSITIONAL_HPP
#define BOOST_SML_CONCEPTS_TRANSITIONAL_HPP

#include "boost/sml/aux_/type_traits.hpp"

namespace detail {
struct on_entry;
struct on_exit;
struct terminate_state;
struct internal;
}  // detail

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

}  // concepts

#endif
