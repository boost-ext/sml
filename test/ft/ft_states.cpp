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

