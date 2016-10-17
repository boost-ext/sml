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
#include "boost/msm-lite/testing/state_machine.hpp"

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

test sm_testing = [] {
  struct data {
    int value = 0;
  };

  struct c {
    auto operator()() noexcept {
      using namespace msm;

      auto guard = [](const data &d) { return d.value == 42; };
      auto action = [](data &d) { d.value = 123; };

      struct Action {
        void operator()(data &d) noexcept { d.value = 12; }
      };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> [ guard ] / action = s1
        , s1 + event<e2> [ guard && [](bool b) { return b; } ] / Action{} = s2
        , s2 + event<e3> [ guard ] / [](data& d) { d.value = 87; } = X
      );
      // clang-format on
    }
  };

  {
    data fake_data;
    const data &c_fake_data = fake_data;
    msm::testing::sm<c> sm{c_fake_data, fake_data};
    expect(sm.is(idle));

    sm.process_event(e1{});
    expect(sm.is(idle));
    expect(0 == fake_data.value);

    fake_data.value = 42;
    sm.process_event(e1{});
    expect(sm.is(s1));
    expect(123 == fake_data.value);
  }

  {
    data fake_data;
    const data &c_fake_data = fake_data;
    msm::testing::sm<c> sm{fake_data, c_fake_data};
    expect(sm.is(idle));

    sm.set_current_states(s2);
    fake_data.value = 42;
    sm.process_event(e3{});
    expect(sm.is(msm::X));
    expect(87 == fake_data.value);
  }

  {
    data fake_data;
    const data &c_fake_data = fake_data;
    msm::testing::sm<c> sm{c_fake_data, fake_data};
    expect(sm.is(idle));

    sm.set_current_states(s2);
    fake_data.value = 42;
    sm.process_event(e3{});
    expect(sm.is(msm::X));
    expect(87 == fake_data.value);
  }

  {
    data fake_data;
    const data &c_fake_data = fake_data;
    msm::testing::sm<c> sm{fake_data, c_fake_data, true};
    expect(sm.is(idle));

    sm.set_current_states(s1);
    fake_data.value = 42;
    sm.process_event(e2{});
    expect(sm.is(s2));
    expect(12 == fake_data.value);
  }
};

test sm_testing_orthogonal_regions = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> = X

        ,*idle2 + event<e1> = s3
        , s3 + event<e2> = X
      );
      // clang-format on
    }
  };

  msm::testing::sm<c> sm;
  expect(sm.is(idle, idle2));
  sm.set_current_states(s1, s3);
  expect(sm.is(s1, s3));
};
