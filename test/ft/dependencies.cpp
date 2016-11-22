//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <string>
#include <type_traits>
#include <utility>

namespace sml = boost::sml;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};
struct e6 {};

const auto idle = sml::state<class idle>;

test minimal_with_dependency = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      return make_transition_table(*idle + event<e1> / [](int i) { expect(42 == i); });
    }
  };

  sml::sm<c> sm{42};
  expect(sm.is(idle));
  sm.process_event(e1{});
  expect(sm.is(idle));
};

test dependencies = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;

      auto guard = [](int i) {
        expect(42 == i);
        return true;
      };

      auto action = [](double d, e1) { expect(d == 87.0); };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> [ guard ] / (action, [](const auto& e, int i) -> void {
            (void)e;
            expect(std::is_same<decltype(e), const e1&>::value);
            expect(42 == i);
          }) = X
      );
      // clang-format on
    }
  };

  {
    sml::sm<c> sm{42, 87.0};
    sm.process_event(e1{});
    expect(sm.is(sml::X));
  }

  {
    sml::sm<c> sm{87.0, 42};
    sm.process_event(e1{});
    expect(sm.is(sml::X));
  }
};
