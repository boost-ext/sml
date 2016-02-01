//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/msm-lite.hpp"
#include <cassert>

namespace msm = boost::msm::lite;

struct e1 {};
struct e2 {
  bool value = true;
};
auto event2 = msm::event<e2>;

struct events {
  auto configure() const noexcept {
    using namespace msm;
    auto guard = [](const e2& e) { return e.value; };

    // clang-format off
    return make_transition_table(
        "idle"_s(initial) == "s1"_s + event<e1>
      , "s1"_s == "s2"_s + event2 [guard]
      , "s2"_s == "s3"_s + "e3"_t
      , "s3"_s == terminate + event<int> / [] (int i) { assert(42 == i); }
    );
    // clang-format on
  }
};

int main() {
  msm::sm<events> sm;
  using namespace msm;
  assert(sm.process_event(e1{}));
  assert(sm.process_event(e2{}));
  assert(sm.process_event("e3"_t));
  assert(sm.process_event(42));
  assert(sm.is(terminate));
}
