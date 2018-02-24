//
// Copyright (c) 2016-2018 Kris Jusiak (kris at jusiak dot net)
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

test dependencies_with_const = [] {
  struct dep {
    int i = 7;
  };

  struct c {
    auto operator()() noexcept {
      using namespace sml;

      auto guard = [](int i, dep& dependency) {
        expect(7 == dependency.i);
        expect(42 == i);
        return true;
      };

      auto const_guard = [](dep const& dependency, int const i) {
        expect(7 == dependency.i);
        expect(42 == i);
        return true;
      };

      auto action = [](int i, dep& dependency, e1) {
        expect(7 == dependency.i);
        expect(42 == i);
      };

      auto const_action = [](dep const& dependency, int const i, e1) {
        expect(7 == dependency.i);
        expect(42 == i);
      };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> [ guard && const_guard ] / (action, const_action) = X
      );
      // clang-format on
    }
  };

  {
    dep dependency;
    sml::sm<c> sm{dependency, 42};
    sm.process_event(e1{});
    expect(sm.is(sml::X));
  }
};
