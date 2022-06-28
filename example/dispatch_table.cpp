//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/sml/utility/dispatch_table.hpp"

#include <boost/sml.hpp>
#include <cassert>

namespace sml = boost::sml;

namespace {
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
  auto operator()() noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
       *"idle"_s + event<event1> = "s1"_s
      , "s1"_s + event<event2> = X
    );
    // clang-format on
  }
};
}  // namespace

int main() {
  sml::sm<dispatch_table> sm;

  auto dispatch_event = sml::utility::make_dispatch_table<runtime_event, 1 /*min*/, 5 /*max*/>(sm);

  {
    runtime_event event{1};
    dispatch_event(event, event.id);
  }

  {
    runtime_event event{2};
    dispatch_event(event, event.id);
  }

  assert(sm.is(sml::X));
}
