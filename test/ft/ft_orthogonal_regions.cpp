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
#include <vector>

namespace msm = boost::msm::lite;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};
struct e6 {};

const auto idle = msm::state<class idle>;
const auto idle2 = msm::state<class idle2>;
const auto s1 = msm::state<class s1>;
const auto s2 = msm::state<class s2>;
const auto s3 = msm::state<class s3>;
const auto s4 = msm::state<class s4>;

test orthogonal_regions = [] {
  struct c {
    auto operator()() noexcept {
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
  sm.process_event(e1{});
  expect(sm.is(s1, idle2));
  sm.process_event(e2{});
  expect(sm.is(s2, idle2));
  sm.process_event(e3{});
  expect(sm.is(s2, s3));
  sm.process_event(e4{});
  expect(sm.is(s2, s4));
};

test orthogonal_regions_event_consumed_by_all_regions = [] {
  struct c {
    auto operator()() noexcept {
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
      using namespace msm;
      // clang-format off

      return make_transition_table(
          *idle + msm::on_entry / [this] { ++entry_1; }
        , *idle2 + msm::on_entry / [this] { ++entry_2; }
      );
      // clang-format on
    }

    int entry_1 = 0;
    int entry_2 = 0;
  };

  c c_;
  msm::sm<c> sm{c_};
  expect(1 == c_.entry_1);
  expect(1 == c_.entry_2);
};

test orthogonal_regions_entry_exit_multiple = [] {
  enum class calls { a_entry, a_exit, b_entry, b_exit, c_entry, c_exit };
  struct c {
    auto operator()() const noexcept {
      using namespace msm;
      const auto a = state<class A>;
      const auto b = state<class B>;
      const auto c = state<class C>;

      // clang-format off
      return make_transition_table(
        *"init1"_s = a,
        *"init2"_s = b,
        *"init3"_s = c,
        a + msm::on_entry / [](std::vector<calls>& c) { c.push_back(calls::a_entry); },
        a + msm::on_exit  / [](std::vector<calls>& c) { c.push_back(calls::a_exit); },
        b + msm::on_entry / [](std::vector<calls>& c) { c.push_back(calls::b_entry); },
        b + msm::on_exit  / [](std::vector<calls>& c) { c.push_back(calls::b_exit); },
        c + msm::on_entry / [](std::vector<calls>& c) { c.push_back(calls::c_entry); },
        c + msm::on_exit  / [](std::vector<calls>& c) { c.push_back(calls::c_exit); },
        a + event<e1> = X,
        b + event<e1> = X,
        c + event<e1> = X
      );
      // clang-format on
    }
  };

  std::vector<calls> c_;
  msm::sm<c> sm{c_};
  expect(std::vector<calls>{calls::a_entry, calls::b_entry, calls::c_entry} == c_);

  c_.clear();
  sm.process_event(e1{});
  expect(std::vector<calls>{calls::a_exit, calls::b_exit, calls::c_exit} == c_);
  expect(sm.is(msm::X, msm::X, msm::X));
};

test orthogonal_regions_entry_exit = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      auto entry_action = [this] { ++a_entry_action; };
      auto exit_action = [this] { ++a_exit_action; };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + msm::on_entry / entry_action
        , s1 + msm::on_exit / exit_action
        , s1 + event<e2> = s2

        ,*idle2 + event<e3> = s3
        , s3 + event<e2> = s4
      );
      // clang-format on
    }

    int a_entry_action = 0;
    int a_exit_action = 0;
  };

  c c_;
  msm::sm<c> sm{c_};
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
      using namespace msm;
      const auto a1 = state<class A1>;
      const auto a2 = state<class A2>;
      // clang-format off
      return make_transition_table(
        *a1 + event<e4> = a2,
         a1 + msm::on_entry / [](std::vector<calls>& c) { c.push_back(calls::a1_entry); },
         a1 + msm::on_exit  / [](std::vector<calls>& c) { c.push_back(calls::a1_exit); },
         a2 + msm::on_entry / [](std::vector<calls>& c) { c.push_back(calls::a2_entry); },
         a2 + msm::on_exit  / [](std::vector<calls>& c) { c.push_back(calls::a2_exit); }
      );
      // clang-format on
    }
  };

  struct B {
    auto operator()() const noexcept {
      using namespace msm;
      const auto b1 = state<class B1>;
      const auto b2 = state<class B2>;
      // clang-format off
      return make_transition_table(
        *b1 + event<e4> = b2,
         b1 + msm::on_entry / [](std::vector<calls>& c) { c.push_back(calls::b1_entry); },
         b1 + msm::on_exit  / [](std::vector<calls>& c) { c.push_back(calls::b1_exit); },
         b2 + msm::on_entry / [](std::vector<calls>& c) { c.push_back(calls::b2_entry); },
         b2 + msm::on_exit  / [](std::vector<calls>& c) { c.push_back(calls::b2_exit); }
      );
      // clang-format on
    }
  };

  struct C {
    auto operator()() const noexcept {
      using namespace msm;
      const auto c1 = state<class C1>;
      const auto c2 = state<class C2>;
      // clang-format off
      return make_transition_table(
        *c1 + event<e4> = c2,
         c1 + msm::on_entry / [](std::vector<calls>& c) { c.push_back(calls::c1_entry); },
         c1 + msm::on_exit  / [](std::vector<calls>& c) { c.push_back(calls::c1_exit); },
         c2 + msm::on_entry / [](std::vector<calls>& c) { c.push_back(calls::c2_entry); },
         c2 + msm::on_exit  / [](std::vector<calls>& c) { c.push_back(calls::c2_exit); }
      );
      // clang-format on
    }
  };

  struct D {
    auto operator()() const noexcept {
      using namespace msm;
      const auto a = state<A>;
      const auto x = state<class X>;
      const auto b = state<B>;
      const auto y = state<class Y>;
      const auto c = state<C>;
      const auto z = state<class Y>;
      // clang-format off
      return make_transition_table(
        *"init1"_s = a,
          a + event<e1> = x,
          *"init2"_s = b,
          b + event<e2> = y,
          *"init3"_s = c,
          c + event<e3> = z,
          a + msm::on_entry / [](std::vector<calls>& c) { c.push_back(calls::a_entry); },
          a + msm::on_exit  / [](std::vector<calls>& c) { c.push_back(calls::a_exit); },
          x + msm::on_entry / [](std::vector<calls>& c) { c.push_back(calls::x_entry); },
          x + msm::on_exit  / [](std::vector<calls>& c) { c.push_back(calls::x_exit); },
          b + msm::on_entry / [](std::vector<calls>& c) { c.push_back(calls::b_entry); },
          b + msm::on_exit  / [](std::vector<calls>& c) { c.push_back(calls::b_exit); },
          y + msm::on_entry / [](std::vector<calls>& c) { c.push_back(calls::y_entry); },
          y + msm::on_exit  / [](std::vector<calls>& c) { c.push_back(calls::y_exit); },
          c + msm::on_entry / [](std::vector<calls>& c) { c.push_back(calls::c_entry); },
          c + msm::on_exit  / [](std::vector<calls>& c) { c.push_back(calls::c_exit); },
          z + msm::on_entry / [](std::vector<calls>& c) { c.push_back(calls::z_entry); },
          z + msm::on_exit  / [](std::vector<calls>& c) { c.push_back(calls::z_exit); }
      );
      // clang-format on
    }
  };

  std::vector<calls> c_;
  msm::sm<D> sm{c_};
  expect(std::vector<calls>{calls::a_entry, calls::a1_entry, calls::b_entry, calls::b1_entry, calls::c_entry,
                            calls::c1_entry} == c_);

  c_.clear();
  sm.process_event(e2{});
  expect(std::vector<calls>{calls::b1_exit, calls::b_exit, calls::y_entry} == c_);
};
