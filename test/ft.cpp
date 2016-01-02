//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "msm/msm.hpp"

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};

test minimal = [] {
  struct c {
    auto configure() const noexcept {
      using namespace msm;
      state idle;

      return make_transition_table(idle + event<e1> / [] {});
    }
  };

  msm::sm<c> sm{c{}};
  sm.process_event(e1{});
};

test transition = [] {
  struct c {
    auto configure() const noexcept {
      using namespace msm;
      state idle, s1;
      return make_transition_table(idle(initial) == s1(terminate) + event<e1>);
    }
  };

  c c_;
  msm::sm<c> sm{c_};
  expect(sm.is(msm::initial));
  sm.process_event(e1{});
  expect(sm.is(msm::terminate));
};

test transition_with_action_with_event = [] {
  struct c {
    auto configure() const noexcept {
      using namespace msm;
      state idle, s1;
      auto action = [this](const e1&) { called = true; };
      return make_transition_table(idle(initial) == s1(terminate) + event<e1> / action);
    }

    mutable bool called = false;
  };

  c c_;
  msm::sm<c> sm{c_};
  sm.process_event(e1{});
  expect(c_.called);
  expect(sm.is(msm::terminate));
};

test transition_with_action_with_parameter = [] {
  struct c {
    auto configure() const noexcept {
      using namespace msm;
      state idle, s1;
      auto action = [this](int i) {
        called = true;
        expect(i == 42);
      };
      return make_transition_table(idle(initial) == s1(terminate) + event<e1> / action);
    }

    mutable bool called = false;
  };

  c c_;
  msm::sm<c> sm{c_, 42};
  sm.process_event(e1{});
  expect(c_.called);
  expect(sm.is(msm::terminate));
};

test transition_with_action_and_guad_with_parameter = [] {
  struct c {
    auto configure() const noexcept {
      using namespace msm;
      state idle, s1;

      auto guard = [this](double d) {
        g_called = true;
        expect(d == 87.0);
        return true;
      };

      auto action = [this](int i) {
        a_called = true;
        expect(i == 42);
      };

      return make_transition_table(idle(initial) == s1(terminate) + event<e1>[guard] / action);
    }

    mutable bool a_called = false;
    mutable bool g_called = false;
  };

  c c_;
  msm::sm<c> sm{c_, 87.0, 42};
  sm.process_event(e1{});
  expect(c_.g_called);
  expect(c_.a_called);
  expect(sm.is(msm::terminate));
};

test transition_with_action_and_guad_with_parameters_and_event = [] {
  struct c {
    auto configure() const noexcept {
      using namespace msm;
      state idle, s1;

      auto guard = [this](int i, auto e, double d) {
        g_called = true;
        static_expect(msm::aux::is_same<decltype(e), e1>::value);
        expect(i == 42);
        expect(d == 87.0);
        return true;
      };

      auto action = [this](int i, float& f) {
        a_called = true;
        expect(i == 42);
        expect(f == 12.0);
      };

      return make_transition_table(idle(initial) == s1(terminate) + event<e1>[guard] / action);
    }

    mutable bool a_called = false;
    mutable bool g_called = false;
  };

  c c_;
  float f = 12.0;
  msm::sm<c> sm{c_, 42, 87.0, f};
  sm.process_event(e1{});
  expect(c_.g_called);
  expect(c_.a_called);
  expect(sm.is(msm::terminate));
};

test sub_sm = [] {
  static auto guard = [](int i) {
    expect(42 == i);
    return true;
  };

  struct guard2 {
    bool operator()(double d) const {
      expect(87.0 == d);
      return true;
    }
  };

  static auto the_end = [] {};

  struct sub {
    auto configure() const noexcept {
      using namespace msm;
      state idle, s1, s2;

      // clang-format off
      return make_transition_table(
      // +-----------------------------------------------------------------+
        idle(initial) == s1 + event<e3> / [this] { a_in_sub++; }
      , s1 == s2 + event<e4> / [this] { a_in_sub++; }
      //+-----------------------------------------------------------------+
      );
      // clang-format on
    }

    mutable int a_in_sub = 0;
  };

  struct c {
    auto configure() const noexcept {
      using namespace msm;
      state idle, s1, s2;

      // clang-format off
      return make_transition_table(
        idle(initial) == s1 + event<e1> [guard2{}] / [this] { a_initial = true; }
      , s1 == sub_ + event<e2> [guard]  / [this]{ a_enter_sub_sm = true; }
      , sub_ == s2(the_end) + event<e5> [guard2{}] / [this] { a_exit_sub_sm = true; }
        );
      // clang-format on
    }

    const msm::sm<sub>& sub_;

    mutable bool a_initial = false;
    mutable bool a_enter_sub_sm = false;
    mutable bool a_exit_sub_sm = false;
  };

  sub sub_;
  msm::sm<sub> s{sub_};
  c c_{s};
  msm::sm<c> sm{c_, 87.0, 42};

  expect(sm.is(msm::initial));
  sm.process_event(e1());
  expect(c_.a_initial);

  sm.process_event(e2());
  expect(c_.a_enter_sub_sm);
  expect(0 == sub_.a_in_sub);

  sm.process_event(e3());
  expect(1 == sub_.a_in_sub);

  sm.process_event(e4());
  expect(2 == sub_.a_in_sub);

  sm.process_event(e5());
  expect(2 == sub_.a_in_sub);
  expect(c_.a_exit_sub_sm);
  expect(sm.is(the_end));
};
