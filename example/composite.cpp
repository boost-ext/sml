//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/msm-lite.hpp"
#include <iostream>
#include <cassert>

namespace msm = boost::msm::lite;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};

struct sub {
  sub() {}
  auto configure() const noexcept {
    using namespace msm;
    // clang-format off
      return make_transition_table(
       *"idle"_s + event<e3> / [] { std::cout << "in sub sm" << std::endl; } = "s1"_s
      , "s1"_s + event<e4> / [] { std::cout << "finish sub sm" << std::endl; } = X
      );
    // clang-format on
  }
};

msm::state<msm::sm<sub>> sub_state;

struct composite {
  composite() {}
  auto configure() const noexcept {
    using namespace msm;
    // clang-format off
    return make_transition_table(
     *"idle"_s + event<e1> = "s1"_s
    , "s1"_s + event<e2> / [] { std::cout << "enter sub sm" << std::endl; } = sub_state
    , sub_state + event<e5> / [] { std::cout << "exit sub sm" << std::endl; } = X
    );
    // clang-format on
  }
};

int main() {
  sub sub_;
  msm::sm<sub> sub_sm{sub_};
  composite composite_;
  msm::sm<composite> sm{composite_, sub_sm};

  using namespace msm;
  assert(sm.is("idle"_s));
  assert(sub_sm.is("idle"_s));

  assert(sm.process_event(e1{}));
  assert(sm.is("s1"_s));
  assert(sub_sm.is("idle"_s));

  assert(sm.process_event(e2{}));  // enter sub sm
  assert(sm.is(sub_state));
  assert(sub_sm.is("idle"_s));

  assert(sm.process_event(e3{}));  // in sub sm
  assert(sm.is(sub_state));
  assert(sub_sm.is("s1"_s));

  assert(sm.process_event(e4{}));  // finish sub sm
  assert(sm.is(sub_state));
  assert(sub_sm.is(X));

  assert(sm.process_event(e5{}));  // exit sub sm
  assert(sm.is(X));
  assert(sub_sm.is(X));
}
