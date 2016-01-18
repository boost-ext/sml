//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "msm/msm.hpp"
#include <iostream>
#include <cassert>

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};

struct sub {
  sub() {}
  auto configure() const noexcept {
    using namespace msm;
    state<class idle> idle;
    state<class s1> s1;

    // clang-format off
      return make_transition_table(
        idle(initial) == s1 + event<e3> / [] { std::cout << "in sub sm" << std::endl; }
      , s1 == terminate + event<e4> / [] { std::cout << "finish sub sm" << std::endl; }
      );
    // clang-format on
  }
};

struct composite {
  composite() {}
  auto configure() const noexcept {
    using namespace msm;
    state<class idle> idle;
    state<class s1> s1;
    state<sm<sub>> sub_;

    // clang-format off
      return make_transition_table(
        idle(initial) == s1 + event<e1>
      , s1 == sub_ + event<e2> / [] { std::cout << "enter sub sm" << std::endl; }
      , sub_ == terminate + event<e5> / [] { std::cout << "exit sub sm" << std::endl; }
        );
    // clang-format on
  }
};

int main() {
  sub sub_;
  msm::sm<sub> sub_sm{sub_};
  composite composite_;
  msm::sm<composite> sm{composite_, sub_sm};

  assert(sm.process_event(e1{}));
  assert(sm.process_event(e2{}));  // enter sub sm
  assert(sm.process_event(e3{}));  // in sub sm
  assert(sm.process_event(e4{}));  // finish sub sm
  assert(sm.process_event(e5{}));  // exit sub sm
  assert(sm.is(msm::terminate));
}
