//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <string>
#include <utility>

namespace sml = boost::sml;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};
struct e6 {};
struct e7 {
  int i = 0;
};

const auto idle = sml::state<class idle>;
const auto s1 = sml::state<class s1>;
const auto s2 = sml::state<class s2>;

test terminate_state = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  expect(sm.is(idle));
  sm.process_event(e1{});
  expect(sm.is(s1));
  sm.process_event(e2{});
  expect(sm.is(sml::X));
  sm.process_event(e1{});
  sm.process_event(e2{});
  sm.process_event(e3{});
  expect(sm.is(sml::X));
};

test is_state = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> = s2
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  expect(sm.is(idle));
  sm.process_event(e1{});
  expect(sm.is(s1));
  sm.process_event(e2{});
  expect(sm.is(s2));
  sm.process_event(e3{});
  expect(sm.is(s2));
};

test states_entry_exit_actions = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      auto entry_action = [this] { a_entry_action++; };
      auto exit_action = [this] { a_exit_action++; };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + sml::on_entry<_> / entry_action
        , s1 + sml::on_exit<_> / exit_action
        , s1 + event<e2> = s2
      );
      // clang-format on
    }

    int a_entry_action = 0;
    int a_exit_action = 0;
  };

  c c_;
  sml::sm<c> sm{c_};
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

#if !defined(_MSC_VER)
template <int N>
struct generic_on_entry {
  void operator()(const e7& event) { expect(N == event.i); }

  template <class TEvent>
  void operator()(const TEvent&) {}
};

test states_entry_exit_actions_with_events = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e7> = s1
        , s1 + sml::on_entry<e7> / [](const auto& event) { expect(42 == event.i); }
        , s1 + sml::on_exit<_> / generic_on_entry<42>{}
        , s1 + event<e1> = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  sm.process_event(e7{42});
  expect(sm.is(s1));
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

test state_names = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;

      // clang-format off
      return make_transition_table(
        *"idle"_s + event<e1> = "s1"_s
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  sm.visit_current_states([](auto state) { expect(std::string{"idle"} == std::string{state.c_str()}); });
  sm.process_event(e1{});
  sm.visit_current_states([](auto state) { expect(std::string{"s1"} == std::string{state.c_str()}); });
};
#endif
