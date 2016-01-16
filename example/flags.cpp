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
struct e2 {};
struct e3 {};

struct {
} terminate;
struct {
} custom_flag;

struct flags {
  auto configure() const noexcept {
    using namespace msm;
    state<class idle> idle;
    state<class s1> s1;
    state<class s2> s2;
    state<class s3> s3;

    // clang-format off
    return make_transition_table(
        idle(initial) == s1 + event<e1>
      , s1 == s2(custom_flag) + event<e2>
      , s2(custom_flag) == s3(terminate) + event<e3>
    );
    // clang-format on
  }
};

int main() {
  msm::sm<flags> sm{flags{}};
  assert(sm.is(msm::initial));

  sm.process_event(e1{});
  assert(!sm.is(msm::initial));

  sm.process_event(e2{});
  assert(sm.is(custom_flag));

  assert(sm.process_event(e3{}));
  assert(sm.is(terminate));
}
