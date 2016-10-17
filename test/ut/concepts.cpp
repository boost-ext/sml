//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/msm-lite/back/concepts/callable.hpp"
#include "boost/msm-lite/back/concepts/configurable.hpp"
#include "boost/msm-lite/back/concepts/stringable.hpp"
#include "boost/msm-lite/back/concepts/transitional.hpp"

namespace concepts {

test configurable_concept = [] {
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
    int operator()();
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

}  // concepts
