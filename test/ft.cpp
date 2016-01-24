//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/msm/msm.hpp"
#include <string>

namespace msm = boost::msm;

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
  expect(sm.is(idle));
  expect(sm.process_event(e1{}));
  expect(sm.is(idle));
};

test minimal_with_dependency = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      return make_transition_table(idle(initial) + event<e1> / [](int i) { expect(42 == i); });
    }
  };

  msm::sm<c> sm{42};
  expect(sm.is(idle));
  expect(sm.process_event(e1{}));
  expect(sm.is(idle));
};

test transition = [] {
  using namespace msm;
  struct c {
    auto configure() noexcept { return make_transition_table(idle(initial) == s1 + event<e1>); }
  };

  msm::sm<c> sm;
  expect(sm.is(idle));
  expect(sm.process_event(e1{}));
  expect(sm.is(s1));
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
  expect(sm.is(idle));
  expect(sm.process_event(e1{}));
  expect(sm.is(s1));
  expect(sm.process_event(e2{}));
  expect(sm.is(s1));
  expect(sm.process_event(e3{}));
  expect(sm.is(s2));
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
  expect(sm.is(s1));
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
  expect(sm.is(idle));
  expect(!sm.process_event(e2{}));
  expect(sm.is(idle));
  expect(!sm.process_event(int{}));
  expect(sm.is(idle));
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
  expect(sm.is(idle));
  expect(sm.process_event(e1{}));
  expect(c_.called);
  expect(sm.is(s1));
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
  expect(sm.is(s1));
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
  expect(sm.is(s1));
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
        expect(f == 12.f);
      };

      return make_transition_table(idle(initial) == s1 + event<e1>[guard] / action);
    }

    bool a_called = false;
    bool g_called = false;
  };

  c c_;
  auto f = 12.f;
  msm::sm<c> sm{c_, 42, 87.0, f};
  expect(sm.process_event(e1{}));
  expect(c_.g_called);
  expect(c_.a_called);
  expect(sm.is(s1));
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

  msm::sm<c> sm;
  expect(sm.process_event(e1{}));
  expect(sm.process_event(e2{}));
  expect(sm.process_event(e3{}));
  expect(sm.is(s4));
};

test transition_loop = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e1>
        , s1 == s2 + event<e2>
        , s2 == idle + event<e3>
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  expect(sm.is(idle));
  expect(sm.process_event(e1{}));
  expect(sm.is(s1));
  expect(sm.process_event(e2{}));
  expect(sm.is(s2));
  expect(sm.process_event(e3{}));
  expect(sm.is(idle));
  expect(sm.process_event(e1{}));
  expect(sm.is(s1));
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

  msm::sm<c> sm;
  expect(sm.process_event(e1{}));
  expect(!sm.process_event(e3{}));
  expect(sm.process_event(e2{}));
  expect(!sm.process_event(e1{}));
  expect(sm.is(s2));
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

  msm::sm<c> sm;
  expect(sm.process_event(e1{}));
  expect(sm.process_event(e2{}));
  expect(sm.process_event(e3{}));
  using namespace msm;
  expect(sm.is("s4"_s));
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
    expect(sm.is(s2));
  }

  {
    msm::sm<c> sm;
    expect(sm.process_event(e1{}));
    expect(sm.process_event(42.f));
    expect(sm.is(s3));
  }
};

test initial_transition_overload = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e1>
        , idle == s2 + event<e2>
      );
      // clang-format on
    }
  };

  {
    msm::sm<c> sm;
    expect(sm.process_event(e1{}));
    expect(sm.is(s1));
  }

  {
    msm::sm<c> sm;
    expect(sm.process_event(e2{}));
    expect(sm.is(s2));
  }
};

test transition_process_event = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e1>
        , s1 == s2 + event<e2> / process_event(e3{})
        , s2 == terminate + event<e3>
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  expect(sm.is(idle));
  expect(sm.process_event(e1{}));
  expect(sm.is(s1));
  expect(sm.process_event(e2{}));  // + process_event(e3{})
  expect(sm.is(msm::terminate));
};

test transition_entry_exit_actions = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      auto entry_action = [this] { a_entry_action++; };
      auto exit_action = [this] { a_exit_action++; };

      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e1>
        , s1 + msm::on_entry / entry_action
        , s1 + msm::on_exit / exit_action
        , s1 == s2 + event<e2>
      );
      // clang-format on
    }

    int a_entry_action = 0;
    int a_exit_action = 0;
  };

  c c_;
  msm::sm<c> sm{c_};
  expect(!c_.a_entry_action);
  expect(sm.is(idle));
  expect(sm.process_event(e1{}));
  expect(1 == c_.a_entry_action);
  expect(sm.is(s1));
  expect(0 == c_.a_exit_action);
  expect(sm.process_event(e2{}));
  expect(1 == c_.a_entry_action);
  expect(1 == c_.a_exit_action);
  expect(sm.is(s2));
};

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
      auto guard1 = [] { return true; };
      auto guard2 = [](auto) { return false; };
      auto guard3 = [=](int v) { return [=] { return guard2(v); }; };
      auto action1 = [] {};
      auto action2 = [](int, auto, float &) {};

      // clang-format off
      return make_transition_table(
        idle(initial) == s1 + event<e1> [ guard1] / (action1, []{})
        , s1 == s2 + event<e2> / ([] { })
        , s2 == s1 + event<e4> / process_event(e5{})
        , s1 / [] {}
        , s1 [guard1 && guard3(42)]
        , s1 [guard1] / action1
        , s1 + event<e3> / action1
        , idle == s1
        , s3 + event<e4> / [] { }
        , s3 + event<e5> [guard1] / action1
        , s1 == s2 [guard1 && guard1 && [] { return true; }]
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
        , idle == s1 + event<e1> [guard1 && guard2] / (action1, action2)
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

  float f = 12.0;
  msm::sm<c> sm{f, 42, 87.0, 0.0f};
};

test terminate_state = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e1>
        , s1 == terminate + event<e2>
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  expect(sm.is(idle));
  expect(sm.process_event(e1{}));
  expect(sm.is(s1));
  expect(sm.process_event(e2{}));
  expect(sm.is(msm::terminate));
  expect(!sm.process_event(e1{}));
  expect(!sm.process_event(e2{}));
  expect(!sm.process_event(e3{}));
};

test is_state = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e1>
        , s1 == s2 + event<e2>
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  expect(sm.is(idle));
  expect(sm.process_event(e1{}));
  expect(sm.is(s1));
  expect(sm.process_event(e2{}));
  expect(sm.is(s2));
  expect(!sm.process_event(e3{}));
  expect(sm.is(s2));
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

  msm::sm<c> sm;
  expect(sm.is(idle, idle2));
  expect(sm.process_event(e1{}));
  expect(sm.is(s1, idle2));
  expect(sm.process_event(e2{}));
  expect(sm.is(s2, idle2));
  expect(sm.process_event(e3{}));
  expect(sm.is(s2, s3));
  expect(sm.process_event(e4{}));
  expect(sm.is(s2, s4));
};

test orthogonal_regions_event_consumed_by_all_regions = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e1>
        , s1 == s2 + event<e2>

        , idle2(initial) == s3 + event<e3>
        , s3 == s4 + event<e2>
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  expect(sm.is(idle, idle2));
  expect(sm.process_event(e1{}));
  expect(sm.is(s1, idle2));
  expect(sm.process_event(e3{}));
  expect(sm.is(s1, s3));
  expect(sm.process_event(e2{}));  // consume by both regions
  expect(sm.is(s2, s4));
};

test state_names = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          "idle"_s(initial) == "s1"_s + event<e1>
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  sm.visit_current_states([](auto state) { expect(std::string{"idle"} == std::string{state.c_str()}); });
  sm.process_event(e1{});
  sm.visit_current_states([](auto state) { expect(std::string{"s1"} == std::string{state.c_str()}); });
};

test dependencies = [] {
  static constexpr auto i_ = 42;
  static constexpr auto d_ = 87.0;

  struct c {
    auto configure() noexcept {
      using namespace msm;
      auto guard = [](int i) {
        expect(i == i_);
        return true;
      };

      auto action = [](double d, auto event) {
        expect(d == d_);
        expect(aux::is_same<e1, decltype(event)>::value);
        return true;
      };

      // clang-format off
      return make_transition_table(
          idle(initial) == terminate + event<e1> [ guard ] / action
      );
      // clang-format on
    }
  };

  {
    msm::sm<c> sm{(int)i_, (double)d_};
    expect(sm.process_event(e1{}));
    expect(sm.is(msm::terminate));
  }

  {
    msm::sm<c> sm{(double)d_, (int)i_};
    expect(sm.process_event(e1{}));
    expect(sm.is(msm::terminate));
  }
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
      state<sm<sub>> sub_state;

      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e1> [guard2{}] / [this] { a_initial = true; }
        , s1 == sub_state + event<e2> [guard]  / [this]{ a_enter_sub_sm = true; }
        , sub_state == s2 + event<e5> [guard2{}] / [this] { a_exit_sub_sm = true; }
      );
      // clang-format on
    }

    bool a_initial = false;
    bool a_enter_sub_sm = false;
    bool a_exit_sub_sm = false;
  };

  c c_;
  sub sub_;
  msm::sm<sub> subsm{sub_};
  msm::sm<c> sm{c_, subsm, 87.0, 42};

  expect(sm.is(idle));
  expect(sm.process_event(e1()));
  expect(c_.a_initial);

  sm.process_event(e2());
  expect(c_.a_enter_sub_sm);
  expect(0 == sub_.a_in_sub);

  expect(sm.process_event(e3()));
  expect(1 == sub_.a_in_sub);

  expect(sm.process_event(e4()));
  expect(2 == sub_.a_in_sub);
  expect(sm.process_event(e5()));
  expect(2 == sub_.a_in_sub);
  expect(c_.a_exit_sub_sm);
  expect(sm.is(s2));
};

test composite_def_ctor = [] {
  static auto in_sub = 0;

  struct sub {
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e3> / [] { in_sub++; }
        , s1 == s2 + event<e4> / [] { in_sub++; }
      );
      // clang-format on
    }
  };

  struct c {
    auto configure() noexcept {
      using namespace msm;
      state<sm<sub>> sub_state;

      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e1>
        , s1 == sub_state + event<e2>
        , sub_state == s2 + event<e5>
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;

  expect(sm.is(idle));
  expect(sm.process_event(e1()));

  sm.process_event(e2());
  expect(0 == in_sub);

  expect(sm.process_event(e3()));
  expect(1 == in_sub);

  expect(sm.process_event(e4()));
  expect(2 == in_sub);
  expect(sm.process_event(e5()));
  expect(2 == in_sub);
  expect(sm.is(s2));
};

test composite_transition_the_same_event = [] {
  struct sub {
    sub() {}
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e1>
        , s1 == s2 + event<e2>
      );
      // clang-format on
    }
  };

  static msm::state<msm::sm<sub>> sub_state;

  struct c {
    c(){};
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          idle(initial) == sub_state + event<e1>
        , sub_state == terminate + event<e3>
      );
      // clang-format on
    }
  };

  c c_;
  sub sub_;
  msm::sm<sub> subsm{sub_};
  msm::sm<c> sm{c_, subsm};

  expect(sm.is(idle));
  expect(sm.process_event(e1()));
  expect(sm.is(sub_state));
  expect(subsm.is(idle));

  expect(!sm.process_event(e4()));

  sm.process_event(e1());
  expect(sm.is(sub_state));
  expect(subsm.is(s1));

  expect(sm.process_event(e2()));
  expect(sm.is(sub_state));
  expect(subsm.is(s2));

  expect(sm.process_event(e3()));
  expect(sm.is(msm::terminate));
  expect(subsm.is(s2));
};

test composite_custom_ctor = [] {
  static auto in_sub = 0;

  struct sub {
    explicit sub(int i) : i(i) {}

    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e3> / [this] { in_sub+=i; }
        , s1 == s2 + event<e4> / [this] { in_sub+=i; }
      );
      // clang-format on
    }
    int i = 0;
  };

  struct c {
    auto configure() noexcept {
      using namespace msm;
      state<sm<sub>> sub_state;

      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e1>
        , s1 == sub_state + event<e2>
        , sub_state == s2 + event<e5>
      );
      // clang-format on
    }
  };

  constexpr auto i = 2;
  auto test = [=](auto &&sm) {
    expect(sm.is(idle));
    expect(sm.process_event(e1()));

    sm.process_event(e2());
    expect(0 == in_sub);

    expect(sm.process_event(e3()));
    expect(1 * i == in_sub);

    expect(sm.process_event(e4()));
    expect(2 * i == in_sub);
    expect(sm.process_event(e5()));
    expect(2 * i == in_sub);
    expect(sm.is(s2));
  };

  {
    in_sub = 0;
    sub sub_{i};
    msm::sm<sub> subsm{sub_};
    msm::sm<c> sm{subsm};
    test(static_cast<decltype(sm) &&>(sm));
  }

  {
    in_sub = 0;
    sub sub_{i};
    c c_;
    msm::sm<sub> subsm{c_, sub_};
    msm::sm<c> sm{subsm};
    test(static_cast<decltype(sm) &&>(sm));
  }

  {
    in_sub = 0;
    sub sub_{i};
    c c_;
    msm::sm<sub> subsm{sub_, c_};
    msm::sm<c> sm{subsm};
    test(static_cast<decltype(sm) &&>(sm));
  }
};

test composite_with_orthogonal_regions = [] {
  struct sub {
    sub() {}
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e4>
        , s1 == s2 + event<e4>
      );
      // clang-format on
    }
  };

  struct c {
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<e1>
        , s1 == terminate + event<e2>
        // ----------------------------------
        , idle2(initial) == sub_state + event<e1>
        , sub_state == terminate + event<e3>
      );
      // clang-format on
    }

    msm::state<msm::sm<sub>> sub_state;
  };

  c c_;
  sub sub_;
  msm::sm<sub> subsm{sub_};
  msm::sm<c> sm{c_, subsm};

  expect(sm.is(idle, idle2));
  expect(sm.process_event(e1()));
  expect(sm.is(s1, c_.sub_state));

  expect(subsm.is(idle));
  expect(sm.process_event(e4()));
  expect(subsm.is(s1));
  expect(sm.process_event(e4()));
  expect(subsm.is(s2));

  expect(sm.process_event(e2()));
  expect(sm.process_event(e3()));
  expect(subsm.is(s2));
  expect(sm.is(msm::terminate, msm::terminate));
};

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
struct event3 {
  static constexpr auto id = 3;
  event3(const runtime_event &) {}
};
test dispatch_runtime_event = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<event1>
        , s1 == s2 + event<event2>
        , s2 == terminate + event<event3>
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  expect(sm.is(idle));
  auto dispatcher = msm::make_dispatch_table<runtime_event, 1 /*min*/, 10 /*max*/>(sm);

  {
    runtime_event event{1};
    expect(dispatcher(event, event.id));
    expect(sm.is(s1));
  }

  {
    runtime_event event{9};
    expect(!dispatcher(event, event.id));
    expect(sm.is(s1));
  }

  {
    runtime_event event{2};
    expect(dispatcher(event, event.id));
    expect(sm.is(s2));
  }

  {
    runtime_event event{3};
    expect(dispatcher(event, event.id));
    expect(sm.is(msm::terminate));
  }

  {
    runtime_event event{5};
    expect(!dispatcher(event, event.id));
    expect(sm.is(msm::terminate));
  }
};

test dispatch_runtime_event_sub_sm = [] {
  static auto in_sub = 0;

  struct sub {
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          idle(initial) == s1 + event<event2> / [] { in_sub++; }
      );
      // clang-format on
    }
  };

  static msm::state<msm::sm<sub>> sub_state;

  struct c {
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          idle(initial) == sub_state + event<event1>
        , sub_state == terminate + event<event3>
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  expect(sm.is(idle));

  auto dispatcher = msm::make_dispatch_table<runtime_event, 1 /*min*/, 4 /*max*/>(sm);

  {
    runtime_event event{1};
    expect(dispatcher(event, event.id));
    expect(sm.is(sub_state));
    expect(0 == in_sub);
  }

  {
    runtime_event event{2};
    expect(dispatcher(event, event.id));
    expect(sm.is(sub_state));
    expect(1 == in_sub);
  }

  {
    runtime_event event{3};
    expect(dispatcher(event, event.id));
    expect(sm.is(msm::terminate));
    expect(1 == in_sub);
  }

  {
    runtime_event event{4};
    expect(!dispatcher(event, event.id));
    expect(sm.is(msm::terminate));
    expect(1 == in_sub);
  }
};
