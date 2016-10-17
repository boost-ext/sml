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
#include "boost/msm-lite/utility/dispatch_table.hpp"

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

struct runtime_event {
  int id = 0;
};
struct event1 {
  static constexpr auto id = 1;
  explicit event1(const runtime_event &) {}
};
struct event2 {
  static constexpr auto id = 2;
};
struct event3 {
  static constexpr auto id = 3;
  explicit event3(const runtime_event &) {}
};
struct event4 {};

test dispatchable_concept = [] {
  using namespace msm;
  static_expect(!concepts::dispatchable<runtime_event, aux::type_list<event4, event1>>::value);
  static_expect(concepts::dispatchable<runtime_event, aux::type_list<>>::value);
  static_expect(concepts::dispatchable<runtime_event, aux::type_list<event1, event2>>::value);
  static_expect(concepts::dispatchable<runtime_event, aux::type_list<event2, event1>>::value);
};

test dispatch_runtime_event = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
         *idle2 = idle // anonymous transition
        , idle + event<event1> = s1
        , s1 + event<event2> = s2
        , s2 + event<event3> = X
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  expect(sm.is(idle));
  auto dispatcher = msm::make_dispatch_table<runtime_event, 1 /*min*/, 10 /*max*/>(sm);

  {
    runtime_event event{1};
    dispatcher(event, event.id);
    expect(sm.is(s1));
  }

  {
    runtime_event event{9};
    dispatcher(event, event.id);
    expect(sm.is(s1));
  }

  {
    runtime_event event{2};
    dispatcher(event, event.id);
    expect(sm.is(s2));
  }

  {
    runtime_event event{3};
    dispatcher(event, event.id);
    expect(sm.is(msm::X));
  }

  {
    runtime_event event{5};
    dispatcher(event, event.id);
    expect(sm.is(msm::X));
  }
};

test dispatch_runtime_event_sub_sm = [] {
  static auto in_sub = 0;

  struct sub {
    auto operator()() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
         *idle + event<event2> / [] { in_sub++; } = s1
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
         *idle + event<event1> = state<sub>
        , state<sub> + event<event3> = X
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  expect(sm.is(idle));

  auto dispatcher = msm::make_dispatch_table<runtime_event, 1 /*min*/, 4 /*max*/>(sm);

  {
    runtime_event event{1};
    dispatcher(event, event.id);
    expect(sm.is(msm::state<sub>));
    expect(0 == in_sub);
  }

  {
    runtime_event event{2};
    dispatcher(event, event.id);
    expect(sm.is(msm::state<sub>));
    expect(1 == in_sub);
  }

  {
    runtime_event event{3};
    dispatcher(event, event.id);
    expect(sm.is(msm::X));
    expect(1 == in_sub);
  }

  {
    runtime_event event{4};
    dispatcher(event, event.id);
    expect(sm.is(msm::X));
    expect(1 == in_sub);
  }
};
