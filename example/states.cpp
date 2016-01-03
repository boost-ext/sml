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

struct events {
  auto configure() const noexcept {
    using namespace msm;

    // clang-format off
    return make_transition_table(
        "idle"_s(initial) == "s1"_s + event<e1>
      , "s1"_s == "s2"_s + event<e2>
      , "s2"_s == "s3"_s(terminate) + event<e3>
    );
    // clang-format on
  }
};

int main() {
  msm::sm<events> sm;
  sm.process_event(e1{});
  sm.process_event(e2{});
  sm.process_event(e3{});
  assert(sm.is(msm::terminate));
}
