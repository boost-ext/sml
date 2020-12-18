//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_AUX_TYPE_TRAITS_HPP
#define BOOST_SML_AUX_TYPE_TRAITS_HPP

#define __BOOST_SML_REQUIRES(...) typename aux::enable_if<__VA_ARGS__, int>::type = 0  // __pph__

namespace aux {

using byte = unsigned char;

struct none_type {};
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
struct identity {
  using type = T;
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

template <class... Ts>
using void_t = void;

template <class...>
struct always : true_type {};

template <class...>
struct never : false_type {};

namespace detail {
template <bool>
struct conditional;
template <>
struct conditional<false> {
  template <class, class T>
  using fn = T;
};
template <>
struct conditional<true> {
  template <class T, class>
  using fn = T;
};
}  // namespace detail

template <bool B, class T, class F>
struct conditional {
  using type = typename detail::conditional<B>::template fn<T, F>;
};
template <bool B, class T, class F>
using conditional_t = typename detail::conditional<B>::template fn<T, F>;

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
#if defined(_MSC_VER) && !defined(__clang__)  // __pph__
struct is_base_of : integral_constant<bool, __is_base_of(T, U)> {
};
#else   // __pph__
using is_base_of = integral_constant<bool, __is_base_of(T, U)>;
#endif  // __pph__

template <class T, class... TArgs>
decltype(T(declval<TArgs>()...), true_type{}) test_is_constructible(int);
template <class, class...>
false_type test_is_constructible(...);
template <class T, class... TArgs>
#if defined(_MSC_VER) && !defined(__clang__)  // __pph__
struct is_constructible : decltype(test_is_constructible<T, TArgs...>(0)) {
};
#else   // __pph__
using is_constructible = decltype(test_is_constructible<T, TArgs...>(0));
#endif  // __pph__

template <class T, class U>
struct is_empty_base : T {
  U _;
};

template <class T>
struct is_empty : aux::integral_constant<bool, sizeof(is_empty_base<T, none_type>) == sizeof(none_type)> {};

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
#if __cplusplus > 201402L && __cpp_noexcept_function_type >= 201510  // __pph__
template <class R, class... TArgs>
struct function_traits<R (*)(TArgs...) noexcept> {
  using args = type_list<TArgs...>;
};
template <class R, class... TArgs>
struct function_traits<R(TArgs...) noexcept> {
  using args = type_list<TArgs...>;
};
template <class R, class T, class... TArgs>
struct function_traits<R (T::*)(TArgs...) noexcept> {
  using args = type_list<TArgs...>;
};
template <class R, class T, class... TArgs>
struct function_traits<R (T::*)(TArgs...) const noexcept> {
  using args = type_list<TArgs...>;
};
#endif  // __pph__
template <class T>
using function_traits_t = typename function_traits<T>::args;

template <class T>
struct remove_const {
  using type = T;
};
template <class T>
struct remove_const<const T> {
  using type = T;
};
template <class T>
using remove_const_t = typename remove_const<T>::type;

template <class T>
struct remove_reference {
  using type = T;
};
template <class T>
struct remove_reference<T &> {
  using type = T;
};
template <class T>
struct remove_reference<T &&> {
  using type = T;
};
template <class T>
using remove_reference_t = typename remove_reference<T>::type;

}  // namespace aux

#endif
