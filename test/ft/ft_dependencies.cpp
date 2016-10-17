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

test minimal_with_dependency = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      return make_transition_table(*idle + event<e1> / [](int i) { expect(42 == i); });
    }
  };

  msm::sm<c> sm{42};
  expect(sm.is(idle));
  sm.process_event(e1{});
  expect(sm.is(idle));
};

test dependencies = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;

      auto guard = [](int i) {
        expect(i == 42);
        return true;
      };

      auto action = [](double d, auto event) {
        expect(d == 87.0);
        expect(msm::aux::is_same<e1, decltype(event)>::value);
      };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> [ guard ] / action = X
      );
      // clang-format on
    }
  };

  {
    msm::sm<c> sm{42, 87.0};
    sm.process_event(e1{});
    expect(sm.is(msm::X));
  }

  {
    msm::sm<c> sm{87.0, 42};
    sm.process_event(e1{});
    expect(sm.is(msm::X));
  }
};
