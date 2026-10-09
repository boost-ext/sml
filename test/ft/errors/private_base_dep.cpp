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

// The only dep deriving from the requested type has it as a private base: reading it must
// not compile, also next to other reference deps (before #715 such a dep was only rejected
// as the pool's only reference dep; otherwise the lookup fell back to a default-constructed
// base, which the const base& bound as a dangling temporary).
struct private_base_dep {
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
  private_derived d;
  int i = 0;
  sml::sm<private_base_dep> sm{d, i};
  sm.process_event(e1{});
}
