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

struct states {
  auto operator()() const noexcept {
    using namespace sml;
    const auto idle = state<class idle>;
    // clang-format off
    return make_transition_table(
       *idle + event<e1> = "s1"_s
      , "s1"_s + sml::on_entry<_> / [] { std::cout << "s1 on entry" << std::endl; }
      , "s1"_s + sml::on_exit<_> / [] { std::cout << "s1 on exit" << std::endl; }
      , "s1"_s + event<e2> = state<class s2>
      , state<class s2> + event<e3> = X
    );
    // clang-format on
  }
};
}  // namespace

int main() {
  sml::sm<states> sm;
  sm.process_event(e1{});
  sm.process_event(e2{});
  sm.process_event(e3{});
  assert(sm.is(sml::X));
}
