//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "msm/msm.hpp"
#include <cassert>

struct e1 {};
struct e2 {
  bool value = true;
};
auto event2 = msm::event<e2>;
struct {
} terminate;

auto guard = [](const e2& e) { return e.value; };

struct events {
  auto configure() const noexcept {
    using namespace msm;
    state<class idle> idle;
    state<class s1> s1;
    state<class s2> s2;
    state<class s3> s3;

    // clang-format off
    return make_transition_table(
        idle(initial) == s1 + event<e1>
      , s1 == s2 + event2 [guard]
      , s2 == s3(terminate) + event<int> / [] (int i) { assert(42 == i); }
    );
    // clang-format on
  }
};

int main() {
  msm::sm<events> sm;
  assert(sm.process_event(e1{}));
  assert(sm.process_event(e2{}));
  assert(sm.process_event(42));
  assert(sm.is(terminate));
}
