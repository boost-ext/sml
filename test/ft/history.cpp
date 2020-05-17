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

const auto idle = sml::state<class idle>;
const auto idle2 = sml::state<class idle2>;
const auto s1 = sml::state<class s1>;
const auto s2 = sml::state<class s2>;
const auto s3 = sml::state<class s3>;

test history = [] {
  struct sub {
    auto operator()() noexcept {
      using namespace sml;

      // clang-format off
      return make_transition_table(
          idle(H) + event<e1> = s1
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

  sml::sm<c> sm{};

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
  expect(sm.is<decltype(state<sub>)>(s2));  // history
};

test no_history_reenter_sub = [] {
  enum class calls { s_a_entry, s_b_entry, s_c_entry, s_a_exit, s_b_exit, s_c_exit, s_entry, s_exit, x_entry, x_exit };

  struct sub {
    auto operator()() const noexcept {
      using namespace sml;
      const auto a = state<class A>;
      const auto b = state<class B>;
      const auto c = state<class C>;
      // clang-format off
      return make_transition_table(
        *a + event<e1> = b,
         b + event<e2> = c,
         a + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::s_a_entry); },
         b + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::s_b_entry); },
         c + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::s_c_entry); },
         a + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::s_a_exit); },
         b + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::s_b_exit); },
         c + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::s_c_exit); }
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() const noexcept {
      using namespace sml;
      const auto s = state<sub>;
      const auto x = state<class X>;

      // clang-format off
      return make_transition_table(
        *idle = s,
         s + event<e3> = x,
         x + event<e4> = s,
         s + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::s_entry); },
         x + sml::on_entry<_> / [](std::vector<calls>& c) { c.push_back(calls::x_entry); },
         s + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::s_exit); },
         x + sml::on_exit<_>  / [](std::vector<calls>& c) { c.push_back(calls::x_exit); }
      );
      // clang-format on
    }
  };

  std::vector<calls> c_;
  sml::sm<c> sm{c_};
  expect(std::vector<calls>{calls::s_entry, calls::s_a_entry} == c_);

  c_.clear();
  sm.process_event(e1{});
  expect(std::vector<calls>{calls::s_a_exit, calls::s_b_entry} == c_);

  c_.clear();
  sm.process_event(e2{});
  expect(std::vector<calls>{calls::s_b_exit, calls::s_c_entry} == c_);

  c_.clear();
  sm.process_event(e3{});
  expect(std::vector<calls>{calls::s_c_exit, calls::s_exit, calls::x_entry} == c_);

  c_.clear();
  sm.process_event(e4{});
  expect(std::vector<calls>{calls::x_exit, calls::s_entry, calls::s_a_entry /*no history*/} == c_);
};

test history_region = [] {
  struct sub {
    auto operator()() noexcept {
      using namespace sml;

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

  sml::sm<c> sm;

  expect(sm.is(idle));
  sm.process_event(e1());
  expect(sm.is(state<sub>));
  expect(sm.is<decltype(state<sub>)>(idle, idle2));

  sm.process_event(e4());

  sm.process_event(e1());
  expect(sm.is(state<sub>));
  expect(sm.is<decltype(state<sub>)>(s1, idle2));

  sm.process_event(e2());
  expect(sm.is(state<sub>));
  expect(sm.is<decltype(state<sub>)>(s2, s3));

  sm.process_event(e3());
  expect(sm.is(s1));
  expect(sm.is<decltype(state<sub>)>(s2, s3));

  sm.process_event(e4());
  expect(sm.is(state<sub>));
  expect(sm.is<decltype(state<sub>)>(s2, idle2));  // history for region 1
};
