//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <utility>
#include <vector>

namespace sml = boost::sml;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};
struct e6 {};
struct e7 {
  explicit e7(int i) : i(i) {}
  int i = 0;
};

const auto idle = sml::state<class idle>;
const auto s1 = sml::state<class s1>;
const auto s2 = sml::state<class s2>;
const auto any = sml::state<sml::_>;

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

  sml::sm<c> sm{};
  c& c_ = sm;
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

template <int N>
struct generic_on_entry {
#if !defined(_MSC_VER)
  void operator()(const e7& event) { expect(N == event.i); }
#endif

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
        , s1 + sml::on_entry<e7> / [](const auto& event) -> void { expect(42 == event.i); }
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

test states_generic_entry_actions_with_events = [] {
  struct c {
    auto operator()() {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle = s1
        , s1 + event<e1> = s1
        , s1 + event<e2> = s1
        , s1 + event<e3> = s1
        , s1 + sml::on_entry<anonymous> / [this](auto const&) -> void { entries.push_back(std::type_index(typeid(anonymous))); }
        , s1 + sml::on_entry<e1>        / [this](e1 const&) { entries.push_back(std::type_index(typeid(e1))); }
        , s1 + sml::on_entry<e2>        / [this](e2 const&) { entries.push_back(std::type_index(typeid(e2))); }
        , s1 + sml::on_entry<_>         / [this](auto const& e) -> void { (void)e; entries.push_back(std::type_index(typeid(e))); }
      );
      // clang-format on
    }

    std::vector<std::type_index> entries;
  };

  sml::sm<c> sm;
  c& c_ = sm;
  expect(1 == c_.entries.size());
  expect(std::type_index(typeid(sml::anonymous)) == c_.entries[0]);
  c_.entries.clear();

  sm.process_event(e1{});
  expect(1 == c_.entries.size());
  expect(std::type_index(typeid(e1)) == c_.entries[0]);
  c_.entries.clear();

  sm.process_event(e2{});
  expect(1 == c_.entries.size());
  expect(std::type_index(typeid(e2)) == c_.entries[0]);
  c_.entries.clear();

  sm.process_event(e3{});
  expect(1 == c_.entries.size());
  expect(std::type_index(typeid(e3)) == c_.entries[0]);
  c_.entries.clear();

  sm.process_event(e4{});  // no on_entry should be called
  expect(0 == c_.entries.size());
};

test states_initial_entry_actions_with_events = [] {
  struct c {
    auto operator()() {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        (*idle) = s1,
          idle + sml::on_entry<initial> / [this](auto const&) -> void { entries.push_back(std::type_index(typeid(initial))); },
          idle + sml::on_exit<_>        / [this](auto const& e) -> void { (void)e; entries.push_back(std::type_index(typeid(e))); },
          s1   + sml::on_entry<_>       / [this](auto const& e) -> void { (void)e; entries.push_back(std::type_index(typeid(e))); },
          s1   + sml::on_exit<_>        / [this](auto const& e) -> void { (void)e; entries.push_back(std::type_index(typeid(e))); }
      );
      // clang-format on
    }

    std::vector<std::type_index> entries;
  };

  sml::sm<c> sm{};
  c& c_ = sm;
  expect(3 == c_.entries.size());
  expect(std::vector<std::type_index>{std::type_index(typeid(sml::initial)), std::type_index(typeid(sml::anonymous)),
                                      std::type_index(typeid(sml::anonymous))} == c_.entries);
};

test any_state = [] {
  struct c {
    auto operator()() {
      using namespace sml;
      auto action1 = [this]{ calls += "a1|"; };
      auto action2 = [this]{ calls += "a2|"; };
      auto action3 = [this]{ calls += "a3|"; };
      
      // clang-format off
      return make_transition_table(
        any + event<e1> / action1,
        any + event<e2> / action2,
        any + event<e3> / action3 = idle,

        *idle + event<e1> = s1,
        s1 + event<e2> = s2,
        s2 + event<e3> = s1
      );
      // clang-format on
    }

    std::string calls{};
  };

  sml::sm<c> sm{};
  const c& c_ = sm;

  sm.process_event(e1());
  expect(sm.is(s1));
  sm.process_event(e1());
  expect(sm.is(s1));
  sm.process_event(e2());
  expect(sm.is(s2));
  sm.process_event(e1());
  expect(sm.is(s2));
  sm.process_event(e2());
  expect(sm.is(s2));
  sm.process_event(e3());
  expect(sm.is(s1));
  sm.process_event(e3());
  expect(sm.is(idle));

  expect("a1|a1|a2|a3|" == c_.calls);
};

#if !defined(_MSC_VER)
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
