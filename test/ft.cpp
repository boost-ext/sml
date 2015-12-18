//
// Copyright (c) 2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "msm/msm.hpp"

#include <iostream>

struct e1 {};

auto expect_state = [](auto& sm, const auto& state) {
  sm.visit_current_states([&](const auto& s) { expect(static_cast<const msm::state_base*>(&s) == state); });
};

test empty = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      init_state<> idle;

      return make_transition_table(idle + event<e1> / [] {});
    }
  };

  msm::sm<c> sm{c{}};
  sm.process_event(e1{});
};

test transition = [] {
  struct c {
    msm::init_state<> idle;
    msm::state<> s1;

    auto configure() noexcept {
      using namespace msm;
      return make_transition_table(idle == s1 + event<e1>);
    }
  };

  c c_;
  msm::sm<c> sm{c_};
  expect_state(sm, &sm.fsm_.idle);
  sm.process_event(e1{});
  expect_state(sm, &sm.fsm_.s1);
};

test action_with_event = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      init_state<> idle;
      state<> s1;
      auto action = [](const e1&) {};

      return make_transition_table(idle == s1 + event<e1> / action);
    }
  };

  msm::sm<c> sm{c{}};
  sm.process_event(e1{});
};
