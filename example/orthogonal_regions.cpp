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
struct e4 {};
struct {
} terminate;

struct orthogonal_regions {
  auto configure() const noexcept {
    using namespace msm;
    state<class idle> idle;
    state<class idle2> idle2;
    state<class s1> s1;
    state<class s2> s2;
    state<class s3> s3;
    state<class s4> s4;

    // clang-format off
    return make_transition_table(
      idle(initial) == s1 + event<e1>
    , s1 == s2(terminate) + event<e2>

    , idle2(initial) == s3 + event<e3>
    , s3 == s4(terminate) + event<e4>
    );
    // clang-format on
  }
};

int main() {
  msm::sm<orthogonal_regions> sm;
  assert(sm.is(msm::initial));
  sm.process_event(e1{});
  sm.process_event(e2{});
  assert(sm.is(terminate));
  assert(sm.is(msm::initial));
  sm.process_event(e3{});
  sm.process_event(e4{});
  assert(sm.is(terminate));
  assert(!sm.is(msm::initial));
}
