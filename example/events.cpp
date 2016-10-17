//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include "boost/msm-lite.hpp"

namespace msm = boost::msm::lite;

struct e1 {};
struct e2 {
  bool value = true;
};
auto event2 = msm::event<e2>;

struct events {
  auto operator()() const noexcept {
    using namespace msm;
    auto guard = [](const e2& e) { return e.value; };

    // clang-format off
    return make_transition_table(
       *"idle"_s + event<e1> = "s1"_s
      , "s1"_s + event2 [guard] = "s2"_s
      , "s2"_s + "e3"_e = "s3"_s
      , "s3"_s + event<int> / [] (int i) { assert(42 == i); } = X
    );
    // clang-format on
  }
};

int main() {
  msm::sm<events> sm;
  using namespace msm;
  sm.process_event(e1{});
  sm.process_event(e2{});
  sm.process_event("e3"_e);
  sm.process_event(42);
  assert(sm.is(X));
}
