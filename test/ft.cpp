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

test empty = [] {
  struct c {
    auto configure() const noexcept {
      using namespace msm;
      return make_transition_table();
    }
  };

  msm::sm<c> sm;
};

test minimal = [] {
  struct c {
    auto configure() const noexcept {
      using namespace msm;
      state idle;
      return make_transition_table(idle + event<e1> / [] {});
    }
  };

  msm::sm<c> sm{c{}};
  expect(sm.process_event(e1{}));
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
  expect(sm.process_event(e1{}));
  expect(sm.is(msm::terminate));
};

test no_transition = [] {
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
  expect(!sm.process_event(e2{}));
  expect(sm.is(msm::initial));
};

test transition_with_action_with_event = [] {
  struct c {
    auto configure() const noexcept {
      using namespace msm;
      state idle, s1;
      auto action = [this](const e1 &) { called = true; };
      return make_transition_table(idle(initial) == s1(terminate) + event<e1> / action);
    }

    mutable bool called = false;
  };

  c c_;
  msm::sm<c> sm{c_};
  expect(sm.process_event(e1{}));
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
  expect(sm.process_event(e1{}));
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
  expect(sm.process_event(e1{}));
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

      auto action = [this](int i, float &f) {
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
  expect(sm.process_event(e1{}));
  expect(c_.g_called);
  expect(c_.a_called);
  expect(sm.is(msm::terminate));
};

test transitions = [] {
  struct c {
    auto configure() const noexcept {
      using namespace msm;
      state idle, s1, s2, s3, s4;
      auto yes = [] { return true; };
      auto no = [] { return false; };

      // clang-format off
      return make_transition_table(
        idle(initial) == s1 + event<e1>
      , s1 == s2 + event<e2>
      , s2 == s3 + event<e3> [no]
      , s2 == s4(terminate) + event<e3> [yes]
      );
      // clang-format on
    }
  };

  c c_;
  msm::sm<c> sm{c_};
  expect(sm.process_event(e1{}));
  expect(sm.process_event(e2{}));
  expect(sm.process_event(e3{}));
  expect(sm.is(msm::terminate));
};

test no_transitions = [] {
  struct c {
    auto configure() const noexcept {
      using namespace msm;
      state idle, s1, s2;
      auto yes = [] { return true; };
      auto no = [] { return false; };

      // clang-format off
      return make_transition_table(
        idle(initial) == s1 + event<e1>
      , s1 == s2(terminate) + event<e2> [no]
      , s1 == s2(terminate) + event<e2> [yes]
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
  expect(sm.is(msm::terminate));
};

test transitions_states = [] {
  struct c {
    auto configure() const noexcept {
      using namespace msm;
      auto yes = [] { return true; };
      auto no = [] { return false; };

      // clang-format off
      return make_transition_table(
        "idle"_s(initial) == "s1"_s + event<e1>
      , "s1"_s == "s2"_s + event<e2>
      , "s2"_s == "s3"_s + event<e3> [no]
      , "s2"_s == "s4"_s(terminate) + event<e3> [yes]
      );
      // clang-format on
    }
  };

  c c_;
  msm::sm<c> sm{c_};
  expect(sm.process_event(e1{}));
  expect(sm.process_event(e2{}));
  expect(sm.process_event(e3{}));
  expect(sm.is(msm::terminate));
};

test transition_overload = [] {
  static auto int_ = [] {};
  static auto float_ = [] {};

  struct c {
    auto configure() const noexcept {
      using namespace msm;
      state idle, s1, s2;

      // clang-format off
      return make_transition_table(
        idle(initial) == s1 + event<e1>
      , s1 == s2(int_) + event<int>
      , s1 == s2(float_) + event<float>
      );
      // clang-format on
    }
  };

  {
    msm::sm<c> sm;
    expect(sm.process_event(e1{}));
    expect(sm.process_event(42));
    expect(sm.is(int_));
  }

  {
    msm::sm<c> sm;
    expect(sm.process_event(e1{}));
    expect(sm.process_event(42.f));
    expect(sm.is(float_));
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
    auto configure() const noexcept {
      using namespace msm;
      state idle, s1, s2, s3, s4, s5, end;
      auto flag = [] {};

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
         , s1 + event<not_handled> / action1
         , idle == s1
         , s3 + event<not_handled> / [] { }
         , s3 + event<not_handled> [guard1] / action1
         , s1 == s2 [guard1 && guard1 && f_guard]
         , s3(initial) == s2 [guard1]
         , s2 == s3 [guard1 && !guard2]
         , s3 == s4 [guard1] / action1
         , s4 == s5 / action1
         , s5 == end(terminate) / (action1, action2)
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
         , idle == s1 + event<e1> [guard1 && guard2 && [] { return true; } && [] (auto) { return false; } ] / (action1, action2, []{}, [](int, auto, float){})
          // clang-format on
          );
    }
  };

  c c_;
  float f = 12.0;
  msm::sm<c> sm{c_, f, 42, 87.0, 0.0};
};

test orthogonal_regions = [] {
  struct c {
    auto configure() const noexcept {
      using namespace msm;
      state idle, idle2, s1, s2, s3, s4;

      // clang-format off
      return make_transition_table(
        idle(initial) == s1 + event<e1>
      , s1 == s2(terminate) + event<e2>

      , idle2(initial) == s3 + event<e3>
      , s3 == s4(terminate) + event<e4>
      );
      // clang-format on
    }
  };

  c c_;
  msm::sm<c> sm{c_};
  expect(sm.is(msm::initial, true, true));
  expect(sm.process_event(e1{}));
  expect(sm.is(msm::initial, false, true));
  expect(sm.is(msm::terminate, false, false));
  expect(sm.process_event(e2{}));
  expect(sm.is(msm::initial, false, true));
  expect(sm.is(msm::terminate, true, false));
  expect(sm.process_event(e3{}));
  expect(sm.is(msm::initial, false, false));
  expect(sm.is(msm::terminate, true, false));
  expect(sm.process_event(e4{}));
  expect(sm.is(msm::initial, false, false));
  expect(sm.is(msm::terminate, true, true));
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

  static auto the_end = [] {};

  struct sub {
    auto configure() const noexcept {
      using namespace msm;
      state idle, s1, s2;

      // clang-format off
      return make_transition_table(
        idle(initial) == s1 + event<e3> / [this] { a_in_sub++; }
      , s1 == s2 + event<e4> / [this] { a_in_sub++; }
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

    const msm::sm<sub> &sub_;

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
