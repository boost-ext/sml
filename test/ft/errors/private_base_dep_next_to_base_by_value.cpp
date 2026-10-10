//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>

namespace sml = boost::sml;

struct e1 {};
const auto idle = sml::state<class idle>;

struct base {
  int val = 1;
};
struct private_derived : private base {};

// The only dep deriving from the requested type has it as a private base, and a base is
// also passed by value. A base parameter by value reads that one, but a const base& must
// not: it would bind the copy that the lookup returns, a dangling temporary.
struct private_base_dep_next_to_base_by_value {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
        *idle + event<e1> / [](const base &, int &) {} = X // base is inaccessible in private_derived
    );
    // clang-format on
  }
};

// Only has to fail to compile: common/test.hpp (force-included) already defines main().
void construct_sm() {
  base b;
  private_derived d;
  int i = 0;
  sml::sm<private_base_dep_next_to_base_by_value> sm{static_cast<base &&>(b), d, i};
  sm.process_event(e1{});
}
