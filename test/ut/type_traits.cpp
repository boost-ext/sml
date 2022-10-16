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

test is_same_types = [] {
  static_expect(!is_same<int, double>::value);
  static_expect(is_same<int, int>::value);
  static_expect(is_same<const double&, const double&>::value);
};

test remove_reference_types = [] {
  static_expect(is_same<int, remove_reference_t<int>>::value);
  static_expect(is_same<int, remove_reference_t<int&>>::value);
  static_expect(is_same<int, remove_reference_t<int&&>>::value);

  static_expect(!is_same<int, remove_reference_t<const int&>>::value);
};

void f1() {}
int f2(int) { return {}; }
int f3(int, const double&) { return {}; }

struct c1 {
  void f1() {}
  int f2(int) { return {}; }
  int f3(int, const double&) { return {}; }
};

struct c2 {
  void f1() const {}
  int f2(int) const { return {}; }
  int f3(int, const double&) const { return {}; }
};

test function_traits_parameters_type_functions = [] {
  static_expect(is_same<type_list<>, typename function_traits<decltype(&f1)>::args>::value);
  static_expect(is_same<type_list<int>, typename function_traits<decltype(&f2)>::args>::value);
  static_expect(is_same<type_list<int, const double&>, typename function_traits<decltype(&f3)>::args>::value);
};

test function_traits_parameters_type_methods = [] {
  static_expect(is_same<type_list<>, typename function_traits<decltype(&c1::f1)>::args>::value);
  static_expect(is_same<type_list<int>, typename function_traits<decltype(&c1::f2)>::args>::value);
  static_expect(is_same<type_list<int, const double&>, typename function_traits<decltype(&c1::f3)>::args>::value);
};

test function_traits_parameters_type_const_methods = [] {
  static_expect(is_same<type_list<>, typename function_traits<decltype(&c2::f1)>::args>::value);
  static_expect(is_same<type_list<int>, typename function_traits<decltype(&c2::f2)>::args>::value);
  static_expect(is_same<type_list<int, const double&>, typename function_traits<decltype(&c2::f3)>::args>::value);
};

test is_empty_type = [] {
  struct empty {};
  struct non_empty {
    int _;
  };
  static_expect(is_empty<empty>::value);
  static_expect(!is_empty<non_empty>::value);
};

}  // namespace aux
BOOST_SML_NAMESPACE_END
