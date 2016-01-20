//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "msm/msm.hpp"
#include <cassert>
#include <iostream>

struct e1 {};
struct e2 {};
struct e3 {};

auto guard = [] {
  std::cout << "guard" << std::endl;
  return true;
};

auto action = [] { std::cout << "action" << std::endl; };

auto idle = msm::state<class idle>{};
auto s1 = msm::state<class s1>{};
auto s2 = msm::state<class s2>{};

struct hello_world {
  auto configure() const noexcept {
    using namespace msm;
    // clang-format off
    return make_transition_table(
        idle(initial) == s1 + event<e1>
      , s1 == s2 + event<e2> [ guard ] / action
      , s2 == terminate + event<e3> / [] { std::cout << "in place action" << std::endl; }
    );
    // clang-format on
  }
};

int main() {
  msm::sm<hello_world> sm;
  assert(sm.is(idle));
  assert(sm.process_event(e1{}));
  assert(sm.is(s1));
  assert(sm.process_event(e2{}));
  assert(sm.is(s2));
  assert(sm.process_event(e3{}));
  assert(sm.is(msm::terminate));
}
