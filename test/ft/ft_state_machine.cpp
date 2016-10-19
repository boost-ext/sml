//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <string>
#include <utility>

namespace sml = boost::sml;

struct e1 {};

const auto idle = sml::state<class idle>;

test empty = [] {
  struct c {
    auto operator()() noexcept { return sml::make_transition_table(); }
  };

  sml::sm<c> sm;
};

test ctor = [] {
  struct c {
    auto operator()() noexcept { return sml::make_transition_table(); }
  };

  sml::sm<c> sm;
  sml::sm<c> sm_{std::move(sm)};
  (void)sm_;
};

test minimal = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      return make_transition_table(*idle + event<e1> / [] {});
    }
  };

  sml::sm<c> sm;
  expect(sm.is(idle));
  sm.process_event(e1{});
  expect(sm.is(idle));
};

test lambda_expr = [] {
  const auto lsm = [] {
    using namespace sml;
    return make_transition_table(*idle + event<e1> / [] {});
  };

  sml::sm<decltype(lsm)> sm{lsm};
  expect(sm.is(idle));
  sm.process_event(e1{});
  expect(sm.is(idle));
};
