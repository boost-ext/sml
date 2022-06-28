//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <cstdio>
#include <string>
#include <utility>
#include <vector>

namespace sml = boost::sml;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};
struct e6 {};

const auto idle = sml::state<class idle>;
const auto idle2 = sml::state<class idle2>;
const auto s1 = sml::state<class s1>;
const auto s2 = sml::state<class s2>;

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
    auto operator()() noexcept {
      using namespace sml;

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
    auto operator()() noexcept {
      using namespace sml;

      // clang-format off
      return make_transition_table(
         *idle + event<e1> [(guard2{})] / [this] { a_initial = true; } = s1
        , s1 + event<e2> [guard]  / [this]{ a_enter_sub_sm = true; } = state<sub>
        , state<sub> + sml::on_entry<_> / [this] { a_on_entry_sub_sm = true; }
        , state<sub> + sml::on_exit<_> / [this] { a_on_exit_sub_sm = true; }
        , state<sub> + event<e5> [(guard2{})] / [this] { a_exit_sub_sm = true; } = s2
      );
      // clang-format on
    }

    bool a_initial = false;
    bool a_enter_sub_sm = false;
    bool a_exit_sub_sm = false;
    bool a_on_exit_sub_sm = false;
    bool a_on_entry_sub_sm = false;
  };

  sml::sm<c> sm{87.0, 42};

  c& c_ = sm;
  sub& sub_ = sm;

  expect(sm.is(idle));
  sm.process_event(e1());
  expect(c_.a_initial);

  sm.process_event(e2());
  expect(c_.a_enter_sub_sm);
  expect(c_.a_on_entry_sub_sm);
  expect(0 == sub_.a_in_sub);

  sm.process_event(e3());
  expect(1 == sub_.a_in_sub);

  sm.process_event(e4());
  expect(2 == sub_.a_in_sub);
  sm.process_event(e5());
  expect(2 == sub_.a_in_sub);
  expect(c_.a_on_entry_sub_sm);
  expect(c_.a_exit_sub_sm);
  expect(sm.is(s2));
};

test composite_def_ctor = [] {
  static auto in_sub = 0;

  struct sub {
    auto operator()() noexcept {
      using namespace sml;

      // clang-format off
      return make_transition_table(
         *idle + event<e3> / [] { in_sub++; } = s1
        , s1 + event<e4> / [] { in_sub++; } = s2
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() noexcept {
      using namespace sml;

      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> = state<sub>
        , state<sub> + event<e5> = s2
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;

  expect(sm.is(idle));
  sm.process_event(e1());

  sm.process_event(e2());
  expect(0 == in_sub);

  sm.process_event(e3());
  expect(1 == in_sub);

  sm.process_event(e4());
  expect(2 == in_sub);
  sm.process_event(e5());
  expect(2 == in_sub);
  expect(sm.is(s2));
};

test composite_def_ctor_passed = [] {
  struct sub {
    auto operator()() const {
      using namespace sml;
      return make_transition_table(*idle = X);
    }
  };

  struct c {
    auto operator()() const {
      using namespace sml;

      // clang-format off
      return make_transition_table(
        *idle = state<sub>
      );
      // clang-format on
    }
  };

  sub s;
  sml::sm<c> sm{s};
  expect(sm.is(sml::state<sub>));
};

test composite_custom_ctor = [] {
  static auto in_sub = 0;

  struct sub {
    explicit sub(int i) : i(i) {}

    auto operator()() noexcept {
      using namespace sml;

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
    auto operator()() noexcept {
      using namespace sml;

      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> = state<sub>
        , state<sub> + event<e5> = s2
      );
      // clang-format on
    }
  };

  constexpr auto i = 2;

  {
    in_sub = 0;
    sml::sm<c> sm{sub{i}};
    expect(sm.is(idle));
    sm.process_event(e1());

    sm.process_event(e2());
    expect(0 == in_sub);

    sm.process_event(e3());
    expect(1 * i == in_sub);

    sm.process_event(e4());
    expect(2 * i == in_sub);
    sm.process_event(e5());
    expect(2 * i == in_sub);
    expect(sm.is(s2));
  }

  {
    in_sub = 0;
    sml::sm<c> sm{sub{i}};
    expect(sm.is(idle));
    sm.process_event(e1());

    sm.process_event(e2());
    expect(0 == in_sub);

    sm.process_event(e3());
    expect(1 * i == in_sub);

    sm.process_event(e4());
    expect(2 * i == in_sub);
    sm.process_event(e5());
    expect(2 * i == in_sub);
    expect(sm.is(s2));
  }

  {
    in_sub = 0;
    sml::sm<c> sm{sub{i}};
    expect(sm.is(idle));
    sm.process_event(e1());

    sm.process_event(e2());
    expect(0 == in_sub);

    sm.process_event(e3());
    expect(1 * i == in_sub);

    sm.process_event(e4());
    expect(2 * i == in_sub);
    sm.process_event(e5());
    expect(2 * i == in_sub);
    expect(sm.is(s2));
  }
};

test composite_is = [] {
  struct sub {
    auto operator()() noexcept {
      using namespace sml;

      // clang-format off
      return make_transition_table(
        *s1 + event<e2> = X
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() noexcept {
      using namespace sml;

      // clang-format off
      return make_transition_table(
        *idle + event<e1> = state<sub>
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  expect(sm.is(idle));
  expect(sm.is<decltype(sml::state<sub>)>(s1));

  sm.process_event(e1{});
  expect(sm.is(sml::state<sub>));
  expect(sm.is<decltype(sml::state<sub>)>(s1));

  sm.process_event(e2{});
  expect(sm.is(sml::state<sub>));
  expect(sm.is<decltype(sml::state<sub>)>(sml::X));
};

test composite_entry_exit_initial = [] {
  struct sub {
    auto operator()() noexcept {
      using namespace sml;

      // clang-format off
      return make_transition_table(
       *idle + sml::on_entry<_> / [this] { ++entry_calls; },
        idle + sml::on_exit<_> / [this] { ++exit_calls; }
      );
      // clang-format on
    }

    int entry_calls = 0;
    int exit_calls = 0;
  };

  struct c {
    auto operator()() noexcept {
      using namespace sml;

      // clang-format off
      return make_transition_table(
       *state<sub> + sml::on_entry<_> / [this] { ++entry_calls; },
        state<sub> + sml::on_exit<_> / [this] { ++exit_calls; }
      );
      // clang-format on
    }

    int entry_calls = 0;
    int exit_calls = 0;
  };

  sml::sm<c> sm;
  const sub& sub_ = sm;
  const c& c_ = sm;
  expect(1 == c_.entry_calls);
  expect(0 == c_.exit_calls);
  expect(1 == sub_.entry_calls);
  expect(0 == sub_.exit_calls);
};

test composite_entry_exit_initial_complex = [] {
  enum class calls {
    SS1_1_entry,
    SS1_1_exit,
    SS1_2_entry,
    SS1_2_exit,
    SS2_1_entry,
    SS2_1_exit,
    SS2_2_entry,
    SS2_2_exit,
    SS1_entry,
    SS1_exit,
    SS2_entry,
    SS2_exit,
  };

  static auto ss1_1 = sml::state<class SubSubState1_1>;
  static auto ss1_2 = sml::state<class SubSubState1_2>;
  static auto ss2_1 = sml::state<class SubSubState2_1>;
  static auto ss2_2 = sml::state<class SubSubState2_2>;

  struct SubState1 {
    auto operator()() const noexcept {
      using namespace sml;

      // clang-format off
      return make_transition_table(
        *ss1_1 + event<e1> = ss1_2,
         ss1_1 + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::SS1_1_entry); },
         ss1_2 + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::SS1_2_entry); },
         ss1_1 + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::SS1_1_exit); },
         ss1_2 + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::SS1_2_exit); }
      );
      // clang-format on
    }
  };

  struct SubState2 {
    auto operator()() const noexcept {
      using namespace sml;

      // clang-format off
      return make_transition_table(
        *ss2_1 + event<e1> = ss2_1,
         ss2_1 + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::SS2_1_entry); },
         ss2_2 + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::SS2_2_entry); },
         ss2_1 + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::SS2_1_exit); },
         ss2_2 + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::SS2_2_exit); }
      );
      // clang-format on
    }
  };

  static const auto ss1 = sml::state<SubState1>;
  static const auto ss2 = sml::state<SubState2>;

  struct TopState {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *ss1 + event<e2> = ss2,
         ss1 + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::SS1_entry); },
         ss2 + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::SS2_entry); },
         ss1 + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::SS1_exit); },
         ss2 + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::SS2_exit); }
      );
      // clang-format on
    }
  };

  std::vector<calls> c_;
  sml::sm<TopState> sm{c_};
  expect(std::vector<calls>{calls::SS1_entry, calls::SS1_1_entry} == c_);
  expect(sm.is(ss1));

  c_.clear();
  sm.process_event(e2{});
  expect(std::vector<calls>{calls::SS1_1_exit, calls::SS1_exit, calls::SS2_entry, calls::SS2_1_entry} == c_);
  expect(sm.is(ss2));
};

test composite_transition_the_same_event = [] {
  struct sub {
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

  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = state<sub>
        , state<sub> + event<e3> = s1
        , s1 + event<e4> = state<sub>
        , state<sub> + event<e5> = X
      );
      // clang-format on
    }
  };

  using namespace sml;

  c c_;
  sub sub_;
  sml::sm<c> sm{c_, sub_};

  expect(sm.is(idle));
  sm.process_event(e1());
  expect(sm.is(state<sub>));
  expect(sm.is<decltype(state<sub>)>(idle));

  sm.process_event(e4());

  sm.process_event(e1());
  expect(sm.is(state<sub>));
  expect(sm.is<decltype(state<sub>)>(s1));

  sm.process_event(e2());
  expect(sm.is(state<sub>));
  expect(sm.is<decltype(state<sub>)>(s2));

  sm.process_event(e3());
  expect(sm.is(s1));
  expect(sm.is<decltype(state<sub>)>(s2));

  sm.process_event(e4());
  expect(sm.is(state<sub>));
  expect(sm.is<decltype(state<sub>)>(idle));  // no history
};

test composite_with_orthogonal_regions = [] {
  struct sub {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e4> = s1
        , s1 + event<e4> = s2
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> = X
        // ----------------------------------
        ,*idle2 + event<e1> = state<sub>
        , state<sub> + event<e3> = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  expect(sm.is(idle, idle2));

  sm.process_event(e1());
  expect(sm.is(s1, sml::state<sub>));
  expect(sm.is<decltype(sml::state<sub>)>(idle));

  sm.process_event(e4());
  expect(sm.is<decltype(sml::state<sub>)>(s1));

  sm.process_event(e4());
  expect(sm.is<decltype(sml::state<sub>)>(s2));

  sm.process_event(e2());
  sm.process_event(e3());
  expect(sm.is<decltype(sml::state<sub>)>(s2));
  expect(sm.is(sml::X, sml::X));
};

test composite_sub_entry_exit = [] {
  enum class calls { a1_entry, a1_exit, b1_entry, b1_exit };

  struct A {
    auto operator()() const noexcept {
      using namespace sml;
      const auto a1 = state<class A1>;
      const auto a2 = state<class A2>;
      // clang-format off
      return make_transition_table(
        *a1 + event<e2> = a2,
         a1 + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::a1_entry); },
         a1 + sml::on_exit<_> / [](std::vector<calls>& c) { c.push_back(calls::a1_exit); }
      );
      // clang-format on
    }
  };

  struct B {
    auto operator()() const noexcept {
      using namespace sml;
      const auto b1 = state<class B1>;
      const auto b2 = state<class B2>;
      // clang-format off
      return make_transition_table(
        *b1 + event<e2> = b2,
         b1 + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::b1_entry); },
         b1 + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::b1_exit); }
      );
      // clang-format on
    }
  };

  struct SM {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *state<class init> = state<A>,
         state<A>  + event<e1> = state<B>,
         state<B>  + event<e3> = X
      );
      // clang-format on
    }
  };

  std::vector<calls> c_;
  sml::sm<SM> sm{c_};
  expect(std::vector<calls>{calls::a1_entry} == c_);
  sm.process_event(e1{});
  expect(std::vector<calls>{calls::a1_entry, calls::a1_exit, calls::b1_entry} == c_);
};

test composite_anonymous_entry_transitions = [] {
  enum class calls { a1_entry, a1_exit, b1_entry, b1_exit, x_entry, x_exit, a_entry, a_exit, b_entry, b_exit };

  struct A {
    auto operator()() const noexcept {
      using namespace sml;
      const auto a1 = state<class A1>;

      // clang-format off
      return make_transition_table(
         *idle = a1,
          a1 + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::a1_entry); },
          a1 + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::a1_exit); }
      );
      // clang-format on
    }
  };

  struct B {
    auto operator()() const noexcept {
      using namespace sml;
      const auto b1 = state<class A1>;

      // clang-format off
      return make_transition_table(
         *idle = b1,
          b1 + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::b1_entry); },
          b1 + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::b1_exit); }
      );
      // clang-format on
    }
  };

  static const auto x = sml::state<class X>;
  static const auto a = sml::state<A>;
  static const auto b = sml::state<B>;

  struct Top {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *x + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::x_entry); },
         x + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::x_exit); },
         a + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::a_entry); },
         a + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::a_exit); },
         b + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::b_entry); },
         b + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::b_exit); }
      );
      // clang-format on
    }
  };

  std::vector<calls> c_;
  sml::sm<Top> sm{c_};
  expect(std::vector<calls>{calls::x_entry} == c_);
  expect(sm.is(x));
};

test composite_entry_exit_sub_sm = [] {
  enum class calls {
    ls1_1_entry,
    ls1_1_exit,
    ls1_2_entry,
    ls1_2_exit,
    ls2_1_entry,
    ls2_1_exit,
    ls2_2_entry,
    ls2_2_exit,
    sub1_entry,
    sub1_exit,
    sub2_entry,
    sub2_exit
  };

  struct sub_1 {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *state<class ls1_1> + event<e1> = state<class ls1_2>
       , state<class ls1_2> + event<e2> = state<class ls1_1>
       , state<class ls1_1> + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::ls1_1_entry); }
       , state<class ls1_1> + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::ls1_1_exit); }
       , state<class ls1_2> + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::ls1_2_entry); }
       , state<class ls1_2> + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::ls1_2_exit); }
      );
      // clang-format on
    }
  };

  struct sub_2 {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *state<class ls2_1> + event<e1> = state<class ls2_2>
       , state<class ls2_2> + event<e2> = state<class ls2_1>
       , state<class ls2_1> + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::ls2_1_entry); }
       , state<class ls2_1> + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::ls2_1_exit); }
       , state<class ls2_2> + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::ls2_2_entry); }
       , state<class ls2_2> + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::ls2_2_exit); }
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *state<sub_1> + event<e3> = state<sub_2>
       , state<sub_2> + event<e4> = state<sub_1>
       , state<sub_1> + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::sub1_entry); }
       , state<sub_1> + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::sub1_exit); }
       , state<sub_2> + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::sub2_entry); }
       , state<sub_2> + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::sub2_exit); }
      );
      // clang-format on
    }
  };

  std::vector<calls> c_;
  sml::sm<c> sm{c_};
  expect(std::vector<calls>{calls::sub1_entry, calls::ls1_1_entry} == c_);

  c_.clear();
  sm.process_event(e1{});
  expect(std::vector<calls>{calls::ls1_1_exit, calls::ls1_2_entry} == c_);

  c_.clear();
  sm.process_event(e2{});
  expect(std::vector<calls>{calls::ls1_2_exit, calls::ls1_1_entry} == c_);

  c_.clear();
  sm.process_event(e3{});
  expect(std::vector<calls>{calls::ls1_1_exit, calls::sub1_exit, calls::sub2_entry, calls::ls2_1_entry} == c_);

  c_.clear();
  sm.process_event(e4{});
  expect(std::vector<calls>{calls::ls2_1_exit, calls::sub2_exit, calls::sub1_entry, calls::ls1_1_entry} == c_);
};

test composite_entry_exit_sub_sm_only_dst = [] {
  using namespace sml;

  struct inner {
    auto operator()() noexcept {
      // clang-format off
      return make_transition_table(
        *state<struct inner1> + on_entry<_> / [](std::vector<bool>& entered) { entered.push_back(true); }
      );
      // clang-format on
    }
  };

  struct outer {
    auto operator()() noexcept {
      // clang-format off
      return make_transition_table(
        *state<struct outer1> + event<e1> = state<inner>
      );
      // clang-format on
    }
  };

  std::vector<bool> entered = {};
  sm<outer> sm{entered};
  sm.process_event(e1{});
  expect(std::vector<bool>{true} == entered);
};

test composite_anonymous_transition_from_outer_state = [] {
  struct sub {
    auto operator()() const noexcept {
      using namespace sml;
      return make_transition_table(*idle + event<e1> = X);
    }
  };

  struct c {
    auto operator()() const noexcept {
      using namespace sml;
      return make_transition_table(*state<sub> = s1);
    }
  };

  sml::sm<c> sm;
  expect(sm.is(sml::state<sub>));
  expect(sm.is<decltype(sml::state<sub>)>(idle));
  sm.process_event(e1{});
  expect(sm.is(s1));
  expect(sm.is<decltype(sml::state<sub>)>(sml::X));
};

test composite_anonymous_transition_from_outer_state_orthogonal_regions = [] {
  struct sub {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *idle + event<e1> = X,
        *idle2 + event<e2> = X
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() const noexcept {
      using namespace sml;
      return make_transition_table(*state<sub> = s1);
    }
  };

  {
    sml::sm<c> sm;
    expect(sm.is(sml::state<sub>));
    expect(sm.is<decltype(sml::state<sub>)>(idle));
    sm.process_event(e1{});
    expect(sm.is(sml::state<sub>));
    expect(sm.is<decltype(sml::state<sub>)>(sml::X, idle2));
    expect(sm.is<decltype(sml::state<sub>)>(sml::X));
  }

  {
    sml::sm<c> sm;
    expect(sm.is(sml::state<sub>));
    expect(sm.is<decltype(sml::state<sub>)>(idle));
    sm.process_event(e2{});
    expect(sm.is(sml::state<sub>));
    expect(sm.is<decltype(sml::state<sub>)>(idle, sml::X));
  }

  {
    sml::sm<c> sm;
    expect(sm.is(sml::state<sub>));
    expect(sm.is<decltype(sml::state<sub>)>(idle));
    sm.process_event(e1{});
    sm.process_event(e2{});
    expect(sm.is(s1));
    expect(sm.is<decltype(sml::state<sub>)>(sml::X, sml::X));
    expect(sm.is<decltype(sml::state<sub>)>(sml::X));
  }
};

test composite_with_names = [] {
  struct a;
  struct b;

  struct sub {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *idle + event<e1> = X
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *state<a>.sm<sub>() + event<e2> = state<b>.sm<sub>(),
         state<b>.sm<sub>() + event<e3> = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  expect(sm.is(sml::state<a>.sm<sub>()));
  expect(sm.is<decltype(sml::state<a>.sm<sub>())>(idle));
  expect(sm.is<decltype(sml::state<b>.sm<sub>())>(idle));

  sm.process_event(e1{});
  expect(sm.is(sml::state<a>.sm<sub>()));
  expect(sm.is<decltype(sml::state<a>.sm<sub>())>(sml::X));
  expect(sm.is<decltype(sml::state<b>.sm<sub>())>(idle));

  sm.process_event(e2{});
  expect(sm.is(sml::state<b>.sm<sub>()));
  expect(sm.is<decltype(sml::state<a>.sm<sub>())>(sml::X));
  expect(sm.is<decltype(sml::state<b>.sm<sub>())>(idle));

  sm.process_event(e1{});
  expect(sm.is(sml::state<b>.sm<sub>()));
  expect(sm.is<decltype(sml::state<a>.sm<sub>())>(sml::X));
  expect(sm.is<decltype(sml::state<b>.sm<sub>())>(sml::X));

  sm.process_event(e3{});
  expect(sm.is(sml::X));
};

test composite_sub_guards = [] {
  enum class calls { a1_entry, a1_exit, a2_entry, a2_exit, b1_entry, b1_exit };

  struct A {
    auto operator()() const noexcept {
      using namespace sml;
      const auto a1 = state<class A1>;
      const auto a2 = state<class A2>;
      // clang-format off
      return make_transition_table(
        (*a1) [([](int& guard_counter, bool& is_ok) {guard_counter++; return is_ok;})] = a2,
         a1 + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::a1_entry); },
         a1 + sml::on_exit<_> / [](std::vector<calls>& c) { c.push_back(calls::a1_exit); },
         a2 + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::a2_entry); },
         a2 + sml::on_exit<_> / [](std::vector<calls>& c) { c.push_back(calls::a2_exit); }
      );
      // clang-format on
    }
  };

  struct B {
    auto operator()() const noexcept {
      using namespace sml;
      const auto b1 = state<class B1>;
      const auto b2 = state<class B2>;
      // clang-format off
      return make_transition_table(
        *b1 + event<e2> = b2,
         b1 + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::b1_entry); },
         b1 + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::b1_exit); }
      );
      // clang-format on
    }
  };

  struct SM {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *state<class init> = state<A>,
         state<A>  + event<e1> = state<B>,
         state<B>  + event<e3> = X
      );
      // clang-format on
    }
  };

  std::vector<calls> c_;
  int guard_counter = 0;
  bool is_ok = false;
  sml::sm<SM> sm{c_, guard_counter, is_ok};
  expect(std::vector<calls>{calls::a1_entry} == c_);
  expect(guard_counter == 1);
  guard_counter = {};
  sm.process_event(e2{});
  expect(guard_counter == 1);
  guard_counter = {};
  is_ok = true;
  sm.process_event(e2{});
  expect(guard_counter == 1);
  expect(std::vector<calls>{calls::a1_entry, calls::a1_exit, calls::a2_entry} == c_);
  guard_counter = {};
  sm.process_event(e1{});
  expect(guard_counter == 0);
  expect(std::vector<calls>{calls::a1_entry, calls::a1_exit, calls::a2_entry, calls::a2_exit, calls::b1_entry} == c_);
};

#if !defined(_MSC_VER)
test composite_with_string_names = [] {
  struct sub {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *idle + event<e1> = X
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *"state a"_s.sm<sub>() + event<e2> = "state b"_s.sm<sub>(),
         "state b"_s.sm<sub>() + event<e3> = X
      );
      // clang-format on
    }
  };

  using namespace sml;
  sm<c> sm;
  expect(sm.is("state a"_s.sm<sub>()));
  expect(sm.is<decltype("state a"_s.sm<sub>())>(idle));
  expect(sm.is<decltype("state b"_s.sm<sub>())>(idle));

  sm.process_event(e1{});
  expect(sm.is("state a"_s.sm<sub>()));
  expect(sm.is<decltype("state a"_s.sm<sub>())>(X));
  expect(sm.is<decltype("state b"_s.sm<sub>())>(idle));

  sm.process_event(e2{});
  expect(sm.is("state b"_s.sm<sub>()));
  expect(sm.is<decltype("state a"_s.sm<sub>())>(X));
  expect(sm.is<decltype("state b"_s.sm<sub>())>(idle));

  sm.process_event(e1{});
  expect(sm.is("state b"_s.sm<sub>()));
  expect(sm.is<decltype("state a"_s.sm<sub>())>(X));
  expect(sm.is<decltype("state b"_s.sm<sub>())>(X));

  sm.process_event(e3{});
  expect(sm.is(X));
};
#endif
