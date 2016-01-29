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

struct runtime_event {
  int id = 0;
};
struct event1 {
  static constexpr auto id = 1;
  event1(const runtime_event &) {}
};
struct event2 {
  static constexpr auto id = 2;
};

struct dispatch_table {
  auto configure() noexcept {
    using namespace msm;
    // clang-format off
    return make_transition_table(
        "idle"_s(initial) == "s1"_s + event<event1>
      , "s1"_s == terminate + event<event2>
    );
    // clang-format on
  }
};

int main() {
  msm::sm<dispatch_table> sm;

  auto dispatch_event = msm::make_dispatch_table<runtime_event, 1 /*min*/, 5 /*max*/>(sm);

  {
    runtime_event event{1};
    assert(dispatch_event(event, event.id));
  }

  {
    runtime_event event{2};
    assert(dispatch_event(event, event.id));
  }

  assert(sm.is(msm::terminate));
}
