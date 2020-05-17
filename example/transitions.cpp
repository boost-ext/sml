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

struct transitions {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
       *"idle"_s                  / [] { std::cout << "anonymous transition" << std::endl; } = "s1"_s
      , "s1"_s + event<e1>        / [] { std::cout << "internal transition" << std::endl; }
      , "s1"_s + event<e2>        / [] { std::cout << "self transition" << std::endl; } = "s1"_s
      , "s1"_s + sml::on_entry<_> / [] { std::cout << "s1 entry" << std::endl; }
      , "s1"_s + sml::on_exit<_>  / [] { std::cout << "s1 exit" << std::endl; }
      , "s1"_s + event<e3>        / [] { std::cout << "external transition" << std::endl; } = X
    );
    // clang-format on
  }
};
}  // namespace

int main() {
  sml::sm<transitions> sm;
  sm.process_event(e1{});
  sm.process_event(e2{});
  sm.process_event(e3{});
  assert(sm.is(sml::X));
}
