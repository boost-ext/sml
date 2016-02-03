//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/msm-lite.hpp"
#include <cassert>

namespace msm = boost::msm::lite;

auto guard = [] { return true; };
auto action = [] {};

struct uml_notation {
  auto configure() const noexcept {
    using namespace msm;
    // clang-format off
    return make_transition_table(
        *"idle"_s == "s1"_s + "e1"_t [ guard ] / action
      , "s1"_s    == "s2"_s + "e2"_t [ guard ] / action
      , "s2"_s    == X      + "e3"_t [ guard ] / action
    );
    // clang-format on
  }
};

int main() {
  msm::sm<uml_notation> sm;
  using namespace msm;
  assert(sm.is("idle"_s));
  assert(sm.process_event("e1"_t));
  assert(sm.is("s1"_s));
  assert(sm.process_event("e2"_t));
  assert(sm.is("s2"_s));
  assert(sm.process_event("e3"_t));
  assert(sm.is(X));
}
