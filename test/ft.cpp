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

auto idle = msm::state<class idle>{};
auto idle2 = msm::state<class idle2>{};
auto s1 = msm::state<class s1>{};
auto s2 = msm::state<class s2>{};
auto s3 = msm::state<class s3>{};
auto s4 = msm::state<class s4>{};
auto s5 = msm::state<class s5>{};
auto end = msm::state<class end>{};

template <class SM, class TState>
void expect_state(const SM &sm, const TState &, int r = 0) noexcept {
  auto result = false;
  auto i = 0;
  sm.visit_current_states([&](auto state) {
    if (r == i++) {
      result = msm::aux::is_same<decltype(state), TState>::value;
    }
  });
  expect(result);
}

template <class SM, class... TStates>
void expect_states(const SM &sm, const TStates &... states) noexcept {
  auto r = 0;
  int _[]{0, (expect_state(sm, states, r++), 0)...};
  (void)_;
}

test empty = [] {
  struct c {
    auto configure() noexcept { return msm::make_transition_table(); }
  };

  msm::sm<c> sm;
};

test ctor = [] {
  struct c {
    auto configure() noexcept { return msm::make_transition_table(); }
  };

  msm::sm<c> sm;
  msm::sm<c> sm_{static_cast<msm::sm<c> &&>(sm)};
  (void)sm_;
};

test minimal = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      return make_transition_table(idle(initial) + event<e1> / [] {});
    }
  };

  msm::sm<c> sm;
  expect_states(sm, idle(msm::initial));
  expect(sm.process_event(e1{}));
  expect_states(sm, idle(msm::initial));
};

test minimal_with_dependency = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      return make_transition_table(idle(initial) + event<e1> / [](int i) { expect(42 == i); });
    }
  };

  msm::sm<c> sm{42};
  expect_states(sm, idle(msm::initial));
  expect(sm.process_event(e1{}));
  expect_states(sm, idle(msm::initial));
};

test transition = [] {
  using namespace msm;
  struct c {
    auto configure() noexcept { return make_transition_table(idle(initial) == s1 + event<e1>); }
  };

  msm::sm<c> sm;
  expect_states(sm, idle(msm::initial));
  expect(sm.process_event(e1{}));
  expect_states(sm, s1);
};

test internal_transition = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e1>
        , s1 + event<e2> / [] {}
        , s1 == s2 + event<e3>
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  expect(sm.is(msm::initial));
  expect_states(sm, idle(msm::initial));
  expect(sm.process_event(e1{}));
  expect_states(sm, s1);
  expect(sm.process_event(e2{}));
  expect_states(sm, s1);
  expect(sm.process_event(e3{}));
  expect_states(sm, s2);
};

test anonymous_transition = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
        idle(initial) == s1 / [this] { a_called = true; }
      );
      // clang-format on
    }
    bool a_called = false;
  };

  c c_;
  msm::sm<c> sm{c_};
  expect_states(sm, s1);
  expect(c_.a_called);
};

test no_transition = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      return make_transition_table(idle(initial) == s1 + event<e1>);
    }
  };

  msm::sm<c> sm;
  expect_states(sm, idle(msm::initial));
  expect(!sm.process_event(e2{}));
  expect_states(sm, idle(msm::initial));
};

test transition_with_action_with_event = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      auto action = [this](const e1 &) { called = true; };
      return make_transition_table(idle(initial) == s1 + event<e1> / action);
    }

    bool called = false;
  };

  c c_;
  msm::sm<c> sm{c_};
  expect_states(sm, idle(msm::initial));
  expect(sm.process_event(e1{}));
  expect(c_.called);
  expect_states(sm, s1);
};

test transition_with_action_with_parameter = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      auto action = [this](int i) {
        called = true;
        expect(i == 42);
      };
      return make_transition_table(idle(initial) == s1 + event<e1> / action);
    }

    bool called = false;
  };

  c c_;
  msm::sm<c> sm{c_, 42};
  expect(sm.process_event(e1{}));
  expect(c_.called);
  expect_states(sm, s1);
};

test transition_with_action_and_guad_with_parameter = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;

      auto guard = [this](double d) {
        g_called = true;
        expect(d == 87.0);
        return true;
      };

      auto action = [this](int i) {
        a_called = true;
        expect(i == 42);
      };

      return make_transition_table(idle(initial) == s1 + event<e1>[guard] / action);
    }

    bool a_called = false;
    bool g_called = false;
  };

  c c_;
  msm::sm<c> sm{c_, 87.0, 42};
  expect(sm.process_event(e1{}));
  expect(c_.g_called);
  expect(c_.a_called);
  expect_states(sm, s1);
};

test transition_with_action_and_guad_with_parameters_and_event = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;

      auto guard = [this](int i, auto e, double d) {
        g_called = true;
        static_expect(msm::aux::is_same<decltype(e), e1>::value);
        expect(i == 42);
        expect(d == 87.0);
        return true;
      };

      auto action = [this](int i, float &f) {
        a_called = true;
        expect(i == 42);
        expect(f == 12.0);
      };

      return make_transition_table(idle(initial) == s1 + event<e1>[guard] / action);
    }

    bool a_called = false;
    bool g_called = false;
  };

  c c_;
  float f = 12.0;
  msm::sm<c> sm{c_, 42, 87.0, f};
  expect(sm.process_event(e1{}));
  expect(c_.g_called);
  expect(c_.a_called);
  expect_states(sm, s1);
};

test transitions = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      auto yes = [] { return true; };
      auto no = [] { return false; };

      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e1>
        , s1 == s2 + event<e2>
        , s2 == s3 + event<e3> [no]
        , s2 == s4 + event<e3> [yes]
      );
      // clang-format on
    }
  };

  c c_;
  msm::sm<c> sm{c_};
  expect(sm.process_event(e1{}));
  expect(sm.process_event(e2{}));
  expect(sm.process_event(e3{}));
  expect_states(sm, s4);
};

test no_transitions = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      auto yes = [] { return true; };
      auto no = [] { return false; };

      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e1>
        , s1 == s2 + event<e2> [no]
        , s1 == s2 + event<e2> [yes]
      );
      // clang-format on
    }
  };

  c c_;
  msm::sm<c> sm{c_};
  expect(sm.process_event(e1{}));
  expect(!sm.process_event(e3{}));
  expect(sm.process_event(e2{}));
  expect(!sm.process_event(e1{}));
  expect_states(sm, s2);
};

test transitions_states = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      auto yes = [] { return true; };
      auto no = [] { return false; };

      // clang-format off
      return make_transition_table(
          "idle"_s(initial) == "s1"_s + event<e1>
        , "s1"_s == "s2"_s + event<e2>
        , "s2"_s == "s3"_s + event<e3> [no]
        , "s2"_s == "s4"_s + event<e3> [yes]
      );
      // clang-format on
    }
  };

  c c_;
  msm::sm<c> sm{c_};
  expect(sm.process_event(e1{}));
  expect(sm.process_event(e2{}));
  expect(sm.process_event(e3{}));
  using namespace msm;
  expect_states(sm, "s4"_s);
};

test transition_overload = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e1>
        , s1 == s2 + event<int>
        , s1 == s3 + event<float>
      );
      // clang-format on
    }
  };

  {
    msm::sm<c> sm;
    expect(sm.process_event(e1{}));
    expect(sm.process_event(42));
    expect_states(sm, s2);
  }

  {
    msm::sm<c> sm;
    expect(sm.process_event(e1{}));
    expect(sm.process_event(42.f));
    expect_states(sm, s3);
  }
};

test initial_transition_overload = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e1>
        , idle(initial) == s2 + event<e2>
      );
      // clang-format on
    }
  };

  {
    msm::sm<c> sm;
    expect(sm.process_event(e1{}));
    expect_states(sm, s1);
  }

  {
    msm::sm<c> sm;
    expect(sm.process_event(e2{}));
    expect_states(sm, s2);
  }
};

void f_action(int, double) {}
auto f_guard(float &) { return true; }

struct c_guard {
  template <class T>
  bool operator()(const T &) const noexcept {
    return true;
  }
};

struct c_action {
  template <class T>
  void operator()(const T &) noexcept {}
};

test transition_types = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      struct {
      } flag;
      auto guard1 = [] { return true; };
      auto guard2 = [](auto) { return false; };
      auto guard3 = [=](int v) { return [=] { return guard2(v); }; };
      auto action1 = [] {};
      auto action2 = [](int, auto, float &) {};

      // clang-format off
      return make_transition_table(
        idle(initial) == s1 + event<e1> [ guard1 && f_guard ] / (action1, f_action)
        , s1 == s2 + event<e2> / ([] { })
        , s2 == s1 + event<e4> / process_event(e5{})
        , s1 / [] {}
        , s1 [guard1 && guard3(42)]
        , s1 [guard1] / action1
        , s1 + event<e3> / action1
        , idle == s1
        , s3 + event<e4> / [] { }
        , s3 + event<e5> [guard1] / action1
        , s1 == s2 [guard1 && guard1 && f_guard]
        , s3(initial) == s2 [guard1]
        , s2 == s3 [guard1 && !guard2]
        , s3 == s4 [guard1] / action1
        , s4 == s5 / action1
        , s5 == end / (action1, action2)
        , idle == s1 + event<e1>
        , idle == s1 + event<e1> [c_guard{} && guard1]
        , idle == s1 + event<e1> [guard1] / action1
        , idle == s1 + event<e1> / action1
        , idle == s1 + event<e1> / (action1, c_action{})
        , idle == s1(flag) + event<e1> [guard1 && guard2] / (action1, action2)
        , idle == s1 + event<e1> [guard1 && guard2] / (action1, action2, []{})
        , idle == s1 + event<e1> [guard1 || !guard2] / (action1, action2, []{}, [](auto){})
        , idle == s1 + event<e2> [guard1 || guard2] / (action1, action2, []{}, [](int, auto, float){})
        , idle == s1 + event<e1> [guard1 && guard2 && [] { return true; } ] / (action1, action2, []{}, [](int, auto, float){})
        , idle == s1 + event<e1> [guard1 && guard2 && [] { return true; } && [] (auto) { return false; } ] / (action1,
        action2, []{}, [](int, auto, float){})
      );
      // clang-format on
    }
  };

  c c_;
  float f = 12.0;
  msm::sm<c> sm{c_, f, 42, 87.0, 0.0f};
};

test flags = [] {
  struct flag {};
  struct c {
    auto configure() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e1>
        , s1 == s2(flag{}) + event<e2>
        , s2(flag{}) == s3 + event<e3>
      );
      // clang-format on
    }
  };

  c c_;
  msm::sm<c> sm{c_};
  expect(sm.is(msm::initial));
  expect(!sm.is(flag{}));
  expect_states(sm, idle(msm::initial));
  expect(sm.process_event(e1{}));
  expect_states(sm, s1);
  expect(sm.process_event(e2{}));
  expect_states(sm, s2(flag{}));
  expect(!sm.is(msm::initial));
  expect(sm.is(flag{}));
  expect(sm.process_event(e3{}));
  expect_states(sm, s3);
};

test orthogonal_regions = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e1>
        , s1 == s2 + event<e2>

        , idle2(initial) == s3 + event<e3>
        , s3 == s4 + event<e4>
      );
      // clang-format on
    }
  };

  c c_;
  msm::sm<c> sm{c_};
  expect_states(sm, idle(msm::initial), idle2(msm::initial));
  expect(sm.process_event(e1{}));
  expect_states(sm, s1, idle2(msm::initial));
  expect(sm.process_event(e2{}));
  expect_states(sm, s2, idle2(msm::initial));
  expect(sm.process_event(e3{}));
  expect_states(sm, s2, s3);
  expect(sm.process_event(e4{}));
  expect_states(sm, s2, s4);
};

test composite = [] {
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

  struct sub {
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e3> / [this] { a_in_sub++; }
        , s1 == s2 + event<e4> / [this] { a_in_sub++; }
      );
      // clang-format on
    }

    int a_in_sub = 0;
  };

  struct c {
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e1> [guard2{}] / [this] { a_initial = true; }
        , s1 == sub_ + event<e2> [guard]  / [this]{ a_enter_sub_sm = true; }
        , sub_ == s2 + event<e5> [guard2{}] / [this] { a_exit_sub_sm = true; }
      );
      // clang-format on
    }

    const msm::sm<sub> &sub_;

    bool a_initial = false;
    bool a_enter_sub_sm = false;
    bool a_exit_sub_sm = false;
  };

  sub sub_;
  msm::sm<sub> s{sub_};
  c c_{s};
  msm::sm<c> sm{c_, 87.0, 42};

  expect_states(sm, idle(msm::initial));
  sm.process_event(e1());
  expect(c_.a_initial);

  sm.process_event(e2());
  expect(c_.a_enter_sub_sm);
  expect(0 == sub_.a_in_sub);

  sm.process_event(e3());
  /*  expect(1 == sub_.a_in_sub);*/

  // sm.process_event(e4());
  // expect(2 == sub_.a_in_sub);

  // sm.process_event(e5());
  // expect(2 == sub_.a_in_sub);
  // expect(c_.a_exit_sub_sm);
  /*expect_states(sm, s2);*/
};

// test dispatcher = [] {};
