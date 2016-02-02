//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/msm-lite.hpp"
#include <iostream>
#include <cassert>

namespace msm = boost::msm::lite;

struct sub {
  auto configure() const noexcept {
    using namespace msm;
    // clang-format off
      return make_transition_table(
        "idle"_s(H) == "s1"_s + "e1"_t / [] { std::cout << "in sub" << std::endl; }
      , "s1"_s      == X      + "e2"_t / [] { std::cout << "in sub again" << std::endl; }
      );
    // clang-format on
  }
};

struct history {
  auto configure() const noexcept {
    using namespace msm;
    state<sm<sub>> sub_state;

    // clang-format off
    return make_transition_table(
      "idle"_s(initial) == sub_state  + "e1"_t / [] { std::cout << "enter sub" << std::endl; }
    , sub_state         == "s1"_s     + "e3"_t / [] { std::cout << "exit sub" << std::endl; }
    , "s1"_s            == sub_state  + "e4"_t / [] { std::cout << "enter sub again" << std::endl; }
    );
    // clang-format on
  }
};

int main() {
  msm::sm<history> sm;
  using namespace msm;
  assert(sm.process_event("e1"_t));
  assert(sm.process_event("e1"_t));  // enter sub
  assert(sm.process_event("e3"_t));  // exit sub
  assert(sm.process_event("e4"_t));  // enter sub
  assert(sm.process_event("e2"_t));  // in sub again (history)
}
