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
struct sub {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
      "s1"_s <= "idle"_s(H) + "e1"_e / [] { std::cout << "in sub" << std::endl; }
    , X      <= "s1"_s      + "e2"_e / [] { std::cout << "in sub again" << std::endl; }
    );
    // clang-format on
  }
};

struct history {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
      state<sub> <= *"idle"_s  + "e1"_e / [] { std::cout << "enter sub" << std::endl; }
    , "s1"_s     <= state<sub> + "e3"_e / [] { std::cout << "exit sub" << std::endl; }
    , state<sub> <= "s1"_s     + "e4"_e / [] { std::cout << "enter sub again" << std::endl; }
    );
    // clang-format on
  }
};
}  // namespace

int main() {
  sml::sm<history> sm;
  using namespace sml;
  sm.process_event("e1"_e());  // enter sub
  sm.process_event("e1"_e());  // in sub
  sm.process_event("e3"_e());  // exit sub
  sm.process_event("e4"_e());  // enter sub again
  sm.process_event("e2"_e());  // in sub again
}
