//
// Created by uyha on 21.09.2020.
//

#include <boost/sml.hpp>

using namespace boost::sml;

struct next {};

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

      return make_transition_table(*"s1"_s + event<next>[guard1] / action1 = "s2"_s,
                                   "s2"_s + event<next>[guard2] / action2 = X);
    }
  };

  int i = 0;
  sm<free_functions> state_machine{i};

  expect(state_machine.is("s1"_s));
  state_machine.process_event(next{});
  expect(state_machine.is("s2"_s));
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
      return make_transition_table(*"s1"_s + event<next>[guard1] / action1 = "s2"_s,
                                   "s2"_s + event<next>[guard2] / action2 = "s3"_s,
                                   "s3"_s + event<next>[guard3] / action3 = "s4"_s, "s4"_s + event<next>[guard4] / action4 = X);
    }
  };

  member_functions mf{};
  sm<member_functions> state_machine{mf};
  expect(state_machine.is("s1"_s));
  state_machine.process_event(next{});
  expect(state_machine.is("s2"_s));
  state_machine.process_event(next{});
  expect(state_machine.is("s3"_s));
  state_machine.process_event(next{});
  expect(state_machine.is("s4"_s));
  state_machine.process_event(next{});
  expect(state_machine.is(X));
};