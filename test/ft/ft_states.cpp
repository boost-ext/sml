//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/msm-lite.hpp>
#include <string>
#include <utility>

namespace msm = boost::msm::lite;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};
struct e6 {};

const auto idle = msm::state<class idle>;
const auto s1 = msm::state<class s1>;
const auto s2 = msm::state<class s2>;

test terminate_state = [] {
  struct c {
    auto operator()() noexcept {
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
  sm.process_event(e1{});
  expect(sm.is(s1));
  sm.process_event(e2{});
  expect(sm.is(msm::X));
  sm.process_event(e1{});
  sm.process_event(e2{});
  sm.process_event(e3{});
  expect(sm.is(msm::X));
};

test is_state = [] {
  struct c {
    auto operator()() noexcept {
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
  sm.process_event(e1{});
  expect(sm.is(s1));
  sm.process_event(e2{});
  expect(sm.is(s2));
  sm.process_event(e3{});
  expect(sm.is(s2));
};

test state_names = [] {
  struct c {
    auto operator()() noexcept {
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
  sm.process_event(e1{});
  sm.visit_current_states([](auto state) { expect(std::string{"s1"} == std::string{state.c_str()}); });
};

test states_entry_exit_actions = [] {
  struct c {
    auto operator()() noexcept {
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
  sm.process_event(e1{});
  expect(1 == c_.a_entry_action);
  expect(sm.is(s1));
  expect(0 == c_.a_exit_action);
  sm.process_event(e2{});
  expect(1 == c_.a_entry_action);
  expect(1 == c_.a_exit_action);
  expect(sm.is(s2));
};

test states_entry_exit_sub_sm = [] {
  struct sub_1 {
    auto operator()() noexcept {
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
    auto operator()() noexcept {
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
    auto operator()() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
        *state<sub_1> + event<e3> = state<sub_2>
       , state<sub_2> + event<e4> = state<sub_1>
       , state<sub_1> + msm::on_entry / [this] { sub1_entry = true; }
       , state<sub_1> + msm::on_exit  / [this] { sub1_exit = true; }
       , state<sub_2> + msm::on_entry / [this] { sub2_entry = true; }
       , state<sub_2> + msm::on_exit  / [this] { sub2_exit = true; }
      );
      // clang-format on
    }

    bool sub1_entry = false;
    bool sub1_exit = false;
    bool sub2_entry = false;
    bool sub2_exit = false;
  };

  c c_;
  sub_1 s1;
  sub_2 s2;
  msm::sm<c> sm{c_, s1, s2};

  sm.process_event(e1{});
  expect(s1.ls1_1_exit);
  expect(s1.ls1_2_entry);

  sm.process_event(e2{});
  expect(s1.ls1_2_exit);
  expect(s1.ls1_1_entry);

  sm.process_event(e3{});
  expect(s1.ls1_1_exit);
  expect(c_.sub1_exit);
  expect(s2.ls2_1_entry);
  expect(c_.sub2_entry);

  sm.process_event(e4{});
  expect(s2.ls2_1_exit);
  expect(c_.sub2_exit);
  expect(s1.ls1_1_entry);
  expect(c_.sub1_entry);
};
