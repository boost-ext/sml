//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <string>
#include "boost/msm-lite.hpp"
// clang-format off
#if __has_include(<boost/di.hpp>)
// clang-format on
#include <boost/di.hpp>
namespace di = boost::di;
#endif

namespace msm = boost::msm::lite;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};
struct e6 {};

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
      return make_transition_table(*idle + event<e1> / [] {});
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
      return make_transition_table(*idle + event<e1> / [](int i) { expect(42 == i); });
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
    auto configure() noexcept { return make_transition_table(*idle + event<e1> = s1); }
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
         *idle + event<e1> = s1
        , s1 + event<e2> / [] {}
        , s1 + event<e3> = s2
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
       *idle / [this] { a_called = true; } = s1
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
      return make_transition_table(*idle + event<e1> = s1);
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
      return make_transition_table(*idle + event<e1> / action = s1);
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
      return make_transition_table(*idle + event<e1> / action = s1);
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

      return make_transition_table(*idle + event<e1>[guard] / action = s1);
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

      return make_transition_table(*idle + event<e1>[guard] / action = s1);
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

test operators = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      auto yes = [] { return true; };
      auto no = [] { return false; };
      auto action = [](int &i) { i++; };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> [ yes || no ] / (action, action) = s1
        , s1 + event<e1> [ !no && yes ] / action = s2
        , s2 + event<e1> [ no && yes ] / (action, [](int&i) {i++;}) = s3
        , s2 + event<e2> [ yes && [] { return true; } ] / (action, [](int&i) {i++;}) = s3
      );
      // clang-format on
    }
  };

  int i = 0;
  msm::sm<c> sm{i};

  {
    i = 0;
    expect(sm.process_event(e1{}));
    expect(sm.is(s1));
    expect(2 == i);
  }

  {
    i = 0;
    expect(sm.process_event(e1{}));
    expect(sm.is(s2));
    expect(1 == i);
  }

  {
    i = 0;
    expect(!sm.process_event(e1{}));
    expect(sm.is(s2));
    expect(0 == i);
  }

  {
    i = 0;
    expect(sm.process_event(e2{}));
    expect(sm.is(s3));
    expect(2 == i);
  }
};

test transitions = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      auto yes = [] { return true; };
      auto no = [] { return false; };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> = s2
        , s2 + event<e3> [no] = s3
        , s2 + event<e3> [yes] = s4
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

test transitions_dsl = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      auto yes = [] { return true; };
      auto no = [] { return false; };

      // clang-format off
      return make_transition_table(
          s1 <= *idle + event<e1>
        , s2 <= s1 + event<e2>
        , s3 <= s2 + event<e3> [no]
        , s4 <= s2 + event<e3> [yes]
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

test transitions_dsl_mix = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      auto yes = [] { return true; };
      auto no = [] { return false; };

      // clang-format off
      return make_transition_table(
          s1 <= *idle + event<e1>
        , s1 + event<e2> = s2
        , s3 <= s2 + event<e3> [no]
        , s2 + event<e3> [yes] = s4
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
         *idle + event<e1> = s1
        , s1 + event<e2> = s2
        , s2 + event<e3> = idle
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
         *idle + event<e1> = s1
        , s1 + event<e2> [no] = s2
        , s1 + event<e2> [yes] = s2
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
         *"idle"_s + event<e1> = "s1"_s
        , "s1"_s + event<e2> = "s2"_s
        , "s2"_s + event<e3> [no] = "s3"_s
        , "s2"_s + event<e3> [yes] = "s4"_s
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
         *idle + event<e1> = s1
        , s1 + event<int> = s2
        , s1 + event<float> = s3
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
         *idle + event<e1> = s1
        , idle + event<e2> = s2
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
         *idle + event<e1> = s1
        , s1 + event<e2> / process_event(e3{}) = X
        , s1 + event<e3> / [this] { a_called++; }
      );
      // clang-format on
    }

    int a_called = 0;
  };

  c c_;
  msm::sm<c> sm{c_};
  expect(sm.is(idle));
  expect(sm.process_event(e1{}));
  expect(sm.is(s1));
  expect(!c_.a_called);
  expect(sm.process_event(e2{}));  // + process_event(e3{})
  expect(1 == c_.a_called);
  expect(sm.is(msm::X));
};

test transition_entry_exit_actions = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      auto entry_action = [this] { a_entry_action++; };
      auto exit_action = [this] { a_exit_action++; };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + msm::on_entry / entry_action
        , s1 + msm::on_exit / exit_action
        , s1 + event<e2> = s2
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

test transition_entry_exit_sub_sm = [] {
  struct sub_1 {
    sub_1() = default;
    auto configure() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
        *"ls1_1"_s + event<e1> = "ls1_2"_s
       , "ls1_2"_s + event<e2> = "ls1_1"_s
       , "ls1_1"_s + msm::on_entry / [this] { ls1_1_entry = true; }
       , "ls1_1"_s + msm::on_exit  / [this] { ls1_1_exit = true; }
       , "ls1_2"_s + msm::on_entry / [this] { ls1_2_entry = true; }
       , "ls1_2"_s + msm::on_exit  / [this] { ls1_2_exit = true; }
      );
      // clang-format on
    }

    bool ls1_1_entry = false;
    bool ls1_1_exit = false;
    bool ls1_2_entry = false;
    bool ls1_2_exit = false;
  };

  struct sub_2 {
    sub_2() = default;
    auto configure() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
        *"ls2_1"_s + event<e1> = "ls2_2"_s
       , "ls2_2"_s + event<e2> = "ls2_1"_s
       , "ls2_1"_s + msm::on_entry / [this] { ls2_1_entry = true; }
       , "ls2_1"_s + msm::on_exit  / [this] { ls2_1_exit = true; }
       , "ls2_2"_s + msm::on_entry / [this] { ls2_2_entry = true; }
       , "ls2_2"_s + msm::on_exit  / [this] { ls2_2_exit = true; }
      );
      // clang-format on
    }

    bool ls2_1_entry = false;
    bool ls2_1_exit = false;
    bool ls2_2_entry = false;
    bool ls2_2_exit = false;
  };

  struct c {
    auto configure() noexcept {
      using namespace msm;
      state<sm<sub_1>> ss1;
      state<sm<sub_2>> ss2;

      // clang-format off
      return make_transition_table(
        *ss1 + event<e3> = ss2
       , ss2 + event<e4> = ss1
       , ss1 + msm::on_entry / [this] { sub1_entry = true; }
       , ss1 + msm::on_exit  / [this] { sub1_exit = true; }
       , ss2 + msm::on_entry / [this] { sub2_entry = true; }
       , ss2 + msm::on_exit  / [this] { sub2_exit = true; }
      );
    // clang-format on
    }

    bool sub1_entry = false;
    bool sub1_exit = false;
    bool sub2_entry = false;
    bool sub2_exit = false;
  };

  sub_1 s1;
  msm::sm<sub_1> sub1{s1};
  sub_2 s2;
  msm::sm<sub_2> sub2{s2};
  c c_;
  msm::sm<c> sm{c_, sub1, sub2};

  expect(sm.process_event(e1{}));
  expect(s1.ls1_1_exit);
  expect(s1.ls1_2_entry);

  expect(sm.process_event(e2{}));
  expect(s1.ls1_2_exit);
  expect(s1.ls1_1_entry);

  expect(sm.process_event(e3{}));
  expect(s1.ls1_1_exit);
  expect(c_.sub1_exit);
  expect(s2.ls2_1_entry);
  expect(c_.sub2_entry);

  expect(sm.process_event(e4{}));
  expect(s2.ls2_1_exit);
  expect(c_.sub2_exit);
  expect(s1.ls1_1_entry);
  expect(c_.sub1_entry);
};

struct c_guard {
  template <class T>
  bool operator()(const T &) const noexcept {
    return true;
  }
};

struct c_action {
  explicit c_action(int) {}
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
         *idle + event<e1> [guard1] / (action1, []{}) = s1
        , s1 + event<e2> / ([] { }) = s2
        , s2 + event<e4> / process_event(e5{}) = s1
        , s1 / [] {}
        , s1 [guard1 && guard3(42)]
        , s1 [guard1] / action1
        , s1 + event<e3> / action1
        , idle = s1
        , s3 + event<e4> / [] { }
        , s3 + event<e5> [guard1] / action1
        , s1 [guard1 && guard1 && [] { return true; }] = s2
        , (*s3) [guard1] = s2
        , s2 [guard1 && !guard2] = s3
        , s3 [guard1] / action1 = s4
        , s4 / action1 = s5
        , s5 / (action1, action2) = end
        , idle(H) + event<e1> = s1
        , idle(H) + event<e1> [c_guard{} && guard1] = s1
        ,*idle + event<e1> [guard1] / action1 = s1
        , idle + event<e1> / action1 = s1
        , idle + event<e1> / (action1, c_action{1}) = s1
        , idle + event<e1> [guard1 && guard2] / (action1, action2) = s1
        , idle + event<e1> [guard1 && guard2] / (action1, action2, []{}) = s1
        , idle + event<e1> [guard1 || !guard2] / (action1, action2, []{}, [](auto){}) = s1
        , idle + event<e2> [guard1 || guard2] / (action1, action2, []{}, [](int, auto, float){}) = s1
        , idle + event<e1> [guard1 && guard2 && [] { return true; } ] / (action1, action2, []{}, [](int, auto, float){}) = X
        , idle + event<e1> [guard1 && guard2 && [] { return true; } && [] (auto) { return false; } ] / (action1, action2, []{}, [](int, auto, double){}) = X
        //
        , s1 <= *idle + event<e1> [guard1] / (action1, []{})
        , s2 <= s1 + event<e2> / ([] { })
        , s1 <= s2 + event<e4> / process_event(e5{})
        , s2 <= s1 [guard1 && guard1 && [] { return true; }]
        , s2 <= (*s3) [guard1]
        , s2 <= s3 [guard1 && !guard2]
        , s3 <= s4 [guard1] / action1
        , s4 <= s5 / action1
        , end <= s5 / (action1, action2)
        , s1 <= idle(H) + event<e1>
        , s1 <= idle(H) + event<e1> [c_guard{} && guard1]
        ,*s1 <= idle + event<e1> [guard1] / action1
        , s1 <= idle + event<e1> / action1
        , s1 <= idle + event<e1> / (action1, c_action{1}, c_action{2})
        , s1 <= idle + event<e1> [guard1 && guard2] / (action1, action2)
        , s1 <= idle + event<e1> [guard1 && guard2] / (action1, action2, []{})
        , s1 <= idle + event<e1> [guard1 || !guard2] / (action1, action2, []{}, [](auto){})
        , s1 <= idle + event<e2> [guard1 || guard2] / (action1, action2, []{}, [](int, auto, float){})
        , X <= idle + event<e1> [guard1 && guard2 && [] { return true; } ] / (action1, action2, []{}, [](int, auto, float){})
        , X <= idle + event<e1> [guard1 && guard2 && [] { return true; } && [] (auto) { return false; } ] / (action1, action2, []{}, [](int, auto, double){})
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
         *idle + event<e1> = s1
        , s1 + event<e2> = X
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  expect(sm.is(idle));
  expect(sm.process_event(e1{}));
  expect(sm.is(s1));
  expect(sm.process_event(e2{}));
  expect(sm.is(msm::X));
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
         *idle + event<e1> = s1
        , s1 + event<e2> = s2
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

test events = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
          *"idle"_s + "e1"_t = "s1"_s
        , "s1"_s + "e2"_t = "s2"_s
        , "s2"_s + "e3"_t / [](auto) { } = X
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  using namespace msm;
  expect(sm.process_event("e1"_t));
  expect(sm.is("s1"_s));
  expect(sm.process_event("e2"_t));
  expect(sm.is("s2"_s));
  expect(sm.process_event("e3"_t));
  expect(sm.is(X));
};

test orthogonal_regions = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> = s2

        ,*idle2 + event<e3> = s3
        , s3 + event<e4> = s4
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
         *idle + event<e1> = s1
        , s1 + event<e2> = s2

        ,*idle2 + event<e3> = s3
        , s3 + event<e2> = s4
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
          *"idle"_s + event<e1> = "s1"_s
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  sm.visit_current_states([](auto state) { expect(std::string{"idle"} == std::string{state.c_str()}); });
  expect(sm.process_event(e1{}));
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
         *idle + event<e1> [ guard ] / action = X
      );
      // clang-format on
    }
  };

  {
    msm::sm<c> sm{(int)i_, (double)d_};
    expect(sm.process_event(e1{}));
    expect(sm.is(msm::X));
  }

  {
    msm::sm<c> sm{(double)d_, (int)i_};
    expect(sm.process_event(e1{}));
    expect(sm.is(msm::X));
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
         *idle + event<e3> / [this] { a_in_sub++; } = s1
        , s1 + event<e4> / [this] { a_in_sub++; } = s2
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
         *idle + event<e1> [guard2{}] / [this] { a_initial = true; } = s1
        , s1 + event<e2> [guard]  / [this]{ a_enter_sub_sm = true; } = sub_state
        , sub_state + msm::on_entry / [this] { a_on_entry_sub_sm = true; }
        , sub_state + msm::on_exit / [this] { a_on_exit_sub_sm = true; }
        , sub_state + event<e5> [guard2{}] / [this] { a_exit_sub_sm = true; } = s2
      );
      // clang-format on
    }

    bool a_initial = false;
    bool a_enter_sub_sm = false;
    bool a_exit_sub_sm = false;
    bool a_on_exit_sub_sm = false;
    bool a_on_entry_sub_sm = false;
  };

  c c_;
  sub sub_;
  msm::sm<sub> subsm{sub_};
  msm::sm<c> sm{c_, subsm, 87.0, 42};

  expect(sm.is(idle));
  expect(sm.process_event(e1()));
  expect(c_.a_initial);

  expect(sm.process_event(e2()));
  expect(c_.a_enter_sub_sm);
  expect(c_.a_on_entry_sub_sm);
  expect(0 == sub_.a_in_sub);

  expect(sm.process_event(e3()));
  expect(1 == sub_.a_in_sub);

  expect(sm.process_event(e4()));
  expect(2 == sub_.a_in_sub);
  expect(sm.process_event(e5()));
  expect(2 == sub_.a_in_sub);
  expect(c_.a_on_entry_sub_sm);
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
         *idle + event<e3> / [] { in_sub++; } = s1
        , s1 + event<e4> / [] { in_sub++; } = s2
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
         *idle + event<e1> = s1
        , s1 + event<e2> = sub_state
        , sub_state + event<e5> = s2
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;

  expect(sm.is(idle));
  expect(sm.process_event(e1()));

  expect(sm.process_event(e2()));
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
         *idle + event<e1> = s1
        , s1 + event<e2> = s2
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
         *idle + event<e1> = sub_state
        , sub_state + event<e3> = "s1"_s
        , "s1"_s + event<e4> = sub_state
        , sub_state + event<e5> = X
      );
      // clang-format on
    }
  };

  using namespace msm;

  c c_;
  sub sub_;
  msm::sm<sub> subsm{sub_};
  msm::sm<c> sm{c_, subsm};

  expect(sm.is(idle));
  expect(sm.process_event(e1()));
  expect(sm.is(sub_state));
  expect(subsm.is(idle));

  expect(!sm.process_event(e4()));

  expect(sm.process_event(e1()));
  expect(sm.is(sub_state));
  expect(subsm.is(s1));

  expect(sm.process_event(e2()));
  expect(sm.is(sub_state));
  expect(subsm.is(s2));

  expect(sm.process_event(e3()));
  expect(sm.is("s1"_s));
  expect(subsm.is(s2));

  expect(sm.process_event(e4()));
  expect(sm.is(sub_state));
  expect(subsm.is(idle));  // no history
};

test composite_history = [] {
  struct sub {
    sub() {}
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          idle(H) + event<e1> = s1
        , s1 + event<e2> = s2
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
         *idle + event<e1> = sub_state
        , sub_state + event<e3> = "s1"_s
        , "s1"_s + event<e4> = sub_state
        , sub_state + event<e5> = X
      );
      // clang-format on
    }
  };

  using namespace msm;

  c c_;
  sub sub_;
  msm::sm<sub> subsm{sub_};
  msm::sm<c> sm{c_, subsm};

  expect(sm.is(idle));
  expect(sm.process_event(e1()));
  expect(sm.is(sub_state));
  expect(subsm.is(idle));

  expect(!sm.process_event(e4()));

  expect(sm.process_event(e1()));
  expect(sm.is(sub_state));
  expect(subsm.is(s1));

  expect(sm.process_event(e2()));
  expect(sm.is(sub_state));
  expect(subsm.is(s2));

  expect(sm.process_event(e3()));
  expect(sm.is("s1"_s));
  expect(subsm.is(s2));

  expect(sm.process_event(e4()));
  expect(sm.is(sub_state));
  expect(subsm.is(s2));  // history
};

test composite_history_region = [] {
  struct sub {
    sub() {}
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          idle(H) + event<e1> = s1
        , s1 + event<e2> = s2
        //-----------------------//
        , *idle2 + event<e2> = s3
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
         *idle + event<e1> = sub_state
        , sub_state + event<e3> = "s1"_s
        , "s1"_s + event<e4> = sub_state
        , sub_state + event<e5> = X
      );
      // clang-format on
    }
  };

  using namespace msm;

  c c_;
  sub sub_;
  msm::sm<sub> subsm{sub_};
  msm::sm<c> sm{c_, subsm};

  expect(sm.is(idle));
  expect(sm.process_event(e1()));
  expect(sm.is(sub_state));
  expect(subsm.is(idle, idle2));

  expect(!sm.process_event(e4()));

  expect(sm.process_event(e1()));
  expect(sm.is(sub_state));
  expect(subsm.is(s1, idle2));

  expect(sm.process_event(e2()));
  expect(sm.is(sub_state));
  expect(subsm.is(s2, s3));

  expect(sm.process_event(e3()));
  expect(sm.is("s1"_s));
  expect(subsm.is(s2, s3));

  expect(sm.process_event(e4()));
  expect(sm.is(sub_state));
  expect(subsm.is(s2, idle2));  // history for region 1
};

test composite_custom_ctor = [] {
  static auto in_sub = 0;

  struct sub {
    explicit sub(int i) : i(i) {}

    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
         *idle + event<e3> / [this] { in_sub+=i; } = s1
        , s1 + event<e4> / [this] { in_sub+=i; } = s2
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
         *idle + event<e1> = s1
        , s1 + event<e2> = sub_state
        , sub_state + event<e5> = s2
      );
      // clang-format on
    }
  };

  constexpr auto i = 2;
  auto test = [=](auto &&sm) {
    expect(sm.is(idle));
    expect(sm.process_event(e1()));

    expect(sm.process_event(e2()));
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
    msm::sm<sub> subsm{sub_};
    msm::sm<c> sm{subsm};
    test(static_cast<decltype(sm) &&>(sm));
  }

  {
    in_sub = 0;
    sub sub_{i};
    msm::sm<sub> subsm{sub_};
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
         *idle + event<e4> = s1
        , s1 + event<e4> = s2
      );
      // clang-format on
    }
  };

  struct c {
    c() {}
    auto configure() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> = X
        // ----------------------------------
        ,*idle2 + event<e1> = sub_state
        , sub_state + event<e3> = X
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
  expect(sm.is(msm::X, msm::X));
};

test composite_with_orthogonal_regions_explicit_entry = [] {
  struct sub {
    sub() {}
    auto configure() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
         (*idle) + event<e1> = s1
        , s1 + event<e2> = s2
        // ----------------------------------
        , (*idle2) + event<e3> = s3
        , s3 + event<e4> = X
      );
      // clang-format on
    }
  };

  static msm::state<msm::sm<sub>> sub_state;

  struct c {
    c() {}
    auto configure() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
       (*sub_state) + event<e5> = s1
      , s1 + event<e6> = sub_state(s2, s3) // explicit entry
      );
      // clang-format on
    }
  };

  sub sub_;
  msm::sm<sub> subsm{sub_};
  c c_;
  msm::sm<c> sm{c_, subsm};
  expect(sm.is(sub_state));
  expect(subsm.is(idle, idle2));

  expect(sm.process_event(e1()));
  expect(sm.is(sub_state));
  expect(subsm.is(s1, idle2));

  expect(sm.process_event(e5()));
  expect(sm.is(s1));
  expect(subsm.is(s1, idle2));

  expect(sm.process_event(e6()));  // go back to sub
  expect(sm.is(sub_state));
  expect(subsm.is(s2, s3));
};

test composite_with_orthogonal_regions_explicit_entry_deduce_region = [] {
  struct sub {
    sub() {}
    auto configure() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
         (*idle) + event<e1> = s1
        , s1 + event<e2> = s2
        // ----------------------------------
        , (*idle2) + event<e3> = s3
        , s3 + event<e4> = X
      );
      // clang-format on
    }
  };

  static msm::state<msm::sm<sub>> sub_state;

  struct c {
    c() {}
    auto configure() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
       (*sub_state) + event<e5> = s1
      , s1 + event<e6> = sub_state(s3) // explicit entry, deduce second region
      );
      // clang-format on
    }
  };

  sub sub_;
  msm::sm<sub> subsm{sub_};
  c c_;
  msm::sm<c> sm{c_, subsm};
  expect(sm.is(sub_state));
  expect(subsm.is(idle, idle2));

  expect(sm.process_event(e1()));
  expect(sm.is(sub_state));
  expect(subsm.is(s1, idle2));

  expect(sm.process_event(e5()));
  expect(sm.is(s1));
  expect(subsm.is(s1, idle2));

  expect(sm.process_event(e6()));  // go back to sub
  expect(sm.is(sub_state));
  // expect(subsm.is(idle, s3));
};

struct runtime_event {
  int id = 0;
};
struct event1 {
  static constexpr auto id = 1;
  explicit event1(const runtime_event &) {}
};
struct event2 {
  static constexpr auto id = 2;
};
struct event3 {
  static constexpr auto id = 3;
  explicit event3(const runtime_event &) {}
};

test dispatch_runtime_event = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
         *idle2 = idle // anonymous transition
        , idle + event<event1> = s1
        , s1 + event<event2> = s2
        , s2 + event<event3> = X
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
    expect(sm.is(msm::X));
  }

  {
    runtime_event event{5};
    expect(!dispatcher(event, event.id));
    expect(sm.is(msm::X));
  }
};

test dispatch_runtime_event_sub_sm = [] {
  static auto in_sub = 0;

  struct sub {
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
         *idle + event<event2> / [] { in_sub++; } = s1
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
         *idle + event<event1> = sub_state
        , sub_state + event<event3> = X
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
    expect(sm.is(msm::X));
    expect(1 == in_sub);
  }

  {
    runtime_event event{4};
    expect(!dispatcher(event, event.id));
    expect(sm.is(msm::X));
    expect(1 == in_sub);
  }
};

test sm_testing = [] {
  struct data {
    int value = 0;
  };

  struct c {
    auto configure() noexcept {
      using namespace msm;

      auto guard = [](const data &d) { return d.value == 42; };
      auto action = [](data &d) { d.value = 123; };

      struct Action {
        void operator()(data &d) noexcept { d.value = 12; }
      };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> [ guard ] / action = s1
        , s1 + event<e2> [ guard && [](bool b) { return b; } ] / Action{} = s2
        , s2 + event<e3> [ guard ] / [](data& d) { d.value = 87; } = X
      );
      // clang-format on
    }
  };

  {
    data fake_data;
    const data &c_fake_data = fake_data;
    msm::testing::sm<c> sm{c_fake_data, fake_data};
    expect(sm.is(idle));

    expect(!sm.process_event(e1{}));
    expect(sm.is(idle));
    expect(0 == fake_data.value);

    fake_data.value = 42;
    expect(sm.process_event(e1{}));
    expect(sm.is(s1));
    expect(123 == fake_data.value);
  }

  {
    data fake_data;
    const data &c_fake_data = fake_data;
    msm::testing::sm<c> sm{fake_data, c_fake_data};
    expect(sm.is(idle));

    sm.set_current_states(s2);
    fake_data.value = 42;
    expect(sm.process_event(e3{}));
    expect(sm.is(msm::X));
    expect(87 == fake_data.value);
  }

  {
    data fake_data;
    const data &c_fake_data = fake_data;
    msm::testing::sm<c> sm{c_fake_data, fake_data};
    expect(sm.is(idle));

    sm.set_current_states(s2);
    fake_data.value = 42;
    expect(sm.process_event(e3{}));
    expect(sm.is(msm::X));
    expect(87 == fake_data.value);
  }

  {
    data fake_data;
    const data &c_fake_data = fake_data;
    msm::testing::sm<c> sm{fake_data, c_fake_data, true};
    expect(sm.is(idle));

    sm.set_current_states(s1);
    fake_data.value = 42;
    expect(sm.process_event(e2{}));
    expect(sm.is(s2));
    expect(12 == fake_data.value);
  }
};

test sm_testing_orthogonal_regions = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
       *idle + event<e1> = s1
      , s1 + event<e2> = X

      ,*idle2 + event<e1> = s3
      , s3 + event<e2> = X
      );
      // clang-format on
    }
  };

  msm::testing::sm<c> sm;
  expect(sm.is(idle, idle2));
  sm.set_current_states(s1, s3);
  expect(sm.is(s1, s3));
};

test uml_notation = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> = X
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  expect(sm.process_event(e1{}));
  expect(sm.is(s1));
  expect(sm.process_event(e2{}));
  expect(sm.is(msm::X));
};

// clang-format off
#if __has_include(<boost/di.hpp>)
// clang-format on
test di_minimal = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      return make_transition_table(*idle + event<e1> / [](int i) { expect(42 == i); });
    }
  };

  auto injector = di::make_injector(di::bind<>.to(42));
  auto sm = injector.create<msm::sm<c>>();
  expect(sm.is(idle));
  expect(sm.process_event(e1{}));
  expect(sm.is(idle));
};

test di_complex = [] {
  struct i1 {
    virtual ~i1() noexcept = default;
    virtual void dummy() = 0;
  };
  struct impl1 : i1 {
    void dummy() override{};
  };

  struct i2 {
    virtual ~i2() noexcept = default;
    virtual void dummy() = 0;
  };
  struct impl2 : i2 {
    void dummy() override{};
  };

  struct i3 {
    virtual ~i3() noexcept = default;
    virtual void dummy() = 0;
  };
  struct impl3 : i3 {
    void dummy() override{};
  };

  struct i4 {
    virtual ~i4() noexcept = default;
    virtual void dummy() = 0;
  };
  struct impl4 : i4 {
    void dummy() override{};
  };

  struct i5 {
    virtual ~i5() noexcept = default;
    virtual void dummy() = 0;
  };
  struct impl5 : i5 {
    void dummy() override{};
  };

  struct c {
    auto configure() noexcept {
      using namespace msm;

      auto guard1 = [](int i, const auto &, double d) {
        expect(42 == i);
        expect(87.0 == d);
        return true;
      };

      auto guard2 = [](int i, i1 &p1, i2 &p2, i3 &p3) {
        expect(42 == i);
        expect(dynamic_cast<const impl1 *>(&p1));
        expect(dynamic_cast<const impl2 *>(&p2));
        expect(dynamic_cast<const impl3 *>(&p3));
        return true;
      };

      auto action1 = [](const auto &, double d, int i) {
        expect(42 == i);
        expect(87.0 == d);
      };

      auto action2 = [](const i1 &p1, const i2 &p2, const i3 &p3, const i4 &p4, const i5 &p5) {
        expect(dynamic_cast<const impl1 *>(&p1));
        expect(dynamic_cast<const impl2 *>(&p2));
        expect(dynamic_cast<const impl3 *>(&p3));
        expect(dynamic_cast<const impl4 *>(&p4));
        expect(dynamic_cast<const impl5 *>(&p5));
      };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> / [](int i) { expect(42 == i); } = s1
        , s1 + event<e2> [ guard1 && guard2 ] / (action1, action2) = X
      );
      // clang-format on
    }
  };

  // clang-format off
  auto f = 12.0f;
  auto injector = di::make_injector(
      di::bind<>.to(42)
    , di::bind<float>.to(f)
    , di::bind<>.to(87.0)
    , di::bind<i1>.to<impl1>()
    , di::bind<i2>.to<impl2>()
    , di::bind<i3>.to<impl3>()
    , di::bind<i4>.to<impl4>()
    , di::bind<i5>.to<impl5>()
  );
  // clang-format on
  auto sm = injector.create<msm::sm<c>>();
  expect(sm.is(idle));

  expect(sm.process_event(e1{}));
  expect(sm.is(s1));

  expect(sm.process_event(e2{}));
  expect(sm.is(msm::X));
};
#endif
