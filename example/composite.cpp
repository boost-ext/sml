//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <cassert>
#include <iostream>

namespace sml = boost::sml;

namespace {
struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};

struct sub {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
      return make_transition_table(
       *"idle"_s + event<e3> / [] { std::cout << "in sub sm" << std::endl; } = "s1"_s
      , "s1"_s + event<e4> / [] { std::cout << "finish sub sm" << std::endl; } = X
      );
    // clang-format on
  }
};

struct composite {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
     *"idle"_s + event<e1> = "s1"_s
    , "s1"_s + event<e2> / [] { std::cout << "enter sub sm" << std::endl; } = state<sub>
    , state<sub> + event<e5> / [] { std::cout << "exit sub sm" << std::endl; } = X
    );
    // clang-format on
  }
};
}  // namespace

int main() {
  sml::sm<composite> sm;

  using namespace sml;
  assert(sm.is("idle"_s));
  assert(sm.is<decltype(state<sub>)>("idle"_s));

  sm.process_event(e1{});
  assert(sm.is("s1"_s));
  assert(sm.is<decltype(state<sub>)>("idle"_s));

  sm.process_event(e2{});  // enter sub sm
  assert(sm.is(state<sub>));
  assert(sm.is<decltype(state<sub>)>("idle"_s));

  sm.process_event(e3{});  // in sub sm
  assert(sm.is(state<sub>));
  assert(sm.is<decltype(state<sub>)>("s1"_s));

  sm.process_event(e4{});  // finish sub sm
  assert(sm.is(state<sub>));
  assert(sm.is<decltype(state<sub>)>(X));

  sm.process_event(e5{});  // exit sub sm
  assert(sm.is(X));
  assert(sm.is<decltype(state<sub>)>(X));
}
