//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <iostream>
#include "boost/msm-lite.hpp"

namespace msm = boost::msm::lite;

struct sub {
  auto operator()() const noexcept {
    using namespace msm;
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
    using namespace msm;
    // clang-format off
    return make_transition_table(
      state<sub> <= *"idle"_s  + "e1"_e / [] { std::cout << "enter sub" << std::endl; }
    , "s1"_s     <= state<sub> + "e3"_e / [] { std::cout << "exit sub" << std::endl; }
    , state<sub> <= "s1"_s     + "e4"_e / [] { std::cout << "enter sub again" << std::endl; }
    );
    // clang-format on
  }
};

int main() {
  msm::sm<history> sm;
  using namespace msm;
  sm.process_event("e1"_e);
  sm.process_event("e1"_e);  // enter sub
  sm.process_event("e3"_e);  // exit sub
  sm.process_event("e4"_e);  // enter sub
  sm.process_event("e2"_e);  // in sub again (history)
}
