//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// clang-format off
#include <cassert>
#include <iostream>
#include "boost/sml.hpp"

namespace sml = boost::sml;

struct e1 {};
struct e2 {};

int main() {
  using namespace sml;

  auto guard = [] {
    std::cout << "guard" << std::endl;
    return true;
  };

  auto action = [] {
    std::cout << "action" << std::endl;
  };

  auto sm = make_sm([&]{
    return make_transition_table(
      *("idle"_s) + event<e1> = "s1"_s
      , "s1"_s    + event<e2> [ guard ] / action = X
    );
  });

  std::cout << "sizeof(sm): " << sizeof(sm) << "b" << std::endl;

  assert(sm.is("idle"_s));
  sm.process_event(e1{});
  assert(sm.is("s1"_s));
  sm.process_event(e2{});
  assert(sm.is(X)); /// terminate state
}
