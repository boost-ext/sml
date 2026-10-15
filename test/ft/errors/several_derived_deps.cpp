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

struct base {};
struct derived_a : base {};
struct derived_b : base {};

struct several_derived_deps {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
        *idle + event<e1> / [](const base &) {} = X // derived_a or derived_b?
    );
    // clang-format on
  }
};

// Not main(): common/test.hpp is force-included and defines it.
void construct_sm() {
  derived_a a;
  derived_b b;
  sml::sm<several_derived_deps> sm{a, b};
  (void)sm;
}
