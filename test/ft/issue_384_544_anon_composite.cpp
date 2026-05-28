//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Issue #384: Anonymous transition in nested composite state not taken at
//   construction — sub-SM's anonymous A→B transition not fired on sm init.
// Issue #544: Same defect when the composite is entered via a runtime event —
//   auto-transitions inside the sub-SM are not triggered after entry.
//
#include <boost/sml.hpp>

namespace sml = boost::sml;

struct e_exit384 {};
struct e_enter544 {};

// --- #384 state types at file scope (shared between SM def and test) ---------
struct state_A384 {};
struct state_B384 {};

struct sub384 {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
        *state<state_A384> = state<state_B384>   // anonymous A→B
      , state<state_B384> + event<e_exit384> = X
    );
    // clang-format on
  }
};

struct sm_384 {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
        *state<sub384> + event<e_exit384> = X
    );
    // clang-format on
  }
};

// --- #544 state types at file scope ------------------------------------------
struct state_Idle544 {};
struct state_Initial544 {};
struct state_Middle544 {};
struct state_Final544 {};

struct sub544 {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
        *state<state_Initial544> = state<state_Middle544>   // anon
      , state<state_Middle544>   = state<state_Final544>    // anon
    );
    // clang-format on
  }
};

struct sm_544 {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
        *state<state_Idle544> + event<e_enter544> = state<sub544>
    );
    // clang-format on
  }
};

// -----------------------------------------------------------------------------

test anon_transition_after_nested_entry_384 = [] {
  // After construction the composite sub-SM must have taken the anonymous
  // A→B transition, so the sub-SM is in state B (not A).
  sml::sm<sm_384> sm;
  expect(sm.is(sml::state<sub384>));
  expect(sm.is<decltype(sml::state<sub384>)>(sml::state<state_B384>));
};

test anon_transition_after_runtime_entry_544 = [] {
  // After e_enter transitions outer SM into sub544, all anonymous transitions
  // inside must fire: Initial→Middle→Final.
  sml::sm<sm_544> sm;
  expect(sm.is(sml::state<state_Idle544>));

  sm.process_event(e_enter544{});

  expect(sm.is(sml::state<sub544>));
  expect(sm.is<decltype(sml::state<sub544>)>(sml::state<state_Final544>));
};
