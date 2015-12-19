//
// Copyright (c) 2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "msm/msm.hpp"

struct e1 {};
struct e2 {};

auto expect_state = [](auto& sm, const auto& state) {
  sm.visit_current_states([&](const auto& s) { expect(static_cast<const msm::state_base*>(&s) == &state); });
};

test empty = [] {
  struct c {
    auto configure() const noexcept {
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

    auto configure() const noexcept {
      using namespace msm;
      return make_transition_table(idle == s1 + event<e1>);
    }
  };

  c c_;
  msm::sm<c> sm{c_};
  expect_state(sm, c_.idle);
  sm.process_event(e1{});
  expect_state(sm, c_.s1);
};

test action_with_event = [] {
  struct c {
    auto configure() const noexcept {
      using namespace msm;
      auto action = [](const e1&) {};
      return make_transition_table(idle == s1 + event<e1> / action);
    }

    msm::init_state<> idle;
    msm::state<> s1;
  };

  c c_;
  msm::sm<c> sm{c_};
  expect_state(sm, c_.idle);
  sm.process_event(e1{});
  expect_state(sm, c_.s1);
};

test action_with_parameter = [] {
  struct c {
    auto configure() const noexcept {
      using namespace msm;
      auto action = [](int i) { expect(i == 42); };
      return make_transition_table(idle == s1 + event<e1> / action);
    }

    msm::init_state<> idle;
    msm::state<> s1;
  };

  c c_;
  msm::sm<c> sm{c_, 42};
  expect_state(sm, c_.idle);
  sm.process_event(e1{});
  expect_state(sm, c_.s1);
};

test action_and_guad_with_parameter = [] {
  struct c {
    auto configure() const noexcept {
      using namespace msm;
      auto guard = [](double d) {
        expect(d == 87.0);
        return true;
      };
      auto action = [](int i) { expect(i == 42); };
      return make_transition_table(idle == s1 + event<e1>[guard] / action);
    }

    msm::init_state<> idle;
    msm::state<> s1;
  };

  c c_;
  msm::sm<c> sm{c_, 87.0, 42};
  expect_state(sm, c_.idle);
  sm.process_event(e1{});
  expect_state(sm, c_.s1);
};

test action_and_guad_with_parameters_and_event = [] {
  struct c {
    auto configure() const noexcept {
      using namespace msm;
      auto guard = [](int i, auto e, double d) {
        static_expect(msm::aux::is_same<decltype(e), e1>::value);
        expect(i == 42);
        expect(d == 87.0);
        return true;
      };
      auto action = [](int i, float& f) {
        expect(i == 42);
        expect(f == 12.0);
      };
      return make_transition_table(idle == s1 + event<e1>[guard] / action);
    }

    msm::init_state<> idle;
    msm::state<> s1;
  };

  c c_;
  float f = 12.0;
  msm::sm<c> sm{c_, 42, 87.0, f};
  expect_state(sm, c_.idle);
  sm.process_event(e1{});
  expect_state(sm, c_.s1);
};
