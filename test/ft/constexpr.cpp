//
// Copyright (c) 2016-2018 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <utility>
#include <vector>

namespace sml = boost::sml;

struct e1 {};
struct e2 {};
struct e3 {};

constexpr auto idle = sml::state<class idle>;
constexpr auto s1 = sml::front::state<class s1>{};
using namespace sml;
constexpr auto s2 = "s2"_s;

test terminate_state = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  expect(sm.is(idle));
  sm.process_event(e1{});
  expect(sm.is(s1));
  sm.process_event(e2{});
  expect(sm.is(sml::X));
  sm.process_event(e1{});
  sm.process_event(e2{});
  sm.process_event(e3{});
  expect(sm.is(sml::X));
};

