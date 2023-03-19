//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/sml.hpp"

BOOST_SML_NAMESPACE_BEGIN
namespace aux {

test unique_types = [] {
  static_expect(is_same<type_list<>, unique_t<>>::value);
  static_expect(is_same<type_list<int>, unique_t<int>>::value);
  static_expect(is_same<type_list<int>, unique_t<int, int>>::value);
  static_expect(is_same<type_list<int>, unique_t<int, int, int>>::value);
  static_expect(is_same<type_list<int, double>, unique_t<int, double, int, int>>::value);
  static_expect(is_same<type_list<double, int, float>, unique_t<double, int, int, int, float>>::value);
  static_expect(is_same<type_list<double, int, float>, unique_t<double, int, int, int, float, double, int>>::value);
};

test index_sequence_types = [] {
  static_expect(is_same<index_sequence<>, make_index_sequence<0>>::value);
  static_expect(is_same<index_sequence<0>, make_index_sequence<1>>::value);
  static_expect(is_same<index_sequence<0, 1>, make_index_sequence<2>>::value);
  static_expect(is_same<index_sequence<0, 1, 2>, make_index_sequence<3>>::value);
  static_expect(is_same<index_sequence<0, 1, 2, 3>, make_index_sequence<4>>::value);
  static_expect(is_same<index_sequence<0, 1, 2, 3, 4>, make_index_sequence<5>>::value);
  static_expect(is_same<index_sequence<0, 1, 2, 3, 4, 5>, make_index_sequence<6>>::value);
  static_expect(is_same<index_sequence<0, 1, 2, 3, 4, 5, 6>, make_index_sequence<7>>::value);
  static_expect(is_same<index_sequence<0, 1, 2, 3, 4, 5, 6, 7>, make_index_sequence<8>>::value);
  static_expect(is_same<index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8>, make_index_sequence<9>>::value);
  static_expect(is_same<index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9>, make_index_sequence<10>>::value);
};

test join_types = [] {
  static_expect(is_same<type_list<>, join_t<>>::value);
  static_expect(is_same<type_list<int>, join_t<type_list<int>>>::value);
  static_expect(is_same<type_list<int, double>, join_t<type_list<int>, type_list<double>>>::value);
  static_expect(is_same<type_list<int, float, double>, join_t<type_list<int>, type_list<float, double>>>::value);
  static_expect(is_same<type_list<float, double>, join_t<type_list<>, type_list<float, double>>>::value);
};

template <class... Ts>
using make_type_list = type_list<Ts...>;

test apply_types = [] {
  using types = type_list<int, double, float>;
  static_expect(is_same<types, apply_t<make_type_list, types>>::value);
};

test tuple_empty = [] {
  tuple<> t;
  (void)t;
};

test tuple_basic = [] {
  tuple<int, double> t{42, 87.0};
  expect(42 == get_by_id<0>(&t));
  expect(87.0 == get_by_id<1>(&t));
};

test tuple_same_types = [] {
  tuple<int, int> t{42, 87};
  expect(42 == get_by_id<0>(&t));
  expect(87 == get_by_id<1>(&t));
};

test pool_empty = [] {
  pool<> p;
  static_expect(0 == size<decltype(p)>::value);
  (void)p;
};

test pool_basic = [] {
  pool<int, double> p{42, 87.0};
  static_expect(2 == size<decltype(p)>::value);
  expect(42 == get<int>(p));
  expect(87.0 == get<double>(p));
};

test pool_init_from_other_pool = [] {
  pool<double, int> p{87.0, 42};
  pool<int, double> p2{init{}, static_cast<decltype(p)&&>(p)};
  expect(42 == get<int>(p2));
  expect(87.0 == get<double>(p2));
};

test is_specialization = [] {
  static_expect(!is<pool, int>::value);
  static_expect(!is<pool, tuple<>>::value);
  static_expect(!is<pool, tuple<int>>::value);
  static_expect(is<pool, pool<>>::value);
  static_expect(is<pool, pool<int, double>>::value);
};

test type_id_basic = [] {
  using type_ids = type_id<int, double>;
  expect(0 == get_id<int, int>((type_ids*)0));
  expect(1 == get_id<int, double>((type_ids*)0));
};

}  // namespace aux
BOOST_SML_NAMESPACE_END
