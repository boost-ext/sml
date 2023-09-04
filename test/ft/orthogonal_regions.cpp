//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
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
const auto idle3 = sml::state<class idle3>;
const auto s1 = sml::state<class s1>;
const auto s2 = sml::state<class s2>;
const auto s3 = sml::state<class s3>;
const auto s4 = sml::state<class s4>;

test orthogonal_regions = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;

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

  sml::sm<c> sm;
  expect(sm.is(idle, idle2));
  expect(sm.is(idle));
  expect(sm.is(idle2));

  sm.process_event(e1{});
  expect(sm.is(s1, idle2));
  expect(sm.is(s1));
  expect(sm.is(idle2));

  sm.process_event(e2{});
  expect(sm.is(s2, idle2));
  expect(sm.is(s2));
  expect(sm.is(idle2));

  sm.process_event(e3{});
  expect(sm.is(s2, s3));
  expect(sm.is(s2));
  expect(sm.is(s3));

  sm.process_event(e4{});
  expect(sm.is(s2, s4));
  expect(sm.is(s2));
  expect(sm.is(s4));
};

test orthogonal_regions_event_consumed_by_all_regions = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;

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

  sml::sm<c> sm;
  expect(sm.is(idle, idle2));
  sm.process_event(e1{});
  expect(sm.is(s1, idle2));
  sm.process_event(e3{});
  expect(sm.is(s1, s3));
  sm.process_event(e2{});  // consume by both regions
  expect(sm.is(s2, s4));
};

test orthogonal_regions_initial_entry = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off

      return make_transition_table(
          *idle + sml::on_entry<_> / [this] { ++entry_1; }
        , *idle2 + sml::on_entry<_> / [this] { ++entry_2; }
      );
      // clang-format on
    }

    int entry_1 = 0;
    int entry_2 = 0;
  };

  sml::sm<c> sm{c{}};
  const c& c_ = sm;
  expect(1 == c_.entry_1);
  expect(1 == c_.entry_2);
};

test orthogonal_regions_entry_exit_multiple = [] {
  enum class calls { a_entry, a_exit, b_entry, b_exit, c_entry, c_exit };
  struct c {
    auto operator()() const noexcept {
      using namespace sml;
      const auto a = state<class A>;
      const auto b = state<class B>;
      const auto c = state<class C>;

      // clang-format off
      return make_transition_table(
        *idle = a,
        *idle2 = b,
        *idle3 = c,
        a + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::a_entry); },
        a + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::a_exit); },
        b + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::b_entry); },
        b + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::b_exit); },
        c + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::c_entry); },
        c + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::c_exit); },
        a + event<e1> = X,
        b + event<e1> = X,
        c + event<e1> = X
      );
      // clang-format on
    }
  };

  std::vector<calls> c_;
  sml::sm<c> sm{c_};
  expect(std::vector<calls>{calls::a_entry, calls::b_entry, calls::c_entry} == c_);

  c_.clear();
  sm.process_event(e1{});
  expect(std::vector<calls>{calls::a_exit, calls::b_exit, calls::c_exit} == c_);
  expect(sm.is(sml::X, sml::X, sml::X));
};

test orthogonal_regions_entry_exit = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      auto entry_action = [this] { ++a_entry_action; };
      auto exit_action = [this] { ++a_exit_action; };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + sml::on_entry<_> / entry_action
        , s1 + sml::on_exit<_> / exit_action
        , s1 + event<e2> = s2

        ,*idle2 + event<e3> = s3
        , s3 + event<e2> = s4
      );
      // clang-format on
    }

    int a_entry_action = 0;
    int a_exit_action = 0;
  };

  sml::sm<c> sm{};
  c& c_ = sm;
  sm.process_event(e1{});
  expect(c_.a_entry_action == 1);
  expect(c_.a_exit_action == 0);
  sm.process_event(e3{});
  expect(c_.a_entry_action == 1);
  expect(c_.a_exit_action == 0);
  sm.process_event(e2{});
  expect(c_.a_entry_action == 1);
  expect(c_.a_exit_action == 1);
};

test orthogonal_regions_reentries = [] {
  enum class calls {
    a1_entry,
    a1_exit,
    a2_entry,
    a2_exit,
    b1_entry,
    b1_exit,
    b2_entry,
    b2_exit,
    c1_entry,
    c1_exit,
    c2_entry,
    c2_exit,
    a_entry,
    a_exit,
    b_entry,
    b_exit,
    c_entry,
    c_exit,
    x_entry,
    x_exit,
    y_entry,
    y_exit,
    z_entry,
    z_exit
  };

  struct A {
    auto operator()() const noexcept {
      using namespace sml;
      const auto a1 = state<class A1>;
      const auto a2 = state<class A2>;
      // clang-format off
      return make_transition_table(
        *a1 + event<e4> = a2,
         a1 + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::a1_entry); },
         a1 + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::a1_exit); },
         a2 + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::a2_entry); },
         a2 + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::a2_exit); }
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
        *b1 + event<e4> = b2,
         b1 + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::b1_entry); },
         b1 + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::b1_exit); },
         b2 + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::b2_entry); },
         b2 + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::b2_exit); }
      );
      // clang-format on
    }
  };

  struct C {
    auto operator()() const noexcept {
      using namespace sml;
      const auto c1 = state<class C1>;
      const auto c2 = state<class C2>;
      // clang-format off
      return make_transition_table(
        *c1 + event<e4> = c2,
         c1 + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::c1_entry); },
         c1 + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::c1_exit); },
         c2 + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::c2_entry); },
         c2 + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::c2_exit); }
      );
      // clang-format on
    }
  };

  struct D {
    auto operator()() const noexcept {
      using namespace sml;
      const auto a = state<A>;
      const auto x = state<class X>;
      const auto b = state<B>;
      const auto y = state<class Y>;
      const auto c = state<C>;
      const auto z = state<class Y>;
      // clang-format off
      return make_transition_table(
        *idle = a,
          a + event<e1> = x,
         *idle2 = b,
          b + event<e2> = y,
         *idle3 = c,
          c + event<e3> = z,
          a + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::a_entry); },
          a + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::a_exit); },
          x + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::x_entry); },
          x + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::x_exit); },
          b + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::b_entry); },
          b + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::b_exit); },
          y + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::y_entry); },
          y + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::y_exit); },
          c + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::c_entry); },
          c + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::c_exit); },
          z + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::z_entry); },
          z + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::z_exit); }
      );
      // clang-format on
    }
  };

  std::vector<calls> c_;
  sml::sm<D> sm{c_};
  expect(std::vector<calls>{calls::a_entry, calls::a1_entry, calls::b_entry, calls::b1_entry, calls::c_entry,
                            calls::c1_entry} == c_);

  c_.clear();
  sm.process_event(e2{});
  expect(std::vector<calls>{calls::b1_exit, calls::b_exit, calls::y_entry} == c_);
};
