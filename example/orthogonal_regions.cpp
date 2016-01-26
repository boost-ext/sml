//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/msm.hpp"
#include <cassert>

namespace msm = boost::msm;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};

struct orthogonal_regions {
  auto configure() const noexcept {
    using namespace msm;
    state<class idle> idle;
    state<class idle2> idle2;
    state<class s1> s1;
    state<class s2> s2;

    // clang-format off
    return make_transition_table(
      idle(initial) == s1 + event<e1>
    , s1 == terminate + event<e2>

    , idle2(initial) == s2 + event<e3>
    , s2 == terminate + event<e4>
    );
    // clang-format on
  }
};

int main() {
  msm::sm<orthogonal_regions> sm;
  assert(sm.process_event(e1{}));
  assert(sm.process_event(e2{}));
  assert(sm.process_event(e3{}));
  assert(sm.process_event(e4{}));
  assert(sm.is(msm::terminate));
}
