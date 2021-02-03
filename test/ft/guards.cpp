//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>

namespace sml = boost::sml;

struct event1 {};

const auto state1 = sml::state<class state1>;
const auto state2 = sml::state<class state2>;
const auto state3 = sml::state<class state2>;

test guards_logic_should_short_circuit = [] {
  struct c {
    auto operator()() {
      using namespace sml;
      auto guard_true = [this] {
        ++guard_true_calls;
        return true;
      };
      auto guard_false = [this] {
        ++guard_false_calls;
        return false;
      };

      // clang-format off
        return make_transition_table(
         *state1 + event<event1> [ guard_true || guard_true ] = state2,
          state2 + event<event1> [ guard_false && guard_true ] = state3
        );
        // clang-format off
    }

    int guard_true_calls = 0;
    int guard_false_calls = 0;
  };

  sml::sm<c> sm{};

  sm.process_event(event1{});
  expect(0 == static_cast<const c&>(sm).guard_false_calls);
  expect(1 == static_cast<const c&>(sm).guard_true_calls);
  expect(sm.is(state2));

  sm.process_event(event1{});
  expect(1 == static_cast<const c&>(sm).guard_false_calls);
  expect(1 == static_cast<const c&>(sm).guard_true_calls);
  expect(sm.is(state2));
};
