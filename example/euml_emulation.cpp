//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include "boost/msm-lite.hpp"

namespace msm = boost::msm::lite;

struct e1 {};
struct e2 {};
struct e3 {};

auto event1 = msm::event<e1>;
auto event2 = msm::event<e2>;
auto event3 = msm::event<e3>;

auto idle = msm::state<class idle>;
auto s1 = msm::state<class s1>;
auto s2 = msm::state<class s2>;

struct data {
  void set(int value) noexcept { i = value; }
  int get() const noexcept { return i; }
  int i = 0;
};

struct {
  template <class TEvent>
  bool operator()(const TEvent&, data& d) const noexcept {
    return d.get() == 42;
  }
} guard;

struct {
  template <class TEvent>
  void operator()(const TEvent&, data& d) noexcept {
    d.set(123);
  }
} action;

class euml_emulation {
 public:
  auto operator()() const noexcept {
    using namespace msm;
    // clang-format off
    return make_transition_table(
      s1 <= *idle + event1,
      s2 <= s1    + event2 [ guard ],
      X  <= s2    + event3 [ guard ] / action
    );
    // clang-format on
  }
};

int main() {
  data d{42};
  msm::sm<euml_emulation> sm{d};
  assert(sm.is(idle));
  sm.process_event(e1{});
  assert(sm.is(s1));
  sm.process_event(e2{});
  assert(sm.is(s2));
  sm.process_event(e3{});
  assert(sm.is(msm::X));
  assert(123 == d.get());
}
