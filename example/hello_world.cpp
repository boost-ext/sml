//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include "boost/sml.hpp"

namespace sml = boost::sml;

// events
struct close {};
struct ack {};
struct fin {};
struct timeout {};

int main() {
  using namespace sml;

  // guards
  const auto is_ack_valid = [](const ack&) { return true; };
  const auto is_fin_valid = [](const fin&) { return true; };

  // actions
  const auto send_fin = [] {};
  const auto send_ack = [] {};

  auto sm = make_sm([&] {
    // clang-format off
    return make_transition_table(
      *("established"_s) + event<close> / send_fin = "fin wait 1"_s,
        "fin wait 1"_s   + event<ack> [ is_ack_valid ] = "fin wait 2"_s,
        "fin wait 2"_s   + event<fin> [ is_fin_valid ] / send_ack = "timed wait"_s,
        "timed wait"_s   + event<timeout> / send_ack = X
    );
    // clang-format on
  });

  static_assert(1 == sizeof(sm), "sizeof(sm) != 1b");
  assert(sm.is("established"_s));

  sm.process_event(close{});
  assert(sm.is("fin wait 1"_s));

  sm.process_event(ack{});
  assert(sm.is("fin wait 2"_s));

  sm.process_event(fin{});
  assert(sm.is("timed wait"_s));

  sm.process_event(timeout{});
  assert(sm.is(X));  /// closed
}
