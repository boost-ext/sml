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

test orthogonal_regions_entry_exit = [] {
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
