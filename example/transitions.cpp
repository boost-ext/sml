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
struct { } terminate;

struct transitions {
  auto configure() const noexcept {
    using namespace msm;
    state<class idle> idle;
    state<class s1> s1;
    state<class s2> s2;

    // clang-format off
    return make_transition_table(
        idle(initial) == s1  / [] { std::cout << "anonymous transition" << std::endl; }
      , s1 + event<e1> / [] { std::cout << "internal transition" << std::endl; }
      , s1 == s2(terminate) + event<e2>
    );
    // clang-format on
  }
};

int main() {
  msm::sm<transitions> sm{transitions{}};
  assert(!sm.is(msm::initial));
  sm.process_event(e1{});
  sm.process_event(e2{});
  assert(sm.is(terminate));
}
