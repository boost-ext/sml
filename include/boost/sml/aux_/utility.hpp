//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_AUX_UTILITY_HPP
#define BOOST_SML_AUX_UTILITY_HPP

#include "boost/sml/aux_/type_traits.hpp"

namespace aux {

template <int...>
struct index_sequence {
  using type = index_sequence;
};
#if __has_builtin(__make_integer_seq)  // __pph__
template <class T, T...>
struct integer_sequence;
template <int... Ns>
struct integer_sequence<int, Ns...> {
  using type = index_sequence<Ns...>;
};
template <int N>
struct make_index_sequence_impl {
  using type = typename __make_integer_seq<integer_sequence, int, N>::type;
};
#else   // __pph__
template <class, class>
struct concat;
template <int... I1, int... I2>
struct concat<index_sequence<I1...>, index_sequence<I2...>> : index_sequence<I1..., (sizeof...(I1) + I2)...> {};
template <int N>
struct make_index_sequence_impl
    : concat<typename make_index_sequence_impl<N / 2>::type, typename make_index_sequence_impl<N - N / 2>::type>::type {};
template <>
struct make_index_sequence_impl<0> : index_sequence<> {};
template <>
struct make_index_sequence_impl<1> : index_sequence<0> {};
#endif  // __pph__

template <int N>
using make_index_sequence = typename make_index_sequence_impl<N>::type;

template <class... Ts>
struct join {
  using type = type_list<>;
};
template <class T>
struct join<T> {
  using type = T;
};
template <class... T1s, class... T2s, class... Ts>
struct join<type_list<T1s...>, type_list<T2s...>, Ts...> : join<type_list<T1s..., T2s...>, Ts...> {};
template <class... Ts, class... T1s, class... T2s, class... T3s, class... T4s, class... T5s, class... T6s, class... T7s,
          class... T8s, class... T9s, class... T10s, class... T11s, class... T12s, class... T13s, class... T14s, class... T15s,
          class... T16s, class... Us>
struct join<type_list<Ts...>, type_list<T1s...>, type_list<T2s...>, type_list<T3s...>, type_list<T4s...>, type_list<T5s...>,
            type_list<T6s...>, type_list<T7s...>, type_list<T8s...>, type_list<T9s...>, type_list<T10s...>, type_list<T11s...>,
            type_list<T12s...>, type_list<T13s...>, type_list<T14s...>, type_list<T15s...>, type_list<T16s...>, Us...>
    : join<type_list<Ts..., T1s..., T2s..., T3s..., T4s..., T5s..., T6s..., T7s..., T8s..., T9s..., T10s..., T11s..., T12s...,
                     T13s..., T14s..., T15s..., T16s...>,
           Us...> {};
template <class... TArgs>
using join_t = typename join<TArgs...>::type;

template <class, class...>
struct unique_impl;
template <class T1, class T2, class... Rs, class... Ts>
struct unique_impl<type<T1, Rs...>, T2, Ts...>
    : conditional_t<is_base_of<type<T2>, T1>::value, unique_impl<type<inherit<T1>, Rs...>, Ts...>,
                    unique_impl<type<inherit<T1, type<T2>>, Rs..., T2>, Ts...>> {};
template <class T1, class... Rs>
struct unique_impl<type<T1, Rs...>> : type_list<Rs...> {};
template <class... Ts>
struct unique : unique_impl<type<none_type>, Ts...> {};
template <class T>
struct unique<T> : type_list<T> {};
template <class... Ts>
using unique_t = typename unique<Ts...>::type;

template <template <class...> class, class>
struct apply;
template <template <class...> class T, template <class...> class U, class... Ts>
struct apply<T, U<Ts...>> {
  using type = T<Ts...>;
};
template <template <class...> class T, class D>
using apply_t = typename apply<T, D>::type;

template <int, class T>
struct tuple_type {
  T value;
};
template <class, class...>
struct tuple_impl;
template <int... Ns, class... Ts>
struct tuple_impl<index_sequence<Ns...>, Ts...> : tuple_type<Ns, Ts>... {
  using boost_di_inject__ = aux::type_list<Ts...>;
  explicit tuple_impl(Ts... ts) : tuple_type<Ns, Ts>{ts}... {}
};
template <>
struct tuple_impl<index_sequence<0>> {
  aux::byte _[0];
};
template <class... Ts>
using tuple = tuple_impl<make_index_sequence<sizeof...(Ts)>, Ts...>;

template <int N, class T>
auto &get_by_id_impl(tuple_type<N, T> *object) {
  return static_cast<tuple_type<N, T> &>(*object).value;
}
template <int N, class Tuple>
auto &get_by_id(Tuple &t) {
  return get_by_id_impl<N>(&t);
}
struct init {};

template <class T>
struct pool_type {
  explicit pool_type(const T &object) : value(object) {}

  template <class U>
  pool_type(const init &i, const U &object) : value(i, object) {}

  T value;
};

template <class T>
aux::remove_reference_t<T> try_get(...) {
  return {};
}
template <class T>
T &try_get(pool_type<T> *object) {
  return static_cast<pool_type<T> &>(*object).value;
}
template <class T>
T &try_get(pool_type<T &> *object) {
  return static_cast<pool_type<T &> &>(*object).value;
}
template <class T, class TPool>
decltype(auto) get(TPool &p) {
  return static_cast<pool_type<T> &>(p).value;
}

template <class... Ts>
struct pool : pool_type<Ts>... {
  using boost_di_inject__ = aux::type_list<Ts...>;

  explicit pool(Ts... ts) : pool_type<Ts>(ts)... {}

  template <class... TArgs>
  pool(init &&, pool<TArgs...> &&p) : pool_type<Ts>(aux::try_get<Ts>(&p))... {}

  template <class... TArgs>
  pool(const pool<TArgs...> &p) : pool_type<Ts>(init{}, &p)... {}
};
template <>
struct pool<> {
  explicit pool(...) {}
  aux::byte _[0];
};

template <class>
struct is_pool : aux::false_type {};
template <class... Ts>
struct is_pool<pool<Ts...>> : aux::true_type {};

template <int, class T>
struct type_id_type {};
template <class, class...>
struct type_id_impl;
template <int... Ns, class... Ts>
struct type_id_impl<index_sequence<Ns...>, Ts...> : type_id_type<Ns, Ts>... {};
template <class... Ts>
struct type_id : type_id_impl<make_index_sequence<sizeof...(Ts)>, Ts...> {};

template <class T, int, int N>
constexpr auto get_id_impl(type_id_type<N, T> *) {
  return N;
}
template <class T, int D>
constexpr auto get_id_impl(...) {
  return D;
}
template <class TIds, int D, class T>
constexpr auto get_id() {
  return get_id_impl<T, D>((TIds *)0);
}

template <class>
struct size;
template <template <class...> class T, class... Ts>
struct size<T<Ts...>> {
  static constexpr auto value = sizeof...(Ts);
};

template <int... Ts>
constexpr auto max() {
  auto max = 0;
  int _[]{0, (Ts > max ? max = Ts : max)...};
  (void)_;
  return max;
}

template <class... Ts>
struct variant {
  using ids_t = type_id<Ts...>;
  alignas(max<alignof(Ts)...>()) byte data[max<sizeof(Ts)...>()];

  template <class T>
  variant(T object) {  // non explicit
    id = get_id<ids_t, -1, T>();
    new (&data) T(static_cast<T &&>(object));
  }

  int id = -1;
};

template <class TExpr, class = void>
struct zero_wrapper : TExpr {
  explicit zero_wrapper(const TExpr &expr) : TExpr(expr) {}
};

template <class, class>
struct zero_wrapper_impl;

template <class TExpr, class... TArgs>
struct zero_wrapper_impl<TExpr, aux::type_list<TArgs...>> {
  auto operator()(TArgs... args) const { return reinterpret_cast<const TExpr &>(*this)(args...); }
  aux::byte _[0];
};

template <class TExpr>
struct zero_wrapper<TExpr, void_t<decltype(+declval<TExpr>())>>
    : zero_wrapper_impl<TExpr, aux::function_traits_t<decltype(&TExpr::operator())>> {
  zero_wrapper(...) {}
};
}  // aux

#endif
