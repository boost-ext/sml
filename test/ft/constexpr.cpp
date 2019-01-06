//
// Copyright (c) 2016-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//
#if !defined(_MSC_VER)

#include <boost/sml.hpp>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <utility>
#include <vector>

namespace sml = boost::sml;

struct e1 {};
struct e2 {};

constexpr auto idle = sml::state<class idle>;
constexpr auto s1 = sml::front::state<class s1>{};

using namespace sml;
constexpr auto s2 = "s2"_s;
constexpr auto event1 = "e1"_e;
constexpr auto event2 = front::event<e2>{};

test terminate_state = [] {
  struct c {
    auto operator()() noexcept {
      // clang-format off
      return make_transition_table(
         *idle + event1 = s1
        , s1 + event1 = s2
        , s2 + event2 = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  expect(sm.is(idle));
  sm.process_event(event1());
  expect(sm.is(s1));
  sm.process_event(event1());
  expect(sm.is(s2));
  sm.process_event(e2{});
  expect(sm.is(sml::X));
};

#endif
