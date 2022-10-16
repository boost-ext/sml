//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/sml.hpp"

BOOST_SML_NAMESPACE_BEGIN
namespace concepts {

struct c0 {};
struct c1 {
  auto operator()() const noexcept { return aux::pool<>{}; }
};
struct c2 {
  auto operator()() noexcept { return aux::pool<>{}; }
};
struct c3 {
  auto operator()() { return aux::pool<>{}; }
};
struct c4 {
  int operator()() { return {}; }
};
struct c5 {
  auto setup() { return aux::pool<>{}; }
};

test composable_concept = [] {
  static_expect(composable<c1>::value);
  static_expect(composable<c2>::value);
  static_expect(composable<c3>::value);
  static_expect(composable<c3>::value);
  static_expect(composable<c3>::value);
  static_expect(!composable<c0>::value);
  static_expect(!composable<c4>::value);
  static_expect(!composable<c5>::value);
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
    void operator()() {}
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

#if defined(__cpp_noexcept_function_type)
  struct class0 {
    void action() {}
    void const_action() const {}
    void const_noexcept_action() const noexcept {}

    bool guard() { return true; }
    bool const_guard() const { return true; }
    bool const_noexcept_guard() const noexcept { return true; }
  };
  static_expect(callable<void, decltype(&class0::action)>::value);
  static_expect(callable<void, decltype(&class0::const_action)>::value);
  static_expect(callable<void, decltype(&class0::const_noexcept_action)>::value);
  static_expect(callable<bool, decltype(&class0::guard)>::value);
  static_expect(callable<bool, decltype(&class0::const_guard)>::value);
  static_expect(callable<bool, decltype(&class0::const_noexcept_guard)>::value);
#endif

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
  static constexpr auto initial = false;
  static constexpr auto history = false;
};
struct transition2 {
  using src_state = void;
  using dst_state = void;
  // using event = void;
  using deps = aux::pool<>;
  static constexpr auto initial = false;
  static constexpr auto history = false;
};
struct transition3 {
  using src_state = void;
  using dst_state = void;
  using event = void;
  using deps = aux::pool<>;
  static constexpr auto /*initial*/ INITIAL = false;
  static constexpr auto history = false;
};

test transitional_concept = [] {
  static_expect(!transitional<int>::value);
  static_expect(!transitional<transition0>::value);
  static_expect(transitional<transition1>::value);
  static_expect(!transitional<transition2>::value);
  static_expect(!transitional<transition3>::value);
};

}  // namespace concepts
BOOST_SML_NAMESPACE_END
