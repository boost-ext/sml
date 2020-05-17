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
struct e2 {};
struct e3 {};

struct orthogonal_regions {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
     *"idle"_s + event<e1> = "s1"_s
    , "s1"_s + event<e2> = X

    ,*"idle2"_s + event<e2> = "s2"_s
    , "s2"_s + event<e3> = X
    );
    // clang-format on
  }
};
}  // namespace

int main() {
  sml::sm<orthogonal_regions> sm;
  using namespace sml;
  assert(sm.is("idle"_s, "idle2"_s));
  sm.process_event(e1{});
  assert(sm.is("s1"_s, "idle2"_s));
  sm.process_event(e2{});
  assert(sm.is(X, "s2"_s));
  sm.process_event(e3{});
  assert(sm.is(X, X));
}
