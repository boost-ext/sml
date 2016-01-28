//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/msm-lite.hpp"

namespace boost {
namespace msm {
namespace lite {
inline namespace v_1_0_0 {
namespace aux {

test remove_reference_types = [] {
  static_expect(is_same<int, remove_reference_t<int>>::value);
  static_expect(is_same<int, remove_reference_t<int&>>::value);
  static_expect(is_same<int, remove_reference_t<const int&>>::value);
  static_expect(is_same<int, remove_reference_t<int&&>>::value);
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
  static_expect(is_same<index_sequence<1>, make_index_sequence<1>>::value);
  static_expect(is_same<index_sequence<1, 2>, make_index_sequence<2>>::value);
  static_expect(is_same<index_sequence<1, 2, 3>, make_index_sequence<3>>::value);
  static_expect(is_same<index_sequence<1, 2, 3, 4>, make_index_sequence<4>>::value);
  static_expect(is_same<index_sequence<1, 2, 3, 4, 5>, make_index_sequence<5>>::value);
  static_expect(is_same<index_sequence<1, 2, 3, 4, 5, 6>, make_index_sequence<6>>::value);
  static_expect(is_same<index_sequence<1, 2, 3, 4, 5, 6, 7>, make_index_sequence<7>>::value);
  static_expect(is_same<index_sequence<1, 2, 3, 4, 5, 6, 7, 8>, make_index_sequence<8>>::value);
  static_expect(is_same<index_sequence<1, 2, 3, 4, 5, 6, 7, 8, 9>, make_index_sequence<9>>::value);
  static_expect(is_same<index_sequence<1, 2, 3, 4, 5, 6, 7, 8, 9, 10>, make_index_sequence<10>>::value);
};

test join_types = [] {
  static_expect(is_same<type_list<>, join_t<>>::value);
  static_expect(is_same<type_list<int>, join_t<type_list<int>>>::value);
  static_expect(is_same<type_list<int, double>, join_t<type_list<int>, type_list<double>>>::value);
  static_expect(is_same<type_list<int, float, double>, join_t<type_list<int>, type_list<float, double>>>::value);
  static_expect(is_same<type_list<float, double>, join_t<type_list<>, type_list<float, double>>>::value);
};

}  // aux

namespace concepts {

struct call {};
struct call1 {
  void operator()();
};
struct call2 {
  void operator()(int) const {}
};
struct call3 {
  auto operator()(int) const { return true; }
};
struct call4 {
  bool operator()(int) const noexcept { return true; }
};
struct call5 {
  template <class T>
  bool operator()(T) const noexcept {
    return true;
  }
};
struct call6 {
  template <class T>
  bool operator()(T&, int) const noexcept {
    return true;
  }
};

auto calll1 = [] {};
auto calll2 = [](int) {};
auto calll3 = [] { return true; };

test callable_types = [] {
  static_expect(!callable<void, call>::value);
  static_expect(callable<void, call1>::value);
  static_expect(callable<void, call2>::value);
  static_expect(callable<bool, call3>::value);
  static_expect(callable<bool, call4>::value);
  static_expect(callable<bool, call5>::value);
  static_expect(callable<bool, call6>::value);
  static_expect(callable<void, decltype(calll1)>::value);
  static_expect(callable<void, decltype(calll2)>::value);
  static_expect(callable<bool, decltype(calll3)>::value);
  (void)calll1;
  (void)calll2;
  (void)calll3;
};
}  // concepts
}  // v_1_0_0
}  // lite
}  // msm
}  // boost
