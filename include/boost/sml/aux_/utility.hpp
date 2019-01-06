//
// Copyright (c) 2016-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_AUX_UTILITY_HPP
#define BOOST_SML_AUX_UTILITY_HPP

#include "boost/sml/aux_/type_traits.hpp"

namespace aux {

using swallow = int[];

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

template <class...>
struct join {
  using type = type_list<>;
};
template <class T>
struct join<T> {
  using type = T;
};
template <class... Ts>
struct join<type_list<Ts...>> : type_list<Ts...> {};
template <class... T1s, class... T2s>
struct join<type_list<T1s...>, type_list<T2s...>> : type_list<T1s..., T2s...> {};
template <class... T1s, class... T2s, class... T3s>
struct join<type_list<T1s...>, type_list<T2s...>, type_list<T3s...>> : type_list<T1s..., T2s..., T3s...> {};

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

template <class, class...>
struct is_unique;

template <class T>
struct is_unique<T> : true_type {};

template <class T1, class T2, class... Ts>
struct is_unique<T1, T2, Ts...>
    : conditional_t<is_base_of<type<T2>, T1>::value, false_type, is_unique<inherit<T1, type<T2>>, Ts...>> {};

template <class... Ts>
using is_unique_t = is_unique<none_type, Ts...>;

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
  explicit tuple_type(const T &object) : value(object) {}
  T value;
};
template <class, class...>
struct tuple_impl;
template <int... Ns, class... Ts>
struct tuple_impl<index_sequence<Ns...>, Ts...> : tuple_type<Ns, Ts>... {
  explicit tuple_impl(Ts... ts) : tuple_type<Ns, Ts>(ts)... {}
};
template <>
struct tuple_impl<index_sequence<0>> {
  __BOOST_SML_ZERO_SIZE_ARRAY(byte);
};
template <class... Ts>
using tuple = tuple_impl<make_index_sequence<sizeof...(Ts)>, Ts...>;

template <int N, class T>
T &get_by_id(tuple_type<N, T> *object) {
  return static_cast<tuple_type<N, T> &>(*object).value;
}

struct init {};
struct pool_type_base {
  __BOOST_SML_ZERO_SIZE_ARRAY(byte);
};

template <class T>
struct pool_type : pool_type_base {
  explicit pool_type(T object) : value{object} {}

  template <class TObject>
  pool_type(init i, TObject object) : value{i, object} {}

  T value;
};

template <class T>
struct missing_ctor_parameter {
  static constexpr auto value = false;

  auto operator()() const { return T{}(); }

  template <class U, __BOOST_SML_REQUIRES(!aux::is_base_of<pool_type_base, U>::value && aux::is_constructible<U>::value)>
  operator U() {
    return {};
  }

#if !defined(_MSC_VER)  // __pph__
  template <class TMissing, __BOOST_SML_REQUIRES(!aux::is_base_of<pool_type_base, TMissing>::value)>
  operator TMissing &() const {
    static_assert(missing_ctor_parameter<TMissing>::value,
                  "State Machine is missing a constructor parameter! Check out the `missing_ctor_parameter` error to see the "
                  "missing type.");
  }
#endif  // __pph__
};

template <class T>
missing_ctor_parameter<T> try_get(...) {
  return {};
}

template <class T>
T try_get(const pool_type<T> *object) {
  return object->value;
}

template <class T>
const T &try_get(const pool_type<const T &> *object) {
  return object->value;
}

template <class T>
T &try_get(const pool_type<T &> *object) {
  return object->value;
}

template <class T, class TPool>
T &get(TPool &p) {
  return static_cast<pool_type<T> &>(p).value;
}

template <class T, class TPool>
const T &cget(const TPool &p) {
  return static_cast<const pool_type<T> &>(p).value;
}

template <class... Ts>
struct pool : pool_type<Ts>... {
  using boost_di_inject__ = type_list<Ts...>;

  pool() = default;

  explicit pool(Ts... ts) : pool_type<Ts>(ts)... {}

  template <class... TArgs>
  pool(init, const pool<TArgs...> &p) : pool_type<Ts>(try_get<aux::remove_const_t<aux::remove_reference_t<Ts>>>(&p))... {}

  template <class... TArgs>
  pool(const pool<TArgs...> &p) : pool_type<Ts>(init{}, p)... {}
};

template <>
struct pool<> {
  using boost_di_inject__ = type_list<>;

  pool() = default;

  template <class... Ts>
  explicit pool(Ts &&...) {}

  __BOOST_SML_ZERO_SIZE_ARRAY(byte);
};

template <int, class>
struct type_id_type {};
template <class, class...>
struct type_id_impl;
template <int... Ns, class... Ts>
struct type_id_impl<index_sequence<Ns...>, Ts...> : type_id_type<Ns, Ts>... {};
template <class... Ts>
struct type_id : type_id_impl<make_index_sequence<sizeof...(Ts)>, Ts...> {};

template <class R, class T, int N>
constexpr R get_id(type_id_type<N, T> *) {
  return static_cast<R>(N);
}

template <template <class...> class, class T>
struct is : false_type {};
template <template <class...> class T, class... Ts>
struct is<T, T<Ts...>> : true_type {};

template <class>
struct size;
template <template <class...> class T, class... Ts>
struct size<T<Ts...>> {
  static constexpr auto value = sizeof...(Ts);
};

#if defined(_MSC_VER)  // __pph__
constexpr int max_impl() { return 0; }
constexpr int max_impl(int r) { return r; }
constexpr int max_impl(int r, int i) { return r > i ? r : i; }
constexpr int max_impl(int r, int i, int ints...) { return i > r ? max_impl(i, ints) : max_impl(r, ints); }
template <int... Ts>
constexpr int max() {
  return max_impl(Ts...);
}
#else   // __pph__
template <int... Ts>
constexpr int max() {
  int max = 0;
  (void)swallow{0, (Ts > max ? max = Ts : max)...};
  return max;
}
#endif  // __pph__

template <class TExpr, class = void>
struct zero_wrapper : TExpr {
  using type = TExpr;
  explicit zero_wrapper(const TExpr &expr) : TExpr(expr) {}
  const TExpr &get() const { return *this; }
};

template <class, class>
struct zero_wrapper_impl;

template <class TExpr, class... TArgs>
struct zero_wrapper_impl<TExpr, type_list<TArgs...>> {
  auto operator()(TArgs... args) const { return reinterpret_cast<const TExpr &>(*this)(args...); }
  __BOOST_SML_ZERO_SIZE_ARRAY(byte);
};

template <class TExpr>
struct zero_wrapper<TExpr, void_t<decltype(+declval<TExpr>())>>
    : zero_wrapper_impl<TExpr, function_traits_t<decltype(&TExpr::operator())>> {
  using type = TExpr;
  template <class... Ts>
  zero_wrapper(Ts &&...) {}
  const TExpr &get() const { return reinterpret_cast<const TExpr &>(*this); }
};

namespace detail {
template <class, int N, int... Ns>
auto get_type_name(const char *ptr, index_sequence<Ns...>) {
  static const char str[] = {ptr[N + Ns]..., 0};
  return str;
}
}  // namespace detail

template <class T>
const char *get_type_name() {
#if defined(_MSC_VER)  // __pph__
  return detail::get_type_name<T, 34>(__FUNCSIG__, make_index_sequence<sizeof(__FUNCSIG__) - 34 - 8>{});
#elif defined(__clang__)  // __pph__
  return detail::get_type_name<T, 58>(__PRETTY_FUNCTION__, make_index_sequence<sizeof(__PRETTY_FUNCTION__) - 58 - 2>{});
#elif defined(__GNUC__)   // __pph__
  return detail::get_type_name<T, 63>(__PRETTY_FUNCTION__, make_index_sequence<sizeof(__PRETTY_FUNCTION__) - 63 - 2>{});
#endif                    // __pph__
}

template <class T, T...>
struct string;

template <char... Chrs>
struct string<char, Chrs...> {
  using type = string;
  static auto c_str() {
    static constexpr char str[] = {Chrs..., 0};
    return str;
  }
};

template <class T>
struct string<T> {
  using type = T;
  static auto c_str() { return c_str_impl((T *)0); }
  template <class U>
  static decltype(U::c_str()) c_str_impl(U *) {
    return U::c_str();
  }
  static auto c_str_impl(...) { return get_type_name<T>(); }
};

}  // namespace aux

#endif
