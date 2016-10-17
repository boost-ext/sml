//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <iostream>
#include <stdexcept>
#include "boost/msm-lite.hpp"

namespace msm = boost::msm::lite;

struct error_handling {
  auto operator()() const {
    using namespace msm;
    // clang-format off
    return make_transition_table(
       *"idle"_s + "event1"_e / [] { throw std::runtime_error{"error"}; }
      , "idle"_s + "event2"_e / [] { throw 0; }
      , *"error_handling"_s + exception<std::runtime_error> / [] { std::cout << "exception caught" << std::endl; }
      ,  "error_handling"_s + exception<> / [] { std::cout << "generic exception caught, terminate..." << std::endl; } = X
    );
    // clang-format on
  }
};

int main() {
  using namespace msm;
  sm<error_handling> sm;
  sm.process_event("event1"_e);
  assert(sm.is("idle"_s, "error_handling"_s));
  sm.process_event("event2"_e);
  assert(sm.is("idle"_s, X));
  assert(sm.is(X));  // any region is terminated
}
