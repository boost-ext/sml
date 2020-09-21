//
// Copyright (c) 2020-2020 Uy Ha
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/sml.hpp>

using namespace boost::sml;

struct next {};

struct s1 {};
struct s2 {};
struct s3 {};
struct s4 {};

void action1_impl(int& i) { ++i; }
void action2_impl(int& i) noexcept { ++i; }

bool guard1_impl() { return true; }
bool guard2_impl() noexcept { return true; }

test wrap_free_functions = [] {
  struct free_functions {
    auto operator()() {
      auto const action1 = wrap(&action1_impl);
      auto const action2 = wrap(&action2_impl);
      auto const guard1 = wrap(&guard1_impl);
      auto const guard2 = wrap(&guard2_impl);

      return make_transition_table(*state<s1> + event<next>[guard1] / action1 = state<s2>,
                                   state<s2> + event<next>[guard2] / action2 = X);
    }
  };

  int i = 0;
  sm<free_functions> state_machine{i};

  expect(state_machine.is(state<s1>));
  state_machine.process_event(next{});
  expect(state_machine.is(state<s2>));
  expect(i == 1);
  state_machine.process_event(next{});
  expect(state_machine.is(X));
  expect(i == 2);
};

test wrap_member_functions = [] {
  struct member_functions {
    using self = member_functions;

    void action1_impl() {}
    void action2_impl() const {}
    void action3_impl() noexcept {}
    void action4_impl() const noexcept {}

    bool guard1_impl() { return true; }
    bool guard2_impl() const { return true; }
    bool guard3_impl() noexcept { return true; }
    bool guard4_impl() const noexcept { return true; }

    auto operator()() {
      auto const action1 = wrap(&self::action1_impl);
      auto const action2 = wrap(&self::action2_impl);
      auto const action3 = wrap(&self::action3_impl);
      auto const action4 = wrap(&self::action4_impl);

      auto const guard1 = wrap(&self::guard1_impl);
      auto const guard2 = wrap(&self::guard2_impl);
      auto const guard3 = wrap(&self::guard3_impl);
      auto const guard4 = wrap(&self::guard4_impl);
      return make_transition_table(
          *state<s1> + event<next>[guard1] / action1 = state<s2>, state<s2> + event<next>[guard2] / action2 = state<s3>,
          state<s3> + event<next>[guard3] / action3 = state<s4>, state<s4> + event<next>[guard4] / action4 = X);
    }
  };

  member_functions mf{};
  sm<member_functions> state_machine{mf};
  expect(state_machine.is(state<s1>));
  state_machine.process_event(next{});
  expect(state_machine.is(state<s2>));
  state_machine.process_event(next{});
  expect(state_machine.is(state<s3>));
  state_machine.process_event(next{});
  expect(state_machine.is(state<s4>));
  state_machine.process_event(next{});
  expect(state_machine.is(X));
};