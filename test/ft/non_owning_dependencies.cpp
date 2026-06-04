//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>

namespace sml = boost::sml;

struct settings {
  bool ok = false;
};
struct context {
  int value = 0;
};

struct e1 {};

const auto idle = sml::state<class idle>;

struct root {
  auto operator()() const noexcept {
    using namespace sml;
    const auto guard = [](const settings& s, const context& c) {
      expect(c.value == 0);
      return s.ok;
    };
    const auto action = [](const settings&, context& c) {
      c.value = 42;
    };
    return make_transition_table(*idle + event<e1> [guard] / action = X);
  }
};

test const_lvalue_dependency_injection = [] {
  const settings s{true};
  context c{};
  sml::sm<root> sm{s, c};
  expect(sm.is(idle));
  sm.process_event(e1{});
  expect(sm.is(sml::X));
  expect(c.value == 42);
};
