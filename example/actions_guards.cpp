//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <cassert>
#include <iostream>
#include <typeinfo>
#include "boost/msm-lite.hpp"

namespace msm = boost::msm::lite;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};

auto guard1 = [] {
  std::cout << "guard1" << std::endl;
  return true;
};

auto guard2 = [](int i) {
  assert(42 == i);
  std::cout << "guard2" << std::endl;
  return false;
};

auto action1 = [](auto e) { std::cout << "action1: " << typeid(e).name() << std::endl; };
struct action2 {
  void operator()(int i) {
    assert(42 == i);
    std::cout << "action2" << std::endl;
  }
};

struct actions_guards {
  auto operator()() const noexcept {
    using namespace msm;
    // clang-format off
    return make_transition_table(
       *"idle"_s + event<e1> = "s1"_s
      , "s1"_s + event<e2> [ guard1 ] / action1 = "s2"_s
      , "s2"_s + event<e3> [ guard1 && ![] { return false;} ] / (action1, action2{}) = "s3"_s
      , "s3"_s + event<e4> [ !guard1 || guard2 ] / (action1, [] { std::cout << "action3" << std::endl; }) = "s4"_s
      , "s3"_s + event<e4> [ guard1 ] / ([] { std::cout << "action4" << std::endl; }, [this] { action4(); }) = X
    );
    // clang-format on
  }

  void action4() const { std::cout << "action4" << std::endl; }
};

int main() {
  msm::sm<actions_guards> sm{42};
  sm.process_event(e1{});
  sm.process_event(e2{});
  sm.process_event(e3{});
  sm.process_event(e4{});
  assert(sm.is(msm::X));
}
