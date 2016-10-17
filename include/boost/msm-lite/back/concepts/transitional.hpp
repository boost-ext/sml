#ifndef TRANSITIONAL_22NW4S2T
#define TRANSITIONAL_22NW4S2T

#include "boost/msm-lite/aux_/type_traits.hpp"

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

}  // concepts

#endif
