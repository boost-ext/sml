//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "msm/msm.hpp"
#include <cassert>
#include <typeinfo>
#include <iostream>

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct {
} terminate;

auto guard1 = [] {
  std::cout << "guard1" << std::endl;
  return true;
};

auto guard2(int i) {
  assert(42 == i);
  std::cout << "guard2" << std::endl;
  return false;
}

auto action1 = [](auto e) { std::cout << "action1: " << typeid(e).name() << std::endl; };
struct action2 {
  void operator()(int i) {
    assert(42 == i);
    std::cout << "action2" << std::endl;
  }
};

struct action_guards {
  auto configure() const noexcept {
    using namespace msm;
    state<class idle> idle;
    state<class s1> s1;
    state<class s2> s2;
    state<class s3> s3;
    state<class s4> s4;

    // clang-format off
    return make_transition_table(
        idle(initial) == s1 + event<e1>
      , s1 == s2 + event<e2> [ guard1 ] / action1
      , s2 == s3 + event<e3> [ guard1 && ![] { return false;} ] / (action1, action2{})
      , s3 == s4 + event<e4> [ !guard1 || guard2 ] / (action1, [] { std::cout << "action3" << std::endl; })
      , s3 == s4(terminate) + event<e4> [ guard1 ] / ([] { std::cout << "action4" << std::endl; })
    );
    // clang-format on
  }
};

int main() {
  msm::sm<action_guards> sm{action_guards{}, 42};
  sm.process_event(e1{});
  sm.process_event(e2{});
  sm.process_event(e3{});
  sm.process_event(e4{});
  assert(sm.is(terminate));
}
