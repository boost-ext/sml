//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <cassert>

namespace sml = boost::sml;

namespace {
// events
struct release {};
struct ack {};
struct fin {};
struct timeout {};

// guards
const auto is_ack_valid = [](const ack&) { return true; };
const auto is_fin_valid = [](const fin&) { return true; };

// actions
const auto send_fin = [] {};
const auto send_ack = [] {};

#if !defined(_MSC_VER)
struct hello_world {
  auto operator()() const {
    using namespace sml;
    // clang-format off
    return make_transition_table(
      *"established"_s + event<release> / send_fin = "fin wait 1"_s,
       "fin wait 1"_s + event<ack> [ is_ack_valid ] = "fin wait 2"_s,
       "fin wait 2"_s + event<fin> [ is_fin_valid ] / send_ack = "timed wait"_s,
       "timed wait"_s + event<timeout> / send_ack = X
    );
    // clang-format on
  }
};
}

int main() {
  using namespace sml;

  sm<hello_world> sm;
  static_assert(1 == sizeof(sm), "sizeof(sm) != 1b");
  assert(sm.is("established"_s));

  sm.process_event(release{});
  assert(sm.is("fin wait 1"_s));

  sm.process_event(ack{});
  assert(sm.is("fin wait 2"_s));

  sm.process_event(fin{});
  assert(sm.is("timed wait"_s));

  sm.process_event(timeout{});
  assert(sm.is(X));  // released
}
#else
class established;
class fin_wait_1;
class fin_wait_2;
class timed_wait;

struct hello_world {
  auto operator()() const {
    using namespace sml;
    // clang-format off
    return make_transition_table(
      *state<established> + event<release> / send_fin = state<fin_wait_1>,
       state<fin_wait_1> + event<ack> [ is_ack_valid ] = state<fin_wait_2>,
       state<fin_wait_2> + event<fin> [ is_fin_valid ] / send_ack = state<timed_wait>,
       state<timed_wait> + event<timeout> / send_ack = X
    );
    // clang-format on
  }
};
}

int main() {
  using namespace sml;

  sm<hello_world> sm;
  assert(sm.is(state<established>));

  sm.process_event(release{});
  assert(sm.is(state<fin_wait_1>));

  sm.process_event(ack{});
  assert(sm.is(state<fin_wait_2>));

  sm.process_event(fin{});
  assert(sm.is(state<timed_wait>));

  sm.process_event(timeout{});
  assert(sm.is(X));  // released
}
#endif
