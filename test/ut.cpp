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
test is_constructible_types = [] {
  struct c {
    c(int, double) {}
  };
  static_expect(is_constructible<int>::value);
  static_expect(is_constructible<c, int, double>::value);
  static_expect(!is_constructible<c, int, double, float>::value);
  static_expect(!is_constructible<c, int>::value);
  static_expect(!is_constructible<c, double>::value);
};

test is_same_types = [] {
  static_expect(!is_same<int, double>::value);
  static_expect(is_same<int, int>::value);
  static_expect(is_same<const double&, const double&>::value);
};

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
using identity = type_list<Ts...>;

test apply_types = [] {
  using types = type_list<int, double, float>;
  static_expect(is_same<types, apply_t<identity, types>>::value);
};

test tuple_empty = [] {
  tuple<> t;
  (void)t;
};

test tuple_basic = [] {
  tuple<int, double> t{42, 87.0};
  expect(42 == get_by_id<0>(t));
  expect(87.0 == get_by_id<1>(t));
};

test tuple_same_types = [] {
  tuple<int, int> t{42, 87};
  expect(42 == get_by_id<0>(t));
  expect(87 == get_by_id<1>(t));
};

test pool_empty = [] {
  pool<> p;
  static_expect(0 == get_size<decltype(p)>::value);
  expect(0 == try_get<int>(p));
};

test pool_basic = [] {
  pool<int, double> p{42, 87.0};
  static_expect(2 == get_size<decltype(p)>::value);
  expect(42 == get<int>(p));
  expect(87.0 == get<double>(p));
  expect(0.f == try_get<float>(p));
};

test pool_init_from_other_pool = [] {
  pool<double, int> p{87.0, 42};
  pool<int, double> p2{init{}, static_cast<decltype(p)&&>(p)};
  expect(42 == get<int>(p));
  expect(87.0 == get<double>(p));
  expect(0.f == try_get<float>(p));
};

test pool_is_pool = [] {
  static_expect(!is_pool<int>::value);
  static_expect(!is_pool<tuple<>>::value);
  static_expect(!is_pool<tuple<int>>::value);
  static_expect(is_pool<pool<>>::value);
  static_expect(is_pool<pool<int, double>>::value);
};

test type_id_empty = [] {
  using type_ids = type_id<>;
  static_expect(-1 == get_id<type_ids, -1, int>());
  static_expect(-1 == get_id<type_ids, -1, double>());
};

test type_id_basic = [] {
  using type_ids = type_id<int, double>;
  static_expect(-1 == get_id<type_ids, -1, float>());
  static_expect(0 == get_id<type_ids, -1, int>());
  static_expect(1 == get_id<type_ids, -1, double>());
};
}  // aux

namespace concepts {
test configurable_concept = [] {
  struct c0 {};
  struct c1 {
    auto configure() const noexcept { return aux::pool<>{}; }
  };
  struct c2 {
    auto configure() noexcept { return aux::pool<>{}; }
  };
  struct c3 {
    auto configure() { return aux::pool<>{}; }
  };
  struct c4 {
    int configure();
  };
  struct c5 {
    auto setup() { return aux::pool<>{}; }
  };

  static_expect(configurable<c1>::value);
  static_expect(configurable<c2>::value);
  static_expect(configurable<c3>::value);
  static_expect(!configurable<c0>::value);
  static_expect(!configurable<c4>::value);
  static_expect(!configurable<c5>::value);
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

test callable_concept = [] {
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
  auto calll1 = [] {};
  auto calll2 = [](int) {};
  auto calll3 = [] { return true; };

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

struct transition0 {};
struct transition1 {
  using src_state = void;
  using dst_state = void;
  using event = void;
  using deps = aux::pool<>;
  static constexpr auto has_initial = false;
};
struct transition2 {
  using src_state = void;
  using dst_state = void;
  // using event = void;
  using deps = aux::pool<>;
  static constexpr auto has_initial = false;
};
struct transition3 {
  using src_state = void;
  using dst_state = void;
  using event = void;
  using deps = aux::pool<>;
  static constexpr auto /*has_*/ initial = false;
};

test transitional_concept = [] {
  static_expect(!transitional<int>::value);
  static_expect(!transitional<transition0>::value);
  static_expect(transitional<transition1>::value);
  static_expect(!transitional<transition2>::value);
  static_expect(!transitional<transition3>::value);
};

struct runtime_event {};
struct event1 {
  static constexpr auto id = 0;
};
struct event2 {
  static constexpr auto id = 0;
  explicit event2(runtime_event) {}
};
struct event3 {
  // static constexpr auto id = 0;
};

test dispatchable_concept = [] {
  static_expect(!dispatchable<runtime_event, aux::type_list<event3, event1>>::value);
  static_expect(dispatchable<runtime_event, aux::type_list<>>::value);
  static_expect(dispatchable<runtime_event, aux::type_list<event1, event2>>::value);
  static_expect(dispatchable<runtime_event, aux::type_list<event2, event1>>::value);
};
}  // concepts
}  // v_1_0_0
}  // lite
}  // msm
}  // boost
