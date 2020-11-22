//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <cassert>
#include <queue>

namespace sml = boost::sml;

namespace {
struct e1 {
  int i = 0;
};
struct dependency {
  int i = 0;
};

struct dependencies {
  auto operator()() const noexcept {
    using namespace sml;

    const auto guard = [](const dependency& d) {  /// more dependencies might be passed
      return !d.i;
    };

    const auto action = [](dependency& d,
                           const auto& event) {  /// event is deduced, order is not important
      d.i = event.i + 42;
    };

    // clang-format off
    return make_transition_table(
       *"idle"_s + event<e1> [ guard ] / action = "s1"_s
      , "s1"_s   + event<e1> [ ([](const auto& event, const dependency& d) { return d.i == event.i; }) ] = X
     );
    // clang-format on
  }
};
}  // namespace

int main() {
  using namespace sml;
  dependency d;
  sm<dependencies> sm{d};  /// pass all dependencies (from guards, actions) via
                           /// constructor (order is not important)
  assert(sm.is("idle"_s));

  sm.process_event(e1{0});
  assert(sm.is("s1"_s));  /// action sets dependency.i to 42

  sm.process_event(e1{0});
  assert(sm.is("s1"_s));  /// not satisfied, dependency.i(42) != event.i(0)

  sm.process_event(e1{42});
  assert(sm.is(X));  /// transition to X
}
