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

const auto idle = msm::state<class idle>;

test empty = [] {
  struct c {
    auto operator()() noexcept { return msm::make_transition_table(); }
  };

  msm::sm<c> sm;
};

test ctor = [] {
  struct c {
    auto operator()() noexcept { return msm::make_transition_table(); }
  };

  msm::sm<c> sm;
  msm::sm<c> sm_{std::move(sm)};
  (void)sm_;
};

test minimal = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      return make_transition_table(*idle + event<e1> / [] {});
    }
  };

  msm::sm<c> sm;
  expect(sm.is(idle));
  sm.process_event(e1{});
  expect(sm.is(idle));
};
