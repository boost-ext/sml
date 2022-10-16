//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <cassert>

namespace sml = boost::sml;

namespace {
struct e1 {};

struct eval {
  auto operator()() const {
    const auto guard = [] { return true; };
    const auto action = [](int &a) { ++a; };

    // clang-format off
    using namespace sml;
    using sml::eval;
    return make_transition_table(
       *"idle"_s + event<e1> [ guard ] / (action, eval [ guard ] / action, action) = X
    );
    // clang-format on
  }
};
}  // namespace

int main() {
  int a{};
  sml::sm<eval> sm{a};
  sm.process_event(e1{});
  assert(3 == a);
  assert(sm.is(sml::X));
}

